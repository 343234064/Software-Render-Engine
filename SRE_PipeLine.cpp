//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_PipeLine.cpp
// Date: 2016/11/26
// Description:
//       Implements of mesh functions and classes which declared
//       in SRE_PipeLine.h
//
//
//*****************************************************
#include <string>
#include "SRE_PipeLine.h"


namespace SRE {
     //=============================
	//Class BasicProcessor functions
	//
	//
	//=============================
    void BasicProcessor::Start()
    {
#ifdef _SRE_DEBUGLOG_
        if(nullptr == this->m_pInputQueue)
        {
           g_log.Write("Processor Ignore Input");
        }
        if(nullptr == this->m_pOutputQueue)
        {
           g_log.Write("Processor Ignore Output");
        }
        if(nullptr == this->m_pObserver)
        {
           g_log.Write("Processor Ignore Message Feedback");
        }
#endif // _SRE_DEBUGLOG_

        if(!m_Started)
        {
           if(nullptr != m_callBacks)
              m_callBacks->OnStart();
           m_thread = std::thread(&BasicProcessor::Run, this);
           m_Started = true;
        }
    }


    void BasicProcessor::Run()
    {
        while(true)
        {
            if(m_Cancel)
            {
                if(nullptr != m_callBacks)
                   m_callBacks->OnCancel();
                m_Cancel = false;
                break;
            }

            if(m_Pause)
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                if(nullptr != m_callBacks)
                   m_callBacks->OnPause();
                m_cond.wait(lock,  [this]{
					return m_Cancel||!m_Pause;
			    });
            }


            try
            {
                if(nullptr != m_callBacks)
                {
                	m_callBacks->HandleElement();
                }

            }
            catch(...)
            {
                if(nullptr != m_callBacks)
                   m_callBacks->OnRunError();
                if(nullptr != m_pObserver)
                   m_pObserver->Notify(SRE_MESSAGE_RUNERROR, m_id);
                throw;
            }

        }

        if(nullptr != m_callBacks)
            m_callBacks->OnRunFinish();

        m_Started = false;
        return;
    }





    //===========================================
	//Class InputAssembler functions
	//
	//
	//===========================================
    void InputAssembler::SetVertexAndIndexBuffers(VertexBuffer* vertexBuffer, Buffer<INT>* indexBuffer)
    {
        if(nullptr == vertexBuffer) return;
        std::lock_guard<std::mutex> lock(m_resMutex);
        m_vertexBuffers.push(vertexBuffer);
        m_indexBuffers.push(indexBuffer);

    }

    void InputAssembler::HandleElement()
    {
        if(nullptr != m_pCurrentHandleVbuffer)
        {
            INT actualIndex1 = 0, actualIndex2 = 0, actualIndex3 = 0;
            INT prevIndex = m_currentIndex;

            if(m_pConstantBuffer->primitiveTopology == SRE_PRIMITIVETYPE_TRIANGLELIST)
            {
                actualIndex1 = m_currentIndex;
                actualIndex2 = m_currentIndex + 1;
                actualIndex3 = m_currentIndex + 2;
                m_currentIndex = m_currentIndex + 3;
            }
            else if(m_pConstantBuffer->primitiveTopology == SRE_PRIMITIVETYPE_TRIANGLEFAN)
            {
                m_currentIndex = m_currentIndex + 1;
                actualIndex1 = m_currentIndex2;
                actualIndex2 = m_currentIndex;
                actualIndex3 = m_currentIndex + 1;
            }
            else //if(m_primitiveTopology == SRE_PRIMITIVETYPE_TriangleSTRIP)
            {
                actualIndex1 = m_currentIndex;
                actualIndex2 = m_currentIndex + 1;
                actualIndex3 = m_currentIndex + 2;
                m_currentIndex = m_currentIndex + 1;
            }

            INT vertexNum = m_pCurrentHandleVbuffer->GetVertexNumber();
            if(nullptr != m_pCurrentHandleIbuffer)
            {
                if(actualIndex3 >= m_pCurrentHandleIbuffer->GetBufferSize())
                {
                    m_pCurrentHandleVbuffer = nullptr;
                    return;
                }
                actualIndex1 = m_pCurrentHandleIbuffer->GetData(actualIndex1);
                actualIndex2 = m_pCurrentHandleIbuffer->GetData(actualIndex2);
                actualIndex3 = m_pCurrentHandleIbuffer->GetData(actualIndex3);

                if(actualIndex1 >= vertexNum || actualIndex1 < 0) {m_currentIndex = prevIndex+1;m_currentIndex2 = m_currentIndex2+1;return;}
                if(actualIndex2 >= vertexNum || actualIndex2 < 0) {m_currentIndex = prevIndex+1;return;}
                if(actualIndex3 >= vertexNum || actualIndex3 < 0) {m_currentIndex = prevIndex+1;return;}

            }
            else if(actualIndex3 >= m_pCurrentHandleVbuffer->GetVertexNumber())
            {
                m_pCurrentHandleVbuffer = nullptr;
                return;
            }

            SendVertex(actualIndex1);
            SendVertex(actualIndex2);
            SendVertex(actualIndex3);

        }
        else
        {
            std::unique_lock<std::mutex> lock(m_resMutex);
            m_cond.wait(lock, [this]{
					return m_Cancel||(!m_vertexBuffers.empty()&&m_valve);
			});
            if(m_Cancel) return;

            m_pCurrentHandleVbuffer = m_vertexBuffers.front();
            if(nullptr != m_pCurrentHandleVbuffer)
            {
                m_pCurrentHandleIbuffer = m_indexBuffers.front();
                m_indexBuffers.pop();

                m_currentIndex = 0;
                m_pCurrentHandleVbuffer->ResetMark(false);
            }
            else
            {
                Output(nullptr);
            }
            m_vertexBuffers.pop();
        }

    }

    void InputAssembler::SendVertex(INT & actualIndex)
    {
        _index_ver_* v=new _index_ver_();
        if(nullptr == v)
        {
#ifdef _SRE_DEBUG_
            _ERRORLOG(SRE_ERROR_OUTOFMEMORY);
#endif // _SRE_DEBUG_
            return;
        }
        if(!(m_pCurrentHandleVbuffer->GetMark(actualIndex)))
        {
            BYTE* vertex=m_pCurrentHandleVbuffer->GetVertex(actualIndex);
            if(nullptr == vertex)
            {
#ifdef _SRE_DEBUG_
               _ERRORLOG(SRE_ERROR_OUTOFMEMORY);
#endif // _SRE_DEBUG_
               return;
            }

            v->vertex = vertex;
            m_pCurrentHandleVbuffer->SetMark(actualIndex, true);
        }
        else
        {
            v->vertex = nullptr;
        }
        v->index = actualIndex;
        Output((BasicIOElement*)v);
    }

    void InputAssembler::HandleMessage(SREVAR message, USINT id)
    {
#ifdef _SRE_DEBUG_
        g_log.WriteKV("InputAssembler GetMessage From:", id);
#endif // _SRE_DEBUG_
    }

    void InputAssembler::OnCancel()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("InputAssembler Cancel!");
#endif // _SRE_DEBUG_
    }

    void InputAssembler::OnPause()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("InputAssembler Pause!");
#endif // _SRE_DEBUG_
    }

    void InputAssembler::OnResume()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("InputAssembler Resume!");
#endif // _SRE_DEBUG_
    }

    void InputAssembler::OnRunError()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("InputAssembler Run Error!");
#endif // _SRE_DEBUG_
    }

    void InputAssembler::OnRunFinish()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("InputAssembler Run Finish!");
#endif // _SRE_DEBUG_
    }

    void InputAssembler::OnStart()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("InputAssembler Start!");
#endif // _SRE_DEBUG_
    }



    //===========================================
	//Class VertexProcessor functions
	//
	//
	//===========================================
    void VertexProcessor::HandleElement()
    {
        std::shared_ptr<BasicIOElement> p = GetInput();
        _index_ver_* v = (_index_ver_*)p.get();

        if(nullptr == v) {
				SetArgus();
        		Output(nullptr);
        		return;
		}

        bool  found = false;
        USINT coverIndex = 0;
        USINT threshold = 65528;

        for(USINT i=0; i<3; i++)
        {
            if(v->index == m_cachedVertexIndex[i])
            {
                VSOutput * out = new VSOutput(m_cachedVertex[i]);
                if(nullptr != out)
                {
                    Output((BasicIOElement*)out);
                }
#ifdef _SRE_DEBUG_
                else
                    _ERRORLOG(SRE_ERROR_OUTOFMEMORY);
#endif // _SRE_DEBUG_

                found = true;
                m_cachedPriority[i]=1;
                continue;
            }
            else
            {
                m_cachedPriority[i]=m_cachedPriority[i]<<1;
            }

            if(m_cachedPriority[i]&threshold)
                coverIndex = i;
        }

        if(!found)
        {
            VSOutput * out = m_pVertexShader->Run(v->vertex);
            if(nullptr != out)
            {
                Output((BasicIOElement*)out);
                m_cachedVertex[coverIndex] = *out;
                m_cachedVertexIndex[coverIndex] = v->index;
                m_cachedPriority[coverIndex] = 1;
            }
        }
    }

    void VertexProcessor::SetArgus()
    {
        if(nullptr == m_pCVertexShader)
		{
#ifdef _SRE_DEBUG_
           g_log.Write("VertexProcessor:vertex shader is null!");
#endif
			this->Cancel();
			return;
		}

#ifdef _SRE_DEBUG_
        if(nullptr == m_pVariableBuffer)
           g_log.Write("Warning,VertexProcessor->variable buffer is null");
#endif // _SRE_DEBUG_

    	m_pVertexShader = m_pCVertexShader;
    	m_pCVariableBuffer = m_pCVariableBuffer;
    }

    void VertexProcessor::HandleMessage(SREVAR message, USINT id)
    {
#ifdef _SRE_DEBUG_
        g_log.WriteKV("VertexProcessor GetMessage From:", id);
#endif // _SRE_DEBUG_
    }

    void VertexProcessor::OnCancel()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("VertexProcessor Cancel!");
#endif // _SRE_DEBUG_
    }

    void VertexProcessor::OnPause()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("VertexProcessor Pause!");
#endif // _SRE_DEBUG_
    }

    void VertexProcessor::OnResume()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("VertexProcessor Resume!");
#endif // _SRE_DEBUG_
    }

    void VertexProcessor::OnRunError()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("VertexProcessor Run Error!");
#endif // _SRE_DEBUG_
    }

    void VertexProcessor::OnRunFinish()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("VertexProcessor Run Finish!");
#endif // _SRE_DEBUG_
    }

    void VertexProcessor::OnStart()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("VertexProcessor Start!");
#endif // _SRE_DEBUG_
    }





    //===========================================
	//Class PrimitiveAssembler functions
	//
	//
	//===========================================
    void PrimitiveAssembler::HandleElement()
    {
        std::shared_ptr<BasicIOElement> pv1=GetInput();
        VSOutput* v1=(VSOutput*)pv1.get();
        if(nullptr == v1)
			{Output(nullptr);return;}

        std::shared_ptr<BasicIOElement> pv2=GetInput();
        std::shared_ptr<BasicIOElement> pv3=GetInput();

	    VSOutput* v2=(VSOutput*)pv2.get();
	    VSOutput* v3=(VSOutput*)pv3.get();

        _Triangle_* out = new _Triangle_(*v1, *v2, *v3);

        if(nullptr != out)
        {
            Output((BasicIOElement*)out);
        }
        else
        {
#ifdef _SRE_DEBUG_
            _ERRORLOG(SRE_ERROR_OUTOFMEMORY);
#endif // _SRE_DEBUG_
        }
    }

    void PrimitiveAssembler::HandleMessage(SREVAR message, USINT id)
    {
#ifdef _SRE_DEBUG_
        g_log.WriteKV("PrimitiveAssembler GetMessage From:", id);
#endif // _SRE_DEBUG_
    }

    void PrimitiveAssembler::OnCancel()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("PrimitiveAssembler Cancel!");
#endif // _SRE_DEBUG_
    }

    void PrimitiveAssembler::OnPause()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("PrimitiveAssembler Pause!");
#endif // _SRE_DEBUG_
    }

    void PrimitiveAssembler::OnResume()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("PrimitiveAssembler Resume!");
#endif // _SRE_DEBUG_
    }

    void PrimitiveAssembler::OnRunError()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("PrimitiveAssembler Run Error!");
#endif // _SRE_DEBUG_
    }

    void PrimitiveAssembler::OnRunFinish()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("PrimitiveAssembler Run Finish!");
#endif // _SRE_DEBUG_
    }

    void PrimitiveAssembler::OnStart()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("PrimitiveAssembler Start!");
#endif // _SRE_DEBUG_
    }



    //===========================================
	//Class VertexPostProcessor functions
	//
    //
    //I use Sutherland and Hodgman Clipping Algorithm
	//===========================================
	void VertexPostProcessor::TriangleClipping()
	{
	    if(m_pCurrentTriangle->v[0].vertex.w <= 0) return;
	    if(m_pCurrentTriangle->v[1].vertex.w <= 0) return;
	    if(m_pCurrentTriangle->v[2].vertex.w <= 0) return;

	    m_vlist[0].Add_back(_vertex_(m_pCurrentTriangle->v[0].vertex, 0, 0, 0));
	    m_vlist[0].Add_back(_vertex_(m_pCurrentTriangle->v[1].vertex, 0, 1, 1));
	    m_vlist[0].Add_back(_vertex_(m_pCurrentTriangle->v[2].vertex, 0, 2, 2));

	    USINT src = 0, des = 1;
        bool  discard = false;
        CLList<_vertex_>::Iterator it;

        for(USINT i=0; i<6; i++)
        {//for all 6 plane
            it = m_vlist[src].Begin();

            while(true)
            {//for each vertex in m_vlist[src]

                _vertex_* va = &(it->data);
                _vertex_* vb = &(it->next->data);
                it = it->next;

                FLOAT Da = va->v.x*m_clipPlaneNormal[i].x+va->v.y*m_clipPlaneNormal[i].y+va->v.z*m_clipPlaneNormal[i].z+m_clipPlaneDistance[i];
                FLOAT Db = vb->v.x*m_clipPlaneNormal[i].x+vb->v.y*m_clipPlaneNormal[i].y+vb->v.z*m_clipPlaneNormal[i].z+m_clipPlaneDistance[i];

                if(Da >= m_clipEpsilon)
                {
                    if(Db >= m_clipEpsilon)
                    {
                        m_vlist[des].Add_back(*vb);
                    }
                    else
                    {
                        FLOAT t = fabs(Da)/(fabs(Da)+fabs(Db));
                        if(t>0)
                        {
                            VERTEX4 lerpVertex = Lerp(va->v, vb->v, t);
                            m_vlist[des].Add_back(_vertex_(lerpVertex, t, va->s, vb->e));
                        }
                    }
                }
                else if(Db >= m_clipEpsilon)
                {
                    FLOAT t = fabs(Da)/(fabs(Da)+fabs(Db));
                    if(t<1)
                    {
                        VERTEX4 lerpVertex = Lerp(va->v, vb->v, t);
                        m_vlist[des].Add_back(_vertex_(lerpVertex, t, va->s, vb->e));
                    }
                    m_vlist[des].Add_back(*vb);
                }

                if(it == m_vlist[src].Begin())
                   break;
            }

            //check if we have vertex output and then swap the destination and source
            if(!m_vlist[des].Empty())
            {
                m_vlist[src].Clear();
                src = !src;
                des = !des;
            }
            else
            {
                discard = true;
                break;
            }

        }

        if(discard)
        {//we need to clip all vertexes
            return;
        }
        if(m_vlist[src].Size()<=3)
        {//do not need to clip any vertex
            OtherTranforms(m_pCurrentTriangle->v[0]);
            OtherTranforms(m_pCurrentTriangle->v[1]);
            OtherTranforms(m_pCurrentTriangle->v[2]);
            SendTriangle(m_pCurrentTriangle->v[0], m_pCurrentTriangle->v[1], m_pCurrentTriangle->v[2]);

            return;
        }
        else
        {//we need to cut the triangle
            it = m_vlist[src].Begin();
            _vertex_* vp = &(it->data);

            VSOutput v1(vp->v, m_pCurrentTriangle->v[vp->s], m_pCurrentTriangle->v[vp->e], vp->t);
            OtherTranforms(v1);
            it = it->next;

            vp = &(it->data);
            VSOutput  v2(vp->v, m_pCurrentTriangle->v[vp->s], m_pCurrentTriangle->v[vp->e], vp->t);
            OtherTranforms(v2);
            it = it->next;

            while(it != m_vlist[src].Begin())
            {
                vp = &(it->data);
                if(vp->t > 0)
                {
                    VSOutput v3(vp->v, m_pCurrentTriangle->v[vp->s], m_pCurrentTriangle->v[vp->e], vp->t);
                    OtherTranforms(v3);

                    SendTriangle(v1, v2, v3);
                    v2 = v3;
                }
                else
                {
                    OtherTranforms(m_pCurrentTriangle->v[vp->s]);
                    SendTriangle(v1, v2, m_pCurrentTriangle->v[vp->s]);

                    v2 = m_pCurrentTriangle->v[vp->s];
                }
                it = it->next;
            }
        }

        return;
	}

	void VertexPostProcessor::OtherTranforms(VSOutput & input)
	{
        //Perspective Divide
        input.vertex.x = input.vertex.x/input.vertex.w;
        input.vertex.y = input.vertex.y/input.vertex.w;
        input.vertex.z = input.vertex.z/input.vertex.w;

        //Viewport Transform
        //FLOAT -> INT
        input.vertex.x = INT(input.vertex.x*  m_viewportWidthHalf   + m_viewportWidthHalf );
        input.vertex.y = INT(input.vertex.y*(-m_viewportHeightHalf) + m_viewportHeightHalf );

        input.vertex.w = 1/fabs(input.vertex.w);

        //viewport :800 : 600
        //CVV      :  1 :   1
        //this may need to be corrected
	}

	void VertexPostProcessor::SendTriangle(VSOutput & v1, VSOutput & v2, VSOutput & v3)
	{
        _Triangle_* out = new _Triangle_(v1, v2, v3);
        if(nullptr == out)
        {
#ifdef _SRE_DEBUG_
            _ERRORLOG(SRE_ERROR_OUTOFMEMORY);
#endif // _SRE_DEBUG_
            return;
        }
        Output((BasicIOElement*)out);
	}

    void VertexPostProcessor::HandleElement()
    {
        std::shared_ptr<BasicIOElement> p = GetInput();
        m_pCurrentTriangle = (_Triangle_*)p.get();
        if(nullptr == m_pCurrentTriangle) {Output(nullptr);return;}
        else
        {
           TriangleClipping();

           m_vlist[0].Clear();
           m_vlist[1].Clear();

        }

        m_pCurrentTriangle = nullptr;
    }

    void VertexPostProcessor::HandleMessage(SREVAR message, USINT id)
    {
#ifdef _SRE_DEBUG_
        g_log.WriteKV("VertexPostProcessor GetMessage From:", id);
#endif // _SRE_DEBUG_
    }

    void VertexPostProcessor::OnCancel()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("VertexPostProcessor Cancel!");
#endif // _SRE_DEBUG_
    }

    void VertexPostProcessor::OnPause()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("VertexPostProcessor Pause!");
#endif // _SRE_DEBUG_
    }

    void VertexPostProcessor::OnResume()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("VertexPostProcessor Resume!");
#endif // _SRE_DEBUG_
    }

    void VertexPostProcessor::OnRunError()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("VertexPostProcessor Run Error!");
#endif // _SRE_DEBUG_
    }

    void VertexPostProcessor::OnRunFinish()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("VertexPostProcessor Run Finish!");
#endif // _SRE_DEBUG_
    }

    void VertexPostProcessor::OnStart()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("VertexPostProcessor Start!");
#endif // _SRE_DEBUG_
    }




    //===========================================
	//Class Rasterizer functions
	//
	//
	//===========================================
	void Rasterizer::HandleElement()
	{
	    std::shared_ptr<BasicIOElement> pTriangle = GetInput();
	    m_pCurrentTriangle = (_Triangle_*)pTriangle.get();
        if(nullptr == m_pCurrentTriangle)
        {
			m_sigCount++;
			_pixelBlock_<BasicIOElement> block(0, 0, 0, 0, false, false);
			if(m_sigCount == 1) block.setArg = true;
			else if(m_sigCount == 2) {block.isEnd = true;m_sigCount = 0;}

			for(USINT i=0; i<m_subProcessorNum; i++)
				  m_subProcessors[i].PushTask(block);
        	return;
        }

        //if cull mode is enable, then test back face and cull
        if(m_pConstantBuffer->CullEnable == SRE_TRUE)
        {
            if(!BackFaceCulling())
            {
                m_pCurrentTriangle = nullptr;
                return;
            }
        }

        //calculate bounding box
        USINT x_min = std::min(m_pCurrentTriangle->v[0].vertex.x, std::min(m_pCurrentTriangle->v[1].vertex.x, m_pCurrentTriangle->v[2].vertex.x));
        USINT x_max = std::max(m_pCurrentTriangle->v[0].vertex.x, std::max(m_pCurrentTriangle->v[1].vertex.x, m_pCurrentTriangle->v[2].vertex.y));
        USINT y_min = std::min(m_pCurrentTriangle->v[0].vertex.y, std::min(m_pCurrentTriangle->v[1].vertex.y, m_pCurrentTriangle->v[2].vertex.x));
        USINT y_max = std::max(m_pCurrentTriangle->v[0].vertex.y, std::max(m_pCurrentTriangle->v[1].vertex.y, m_pCurrentTriangle->v[2].vertex.y));

        //split the bounding box into m_perPixelBlockSize X m_perPixelBlockSize size pixel blocks
        //and put these pixel block to the sub-processors to wait for handle
        USINT startX=x_min, startY=y_min;
        USINT distX, distY;

        std::lock_guard<std::mutex> lock(m_resMutex);
        for(startY=y_min; startY<y_max; startY+=m_perPixelBlockSize)
        {
            distY = distX = m_perPixelBlockSize-1;
            for(startX=x_min; startX<x_max; startX+=m_perPixelBlockSize)
            {
                if((startX+distX) > x_max) {distX = x_max-startX;}
                if((startY+distY) > y_max) {distY = y_max-startY;}

                _pixelBlock_<BasicIOElement> block(startX, startY, distX, distY, false, false, pTriangle);
                m_subProcessors[m_subIndex++].PushTask(block);
                if(m_subIndex == m_subProcessorNum) m_subIndex=0;
            }
        }

	}

	bool Rasterizer::BackFaceCulling()
	{
	    return true;
	}

	void Rasterizer::AddSubProcessors(USINT num)
	{
	    if(m_Started) return;

	    INT id=m_subProcessors.size();
	    while(num>0)
         {
            m_subProcessors.push_back(PixelProcessor(id, &m_subObserver, m_sampleStep, this));
            id++;
            num--;
         }
         m_subProcessorNum = m_subProcessors.size();
	}

	void Rasterizer::RemoveSubProcessors(USINT num)
	{
	    if(m_Started) return;
        while(num>0 && !m_subProcessors.empty())
        {
            m_subProcessors.pop_back();
            num--;
        }
        m_subProcessorNum = m_subProcessors.size();
	}

	void Rasterizer::SetProcessorArgus(USINT id)
     {

        if(nullptr == m_pZbuffer || nullptr == m_pRenderTarget || nullptr == m_pPixelShader || nullptr == m_pConstantBuffer)
		{
#ifdef _SRE_DEBUG_
            g_log.Write("Rasterizer : ZBuffer or RenderTarget or Pixel Shader or ConstantBuffer is null!");
#endif // _SRE_DEBUG_
            this->Cancel();
            return;
		}

        std::lock_guard<std::mutex> lock(m_resMutex);

		m_subProcessors[id].SetSampleStep(m_sampleStep);
		m_subProcessors[id].SetZBuffer(m_pZbuffer);
		m_subProcessors[id].SetRenderTarget(m_pRenderTarget);
		m_subProcessors[id].SetPixelShader(m_pPixelShader);
		m_subProcessors[id].SetConstantBuffer(m_pConstantBuffer);

     }


	void Rasterizer::CancelSubProcessors()
	{
        INT i=0, num=m_subProcessorNum;
        while(i<num)
        {
            m_subProcessors[i++].Cancel();
        }
		i=0;
		while(i<num)
        {
            m_subProcessors[i++].Join();
        }

	}

    void Rasterizer::HandleMessage(SREVAR message, USINT id)
    {
#ifdef _SRE_DEBUG_
        g_log.WriteKV("Rasterizer GetMessage From:", id);
#endif // _SRE_DEBUG_

        if(message == SRE_MESSAGE_ENDDRAW)
		{
			m_finishCount++;
			if(m_finishCount == m_subProcessorNum)
			{
				m_finishCount = 0;
				 m_pObserver->Notify(SRE_MESSAGE_ENDSCENE, m_id);
			}
		}
    }

    void Rasterizer::OnCancel()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("Rasterizer Cancel!");
#endif // _SRE_DEBUG_

    }

    void Rasterizer::OnPause()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("Rasterizer Pause!");
#endif // _SRE_DEBUG_
    }

    void Rasterizer::OnResume()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("Rasterizer Resume!");
#endif // _SRE_DEBUG_
    }

    void Rasterizer::OnRunError()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("Rasterizer Run Error!");
#endif // _SRE_DEBUG_
    }

    void Rasterizer::OnRunFinish()
    {
        CancelSubProcessors();

#ifdef _SRE_DEBUG_
        g_log.Write("Rasterizer Run Finish!");
#endif // _SRE_DEBUG_
    }

    void Rasterizer::OnStart()
    {
        INT index=0;
        while(index < m_subProcessors.size())
            m_subProcessors[index++].Start();

#ifdef _SRE_DEBUG_
        g_log.Write("Rasterizer Start!");
#endif // _SRE_DEBUG_

    }



    //===========================================
	//Class PixelProcessor functions
	//
	//
	//===========================================
	void PixelProcessor::Start()
	{
        if(!m_started)
        {
           m_thread = std::thread(&PixelProcessor::ScanConversion, this);
           m_started = true;
        }
	}

	void PixelProcessor::Cancel()
	{
	    if(m_started)
           m_cancel = true;

		m_cond.notify_one();
	}

	void PixelProcessor::PushTask(_pixelBlock_<BasicIOElement> & task)
	{
        std::lock_guard<std::mutex> lock(m_mutex);
        m_inputQueue.push(task);
        m_cond.notify_one();
	}

	std::shared_ptr<BasicIOElement>
	PixelProcessor::GetTask()
	{
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cond.wait(lock, [this]{return !m_inputQueue.empty()||m_cancel;});
        if(m_cancel) {m_end = true;return  std::shared_ptr<BasicIOElement>();}

        _pixelBlock_<BasicIOElement> task = m_inputQueue.front();
        m_inputQueue.pop();

        m_sx = task.sx;
        m_sy = task.sy;
        m_distX = task.distX;
        m_distY = task.distY;

        if(task.setArg)
		{
			m_sampleStep = m_pParent->m_sampleStep;
			m_pConstantBuffer = m_pParent->m_pConstantBuffer;
			m_pRenderTarget = m_pParent->m_pRenderTarget;
			m_pZbuffer = m_pParent->m_pZbuffer;
			m_pPixelShader = m_pParent->m_pPixelShader;
		}

		if(task.isEnd)
			m_observer->Notify(SRE_MESSAGE_ENDDRAW, m_id);

		m_end = task.isEnd || task.setArg;

        return task.tempData;
	}



	void PixelProcessor::ScanConversion()
	{
        while(!m_cancel)
        {
            std::shared_ptr<BasicIOElement> ptr=GetTask();
            if(m_end){m_end = false;continue;}

            m_pTri = (_Triangle_*)ptr.get();

            FLOAT area = EdgeFunction2D(m_pTri->v[0].vertex, m_pTri->v[2].vertex, m_pTri->v[1].vertex);

            //simplified-EdgeFunction
            //stepX:w0_new - w0 = (b.y-a.y)*s
            //stepY:w0_new - w0 = -(b.x-a.x)*s
            m_wStepX[0] =  (m_pTri->v[1].vertex.y - m_pTri->v[2].vertex.y)*m_sampleStep;
            m_wStepX[1] =  (m_pTri->v[0].vertex.y - m_pTri->v[1].vertex.y)*m_sampleStep;
            m_wStepX[2] =  (m_pTri->v[2].vertex.y - m_pTri->v[0].vertex.y)*m_sampleStep;

            m_wStepY[0] = -(m_pTri->v[1].vertex.x - m_pTri->v[2].vertex.x)*m_sampleStep;
            m_wStepY[1] = -(m_pTri->v[0].vertex.x - m_pTri->v[1].vertex.x)*m_sampleStep;
            m_wStepY[2] = -(m_pTri->v[2].vertex.x - m_pTri->v[0].vertex.x)*m_sampleStep;

            FLOAT w00[3], w01[3], w10[3], w11[3];
            //calculate edgeFunction for 4 corners
            w00[0]=EdgeFunction2D(m_pTri->v[2].vertex.x, m_pTri->v[2].vertex.y, m_pTri->v[1].vertex.x, m_pTri->v[1].vertex.y, m_sx, m_sy);
            w00[1]=EdgeFunction2D(m_pTri->v[1].vertex.x, m_pTri->v[1].vertex.y, m_pTri->v[0].vertex.x, m_pTri->v[0].vertex.y, m_sx, m_sy);
            w00[2]=EdgeFunction2D(m_pTri->v[0].vertex.x, m_pTri->v[0].vertex.y, m_pTri->v[2].vertex.x, m_pTri->v[2].vertex.y, m_sx, m_sy);

            w01[0]=w00[0]+m_distY*(-(m_pTri->v[1].vertex.x - m_pTri->v[2].vertex.x));
            w01[1]=w00[1]+m_distY*(-(m_pTri->v[0].vertex.x - m_pTri->v[1].vertex.x));
            w01[2]=w00[2]+m_distY*(-(m_pTri->v[2].vertex.x - m_pTri->v[0].vertex.x));

            w10[0]=w00[0]+m_distX*(m_pTri->v[1].vertex.y - m_pTri->v[2].vertex.y);
            w10[1]=w00[1]+m_distX*(m_pTri->v[0].vertex.y - m_pTri->v[1].vertex.y);
            w10[2]=w00[2]+m_distX*(m_pTri->v[2].vertex.y - m_pTri->v[0].vertex.y);

            w11[0]=w01[0]+w10[0]-w00[0];
            w11[1]=w01[1]+w10[1]-w00[1];
            w11[2]=w01[2]+w10[2]-w00[2];

            //test 4 corners of the pixel block
            INT all=0;
            all+=(w00[0]>=0 && w00[1]>=0 && w00[2]>=0) ? 1: 0;
            all+=(w01[0]>=0 && w01[1]>=0 && w01[2]>=0) ? 1: 0;
            all+=(w10[0]>=0 && w10[1]>=0 && w10[2]>=0) ? 1: 0;
            all+=(w11[0]>=0 && w11[1]>=0 && w11[2]>=0) ? 1: 0;


            if(all >= 4)
            {//4 corners are all in the triangle
                w00[0]/=area;w00[1]/=area;w00[2]=1-w00[0]-w00[1];
                w01[0]/=area;w01[1]/=area;w01[2]=1-w01[0]-w01[1];
                w10[0]/=area;w10[1]/=area;w10[2]=1-w10[0]-w10[1];
                w11[0]/=area;w11[1]/=area;w11[2]=1-w11[0]-w11[1];

                if(m_pConstantBuffer->ZEnable == SRE_TRUE)
                   Scan_ZOn_IOff(w00, w10, w01, w11);
                else
                   Scan_ZOff_IOff(w00, w10, w01, w11);

            }
			else if(all > 0)
            {//at least 1 corner is in the triangle
                if(m_pConstantBuffer->ZEnable == SRE_TRUE)
                   Scan_ZOn_IOn(w00, area);
                else
                   Scan_ZOff_IOn(w00, area);

            }//no corner is in the triangle
             //skip this pixel block

             //DECOLOR* dev = new DECOLOR[4*4];
             //dev = RenderTargetToDeviceBuffer(dev, m_pRenderTarget->Get(), 4*4);
			 //OutputAsImage(dev, 4, 4);

        }

        m_cancel = false;
        m_started = false;

	}

	void PixelProcessor::Scan_ZOn_IOff(FLOAT W00[3], FLOAT W10[3], FLOAT W01[3], FLOAT W11[3])
	{
	    USINT  sx, sy, ex = m_sx+m_distX, ey = m_sy+m_distY;
	    FLOAT  interpolatedZ;
        bool   drawSquare = m_sampleStep>1 ? true: false;

        PSInput in00(m_pTri->v[0], W00[0], m_pTri->v[1], W00[1], m_pTri->v[2], W00[2]);
        PSInput in01(m_pTri->v[0], W01[0], m_pTri->v[1], W01[1], m_pTri->v[2], W01[2]);
        PSInput in10(m_pTri->v[0], W10[0], m_pTri->v[1], W10[1], m_pTri->v[2], W10[2]);
        PSInput in11(m_pTri->v[0], W11[0], m_pTri->v[1], W11[1], m_pTri->v[2], W11[2]);

        FLOAT factorX = (FLOAT)m_sampleStep/(m_distX+1), factorY = (FLOAT)m_sampleStep/(m_distY+1);
        FLOAT fx, fy;
        for(sy = m_sy, fy = 0; sy <=ey; sy+=m_sampleStep, fy+=factorY)
        {
            for(sx = m_sx, fx = 0; sx <=ex; sx+=m_sampleStep, fx+=factorX)
            {
                interpolatedZ = Lerp(in00.vertex.z, in10.vertex.z, in01.vertex.z, in11.vertex.z, fx, fy);
                if(ZTest(sx, sy, interpolatedZ))
                {
                    PSInput in(in00, in10, in01, in11, fx, fy, interpolatedZ);

                    if(drawSquare)
                        m_pRenderTarget->DrawSquare(sx, sy,
                                                    (sx+m_sampleStep-1)<ex?sx+m_sampleStep-1:ex,
                                                    (sy+m_sampleStep-1)<ey?sy+m_sampleStep-1:ey,
                                                    m_pPixelShader->Run(in));
                    else
                        m_pRenderTarget->Draw(sx, sy, m_pPixelShader->Run(in));
                }
            }
        }

	}

	void PixelProcessor::Scan_ZOff_IOff(FLOAT W00[3], FLOAT W01[3], FLOAT W10[3], FLOAT W11[3])
	{
	    USINT  sx, sy, ex = m_sx+m_distX, ey = m_sy+m_distY;
	    FLOAT  interpolatedZ;
        bool   drawSquare = m_sampleStep>1 ? true: false;

        PSInput in00(m_pTri->v[0], W00[0], m_pTri->v[1], W00[1], m_pTri->v[2], W00[2]);
        PSInput in01(m_pTri->v[0], W01[0], m_pTri->v[1], W01[1], m_pTri->v[2], W01[2]);
        PSInput in10(m_pTri->v[0], W10[0], m_pTri->v[1], W10[1], m_pTri->v[2], W10[2]);
        PSInput in11(m_pTri->v[0], W11[0], m_pTri->v[1], W11[1], m_pTri->v[2], W11[2]);

        FLOAT factorX = (FLOAT)m_sampleStep/(m_distX+1), factorY = (FLOAT)m_sampleStep/(m_distY+1);
        FLOAT fx, fy;
        for(sy = m_sy, fy = 0; sy <= ey; sy+=m_sampleStep, fy+=factorY)
        {
            for(sx = m_sx, fx = 0; sx <= ex; sx+=m_sampleStep, fx+=factorX)
            {
                PSInput in(in00, in10, in01, in11, fx, fy, interpolatedZ);

                if(drawSquare)
                    m_pRenderTarget->DrawSquare(sx, sy,
                                                (sx+m_sampleStep-1)<ex?sx+m_sampleStep-1:ex,
                                                (sy+m_sampleStep-1)<ey?sy+m_sampleStep-1:ey,
                                                m_pPixelShader->Run(in));
                else
                    m_pRenderTarget->Draw(sx, sy, m_pPixelShader->Run(in));

            }
        }
	}

	void PixelProcessor::Scan_ZOn_IOn(FLOAT W00[3], FLOAT wTri)
	{
	    SINT  sy=m_sy, sx=m_sx;
        SINT  step=m_sampleStep;
        FLOAT w0=W00[0], w1=W00[1], w2=W00[2];
        VEC2  Barycentric;
        FLOAT interpolatedZ;
        bool  drawSquare = m_sampleStep>1? true: false;

        FLOAT sty0= m_wStepY[0], sty1= m_wStepY[1], sty2= m_wStepY[2];

        for(SINT distY = m_distY; ;sy+=m_sampleStep, distY-=m_sampleStep)
        {
            for(SINT distX = m_distX; ;sx+=step, distX-=m_sampleStep)
            {
                if(w0>=0 && w1>=0 && w2>=0)
                {
                    Barycentric.x=w0/wTri;
                    Barycentric.y=w1/wTri;

                    interpolatedZ=Bicp(m_pTri->v[0].vertex.z, Barycentric.x,
												  m_pTri->v[1].vertex.z, Barycentric.y,
                                                  m_pTri->v[2].vertex.z);

                    if(ZTest(sx, sy, interpolatedZ))
                    {

                       PSInput in(m_pTri->v[0], Barycentric.x,
                                       m_pTri->v[1], Barycentric.y,
                                       m_pTri->v[2], 1-Barycentric.x-Barycentric.y,
                                       interpolatedZ);

                       if(drawSquare)
                           m_pRenderTarget->DrawSquare(sx, sy,
                                                       distX>m_sampleStep?sx+m_sampleStep-1:m_sx+m_distX,
                                                       distY>m_sampleStep?sy+m_sampleStep-1:m_sy+m_distY,
                                                       m_pPixelShader->Run(in));
                       else
                           m_pRenderTarget->Draw(sx, sy, m_pPixelShader->Run(in));
                    }
                }
				if( distX <= 0)
				{
					m_wStepX[0]=-m_wStepX[0];
					m_wStepX[1]=-m_wStepX[1];
					m_wStepX[2]=-m_wStepX[2];
					break;
				}
                w0 = w0 + m_wStepX[0];
                w1 = w1 + m_wStepX[1];
                w2 = w2 + m_wStepX[2];

            }
            step = -step;
            if(distY <= 0) break;

            w0 = w0 + m_wStepY[0];
            w1 = w1 + m_wStepY[1];
            w2 = w2 + m_wStepY[2];

        }

	}

    void PixelProcessor::Scan_ZOff_IOn(FLOAT W00[3], FLOAT wTri)
	{
	     SINT  sy=m_sy, sx=m_sx;
         SINT  step=m_sampleStep;
         FLOAT w0=W00[0], w1=W00[1], w2=W00[2];
         VEC2  Barycentric;
         FLOAT interpolatedZ;
         bool  drawSquare = m_sampleStep>1 ? true: false;

        for(SINT distY = m_distY; ;sy+=m_sampleStep, distY-=m_sampleStep)
        {
            for(SINT distX = m_distX; ;sx+=step, distX-=m_sampleStep)
            {
                if(w0>=0 && w1>=0 && w2>=0)
                {
                    Barycentric.x=w0/wTri;
                    Barycentric.y=w1/wTri;

                    interpolatedZ=Bicp(m_pTri->v[0].vertex.z, Barycentric.x,
                                                  m_pTri->v[1].vertex.z, Barycentric.y,
                                                  m_pTri->v[2].vertex.z);

                    PSInput in(m_pTri->v[0], Barycentric.x,
                                    m_pTri->v[1], Barycentric.y,
                                    m_pTri->v[2], 1-Barycentric.x-Barycentric.y,
                                    interpolatedZ);

                    if(drawSquare)
                        m_pRenderTarget->DrawSquare(sx, sy,
                                                    distX>m_sampleStep?sx+m_sampleStep-1:m_sx+m_distX,
                                                    distY>m_sampleStep?sy+m_sampleStep-1:m_sy+m_distY,
                                                    m_pPixelShader->Run(in));
                    else
                        m_pRenderTarget->Draw(sx, sy, m_pPixelShader->Run(in));

                }
				if( distX <= 0)
				{
					m_wStepX[0]=-m_wStepX[0];
					m_wStepX[1]=-m_wStepX[1];
					m_wStepX[2]=-m_wStepX[2];
					break;
				}

                w0 = w0 + m_wStepX[0];
                w1 = w1 + m_wStepX[1];
                w2 = w2 + m_wStepX[2];

            }
            step = -step;
            if(distY <= 0) break;

            w0 = w0 + m_wStepY[0];
            w1 = w1 + m_wStepY[1];
            w2 = w2 + m_wStepY[2];

        }

	}




    //===========================================
	//Class OutputMerger functions
	//*Unused*
	//
	//===========================================
	/*******************************************************************
    void OutputMerger::HandleElement()
    {
        std::shared_ptr<BasicIOElement> ptr=GetInput();
        _target_* t=(_target_*)ptr.get();

		if(nullptr == t)
		{
			return;
		}

		m_pCurTarget=t->texture;
        if(m_toFrame)
		{
			//注意framebuffer size 与 target size 不一致的问题


			RTCOLOR* frame = m_pCurTarget->Get();
			INT size = m_pCurTarget->GetWidth()*m_pCurTarget->GetHeight();
	        for(int i=0; i<size; i++)
          	{
				m_pFrame[i].b=frame[i].b;
		        m_pFrame[i].g=frame[i].g;
		        m_pFrame[i].r=frame[i].r;
			}

			//OutputAsImage(m_pFrame, m_pCurTarget->GetWidth(), m_pCurTarget->GetHeight());
			m_pObserver->Notify(SRE_MESSAGE_ENDSCENE, m_id);
		}
    }

    void OutputMerger::HandleMessage(SREVAR message, USINT id)
    {
#ifdef _SRE_DEBUG_
        g_log.WriteKV("OutputMerger GetMessage From:", id);
#endif // _SRE_DEBUG_
    }

    void OutputMerger::OnCancel()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("OutputMerger Cancel!");
#endif // _SRE_DEBUG_
    }

    void OutputMerger::OnPause()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("OutputMerger Pause!");
#endif // _SRE_DEBUG_
    }

    void OutputMerger::OnResume()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("OutputMerger Resume!");
#endif // _SRE_DEBUG_
    }

    void OutputMerger::OnRunError()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("OutputMerger Run Error!");
#endif // _SRE_DEBUG_
    }

    void OutputMerger::OnRunFinish()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("OutputMerger Run Finish!");
#endif // _SRE_DEBUG_
    }

    void OutputMerger::OnStart()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("OutputMerger Start!");
#endif // _SRE_DEBUG_
    }
    ******************************************************************/








}
