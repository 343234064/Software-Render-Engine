//*****************************************************
//
// Software Render Engine
// Version 0.01 by XJL
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
           //m_thread.detach();
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
    void InputAssembler::Run()
    {
#ifdef _SRE_DEBUG_
        if(nullptr == m_pConstantBuffer)
        {
           m_pObserver->Notify(SRE_MESSAGE_RUNERROR, m_id);
           return;
        }
#endif

        if(nullptr == m_pCurrentHandleVbuffer)
        {
            if(m_vertexBuffers.empty())
            {
               m_pObserver->Notify(SRE_MESSAGE_END, m_id);
               return;
            }

            m_pCurrentHandleVbuffer = m_vertexBuffers.front();
            m_vertexBuffers.pop();

            m_pCurrentHandleIbuffer = m_indexBuffers.front();
            m_indexBuffers.pop();
        }


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
            if(nullptr != m_pCurrentHandleIbuffer && !m_pCurrentHandleIbuffer->Empty())
            {
                if(actualIndex3 >= m_pCurrentHandleIbuffer->GetBufferSize())
                {
                    m_pCurrentHandleVbuffer = nullptr;
                    m_pObserver->Notify(SRE_MESSAGE_END, m_id);
                    return;
                }
                actualIndex1 = m_pCurrentHandleIbuffer->GetData(actualIndex1);
                actualIndex2 = m_pCurrentHandleIbuffer->GetData(actualIndex2);
                actualIndex3 = m_pCurrentHandleIbuffer->GetData(actualIndex3);

                if(actualIndex1 >= vertexNum || actualIndex1 < 0)
                {m_pCurrentHandleVbuffer = nullptr;
                 m_pObserver->Notify(SRE_MESSAGE_END, m_id);
                 return;}
                if(actualIndex2 >= vertexNum || actualIndex2 < 0)
                {m_pCurrentHandleVbuffer = nullptr;
                 m_pObserver->Notify(SRE_MESSAGE_END, m_id);
                 return;}
                if(actualIndex3 >= vertexNum || actualIndex3 < 0)
                {m_pCurrentHandleVbuffer = nullptr;
                 m_pObserver->Notify(SRE_MESSAGE_END, m_id);
                 return;}

            }
            else if(actualIndex3 >= m_pCurrentHandleVbuffer->GetVertexNumber())
            {
                m_pCurrentHandleVbuffer = nullptr;
                m_pObserver->Notify(SRE_MESSAGE_END, m_id);
                return;
            }

            SendVertex(actualIndex1, actualIndex2, actualIndex3);


    }

    void InputAssembler::SendVertex(INT actualIndex1, INT actualIndex2, INT actualIndex3)
    {
        BYTE* vertex1=m_pCurrentHandleVbuffer->GetVertexAddr(actualIndex1);//This is not safe

        BYTE* vertex2=m_pCurrentHandleVbuffer->GetVertexAddr(actualIndex2);

        BYTE* vertex3=m_pCurrentHandleVbuffer->GetVertexAddr(actualIndex3);

        m_pNextStage->SetInput(vertex1, actualIndex1);
        m_pNextStage->SetInput(vertex2, actualIndex2);
        m_pNextStage->SetInput(vertex3, actualIndex3);

    }

    void InputAssembler::Reset()
    {
       m_currentIndex   = 0;
       m_currentIndex2 = 0;
    }

    void InputAssembler::ClearVertexbufferAndIndexbuffer()
    {
        INT num = m_vertexBuffers.size();
        while(num>0)
        {
           m_vertexBuffers.pop();
           num--;
        }

        num = m_indexBuffers.size();
        while(num>0)
        {
           m_indexBuffers.pop();
           num--;
        }

    }

    void InputAssembler::SetInput(void* data, INT index)
    {

    }


    //===========================================
	//Class VertexProcessor functions
	//
	//
	//===========================================
    void VertexProcessor::Run()
    {
#ifdef _SRE_DEBUG_
        if(nullptr == m_pVertexShader)
        {
           m_pObserver->Notify(SRE_MESSAGE_RUNERROR, m_id);
           return;
        }
#endif

        if(m_inputvertexid[0]==-1 || m_inputvertexid[1]==-1 || m_inputvertexid[2]==-1)
        {
           m_pObserver->Notify(SRE_MESSAGE_END, m_id);
           return;
        }

        VSOutput* vout1 = SendToVS(m_inputvertex[0], m_inputvertexid[0]);
        VSOutput* vout2 = SendToVS(m_inputvertex[1], m_inputvertexid[1]);
        VSOutput* vout3 = SendToVS(m_inputvertex[2], m_inputvertexid[2]);

        _Triangle_ * triangle = new _Triangle_(vout1, vout2, vout3);
        if(nullptr == triangle)
        {
#ifdef _SRE_DEBUG_
            _ERRORLOG(SRE_ERROR_OUTOFMEMORY);
#endif // _SRE_DEBUG_
            delete vout1;
            delete vout2;
            delete vout3;
            m_pObserver->Notify(SRE_MESSAGE_RUNERROR, m_id);
        }
        else
           m_pNextStage->SetInput(triangle, 0);

        delete vout1;
        delete vout2;
        delete vout3;
        m_inputvertexid[0]=-1;
        m_inputvertexid[1]=-1;
        m_inputvertexid[2]=-1;

    }

    VSOutput* VertexProcessor::SendToVS(BYTE* vertex, USINT index)
    {
        bool  found = false;
        USINT coverIndex = 0;
        USINT maxpriority = 8;
        VSOutput * out = nullptr;
        for(USINT i=0; i<3; i++)
        {
            if(index == m_cachedVertexIndex[i])
            {
                if(!found)
                {
                   out = new VSOutput(m_cachedVertex[i]);
                   if(nullptr == out)
                   {
#ifdef _SRE_DEBUG_
                      _ERRORLOG(SRE_ERROR_OUTOFMEMORY);
#endif // _SRE_DEBUG_
                     m_pObserver->Notify(SRE_MESSAGE_RUNERROR, m_id);
                   }
                }

                found = true;
                m_cachedPriority[i]=1;
                continue;
            }
            else
            {
                m_cachedPriority[i]=m_cachedPriority[i]<<1;
            }

            if(m_cachedPriority[i]>=maxpriority)
            {
                coverIndex = i;
                maxpriority = m_cachedPriority[i];
            }

        }

        if(!found)
        {
            out = m_pVertexShader->Run(vertex, m_pVariableBuffer);
            if(nullptr != out)
            {
                m_cachedVertex[coverIndex] = *out;
                m_cachedVertexIndex[coverIndex] = index;
                m_cachedPriority[coverIndex] = 1;
            }
        }

        return out;
    }

    void VertexProcessor::Reset()
    {
       m_cachedVertexIndex[0]=-1;
       m_cachedVertexIndex[1]=-1;
       m_cachedVertexIndex[2]=-1;
       m_cachedPriority[0]=4;
       m_cachedPriority[1]=2;
       m_cachedPriority[2]=1;
       m_inputvertexid[0]=-1;
       m_inputvertexid[1]=-1;
       m_inputvertexid[2]=-1;
       m_curinput=0;
    }

    void VertexProcessor::SetInput(void* data, INT index)
    {
       m_inputvertex[m_curinput] = (BYTE*)data;
       m_inputvertexid[m_curinput] = index;
       m_curinput++;

       if(m_curinput == 3)
          m_curinput = 0;
    }

   //===========================================
	//Class VertexPostProcessor functions
	//
   //
   //I use Sutherland and Hodgman Clipping Algorithm
	//===========================================
	void VertexPostProcessor::TriangleClipping()
	{
	     _Triangle_* ptriangle = m_pCurrentTriangle;

        if(ptriangle->v[0].vertex.w <= 0) return;
	     if(ptriangle->v[1].vertex.w <= 0) return;
	     if(ptriangle->v[2].vertex.w <= 0) return;

	     m_vlist[0].Add_back(_vertex_(ptriangle->v[0].vertex, 0, 0, 0));
	     m_vlist[0].Add_back(_vertex_(ptriangle->v[1].vertex, 0, 1, 1));
	     m_vlist[0].Add_back(_vertex_(ptriangle->v[2].vertex, 0, 2, 2));

        bool  discard = false;
        bool  cut = false;
	     USINT src = 0, des = 1;
        CLList<_vertex_>::Iterator it;

        for(USINT i=0; i<6; i++)
        {//for all 6 plane
            it = m_vlist[src].Begin();

            while(true)
            {//for each vertex in m_vlist[src]
                _vertex_* va = &(it->data);
                _vertex_* vb = &(it->next->data);

                _vertex_ a= *va;
                _vertex_ b= *vb;

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
                            cut = true;
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
                        cut = true;
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

        if(!cut)
        {//do not need to clip any vertex
            OtherTranforms(ptriangle->v[0]);
            OtherTranforms(ptriangle->v[1]);
            OtherTranforms(ptriangle->v[2]);
            SendTriangle(ptriangle->v[0], ptriangle->v[1], ptriangle->v[2]);
            return;
        }
        else
        {//we need to cut the triangle
            it = m_vlist[src].Begin();
            _vertex_* vp = &(it->data);

            VSOutput v1(vp->v, ptriangle->v[vp->s], ptriangle->v[vp->e], vp->t);
            OtherTranforms(v1);
            it = it->next;

            vp = &(it->data);
            VSOutput  v2(vp->v, ptriangle->v[vp->s], ptriangle->v[vp->e], vp->t);
            OtherTranforms(v2);
            it = it->next;

            while(it != m_vlist[src].Begin())
            {
                vp = &(it->data);
                if(vp->t > 0)
                {
                    VSOutput v3(vp->v, ptriangle->v[vp->s], ptriangle->v[vp->e], vp->t);
                    OtherTranforms(v3);

                    SendTriangle(v1, v2, v3);
                    v2 = v3;
                }
                else
                {
                    OtherTranforms(ptriangle->v[vp->s]);
                    SendTriangle(v1, v2, ptriangle->v[vp->s]);

                    v2 = ptriangle->v[vp->s];
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
        //Notice that : FLOAT -> INT
        input.vertex.x = INT(input.vertex.x*  m_viewportWidthHalf   + m_viewportWidthHalf );
        input.vertex.y = INT(input.vertex.y*(-m_viewportHeightHalf) + m_viewportHeightHalf );

        input.vertex.w = 1/fabs(input.vertex.w);

        //Notice that:
        //viewport :800 : 600
        //CVV      :  1 :   1

	}

	void VertexPostProcessor::SendTriangle(VSOutput & v1, VSOutput & v2, VSOutput & v3)
	{
        _Triangle_* tri = new _Triangle_(v1, v2, v3);

        if(nullptr == tri)
        {
#ifdef _SRE_DEBUG_
            _ERRORLOG(SRE_ERROR_OUTOFMEMORY);
#endif // _SRE_DEBUG_
            m_pObserver->Notify(SRE_MESSAGE_RUNERROR, m_id);
            return;
        }

        m_pObserver->Notify(SRE_MESSAGE_SENDDATA, m_id, (void*)tri);
	}

    void VertexPostProcessor::Run()
    {
#ifdef _SRE_DEBUG_
        if(nullptr == m_pConstantBuffer)
        {
           m_pObserver->Notify(SRE_MESSAGE_RUNERROR, m_id);
           return;
        }
#endif

        if(nullptr == m_pCurrentTriangle)
        {
           m_pObserver->Notify(SRE_MESSAGE_END, m_id);
           return;
        }

        if(m_pConstantBuffer->ClipEnable == SRE_TRUE)
        {
            TriangleClipping();
            m_vlist[0].Clear();
            m_vlist[1].Clear();
        }
        else
        {
           OtherTranforms(m_pCurrentTriangle->v[0]);
           OtherTranforms(m_pCurrentTriangle->v[1]);
           OtherTranforms(m_pCurrentTriangle->v[2]);

           SendTriangle(m_pCurrentTriangle->v[0], m_pCurrentTriangle->v[1], m_pCurrentTriangle->v[2]);
        }

        delete m_pCurrentTriangle;
        m_pCurrentTriangle = nullptr;

    }

    void VertexPostProcessor::Reset()
    {
         m_vlist[0].Clear();
         m_vlist[1].Clear();
         m_pCurrentTriangle = nullptr;
    }


    void VertexPostProcessor::SetInput(void* data, INT index)
    {
         m_pCurrentTriangle = (_Triangle_*)data;
    }




   //===========================================
	//Class GeometryProcessor functions
	//
	//
	//===========================================
	 void GeometryProcessor::HandleElement()
    {
       if(m_valve)
       {
          m_ia.Run();
          if(m_valve)
          {
             m_vp.Run();
             m_vpp.Run();
          }
       }
       else
       {
          std::unique_lock<std::mutex> lock(m_resMutex);
          m_cond.wait(lock, [this]{return m_valve || m_Cancel;});

          m_ia.Reset();
          m_vp.Reset();

       }

    }

    void GeometryProcessor::HandleMessage(SREVAR message, USINT id, void* data)
    {
      if(message == SRE_MESSAGE_END)
		{
		     m_valve = false;
		     Output(nullptr);
           {
#ifdef _SRE_DEBUG_
        g_log.Write("GeometryProcessor GetEnd:");
        if(id == 1)
           g_log.Write("Input Assembler Run END!");
        else if(id == 2)
           g_log.Write("Vertex Processor Run END!");
        else if(id == 3)
           g_log.Write("Vertex Post Processor Run END!");
#endif // _SRE_DEBUG_

           }
		}
		else if(message == SRE_MESSAGE_SENDDATA)
      {
         BasicIOElement* out = (BasicIOElement*)data;
         Output(out);
      }
		else if(message == SRE_MESSAGE_RUNERROR)
      {
#ifdef _SRE_DEBUG_
        g_log.Write("GeometryProcessor GetMessage:");
        if(id == 1)
           g_log.Write("Input Assembler Run Error!");
        else if(id == 2)
           g_log.Write("Vertex Processor Run Error!");
        else if(id == 3)
           g_log.Write("Vertex Post Processor Run Error!");
#endif // _SRE_DEBUG_

         m_valve = false;
         m_pObserver->Notify(SRE_MESSAGE_RUNERROR, m_id);
      }
    }

    void GeometryProcessor::BeginScene()
    {
       m_valve = false;
    }

    void GeometryProcessor::EndScene()
    {
       m_valve = true;
       m_cond.notify_one();
    }

    void GeometryProcessor::OnCancel()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("GeometryProcessor Cancel!");
#endif // _SRE_DEBUG_
        m_ia.ClearVertexbufferAndIndexbuffer();
        m_ia.Reset();
        m_vp.Reset();
        m_vpp.Reset();
    }

    void GeometryProcessor::OnPause()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("GeometryProcessor Pause!");
#endif // _SRE_DEBUG_
    }

    void GeometryProcessor::OnResume()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("GeometryProcessor Resume!");
#endif // _SRE_DEBUG_
    }

    void GeometryProcessor::OnRunError()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("GeometryProcessor Run Error!");
#endif // _SRE_DEBUG_
    }

    void GeometryProcessor::OnRunFinish()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("GeometryProcessor Run Finish!");
#endif // _SRE_DEBUG_
    }

    void GeometryProcessor::OnStart()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("GeometryProcessor Start!");
#endif // _SRE_DEBUG_

    }




   //===========================================
	//Class Rasterizer functions
	//
	//
	//===========================================
	void Rasterizer::HandleElement()
	{
	     std::shared_ptr<BasicIOElement> ptr = GetInput();
	     m_pCurrentTriangle = (_Triangle_*)ptr.get();

	     if(nullptr == m_pCurrentTriangle)
        {
            _pixelBlock_<BasicIOElement> block(0, 0, 0, 0, 0.0f, false);
            block.isEnd = true;

			   for(USINT i=0; i<m_subProcessorNum; i++)
				     m_subProcessors[i].PushTask(block);

        	   return;
        }

        //g_log.WriteKV("RZ get tri v1:", m_pCurrentTriangle->v[0].vertex.x, m_pCurrentTriangle->v[0].vertex.y, m_pCurrentTriangle->v[0].vertex.z);
        //g_log.WriteKV("RZ get tri v2:", m_pCurrentTriangle->v[1].vertex.x, m_pCurrentTriangle->v[1].vertex.y, m_pCurrentTriangle->v[1].vertex.z);
        //g_log.WriteKV("RZ get tri v3:", m_pCurrentTriangle->v[2].vertex.x, m_pCurrentTriangle->v[2].vertex.y, m_pCurrentTriangle->v[2].vertex.z);


        //if cull mode is enable, then test back face
        if(m_pConstantBuffer->CullEnable == SRE_TRUE)
        {
            if(!BackFaceCulling())
            {
                return;
            }
        }

        FLOAT area = EdgeFunction2D(m_pCurrentTriangle->v[0].vertex, m_pCurrentTriangle->v[2].vertex, m_pCurrentTriangle->v[1].vertex);
        //calculate bounding box
        USINT x_min = std::min(m_pCurrentTriangle->v[0].vertex.x, std::min(m_pCurrentTriangle->v[1].vertex.x, m_pCurrentTriangle->v[2].vertex.x));
        USINT x_max = std::max(m_pCurrentTriangle->v[0].vertex.x, std::max(m_pCurrentTriangle->v[1].vertex.x, m_pCurrentTriangle->v[2].vertex.x));
        USINT y_min = std::min(m_pCurrentTriangle->v[0].vertex.y, std::min(m_pCurrentTriangle->v[1].vertex.y, m_pCurrentTriangle->v[2].vertex.y));
        USINT y_max = std::max(m_pCurrentTriangle->v[0].vertex.y, std::max(m_pCurrentTriangle->v[1].vertex.y, m_pCurrentTriangle->v[2].vertex.y));

        //split the bounding box into m_perPixelBlockSize X m_perPixelBlockSize size pixel blocks
        //and put these pixel block to the sub-processors to wait for handle
        USINT startX=x_min, startY=y_min;
        USINT distX, distY;

        for(startY=y_min; startY<y_max; startY+=m_perPixelBlockSize)
        {
            distY = distX = m_perPixelBlockSize-1;
            for(startX=x_min; startX<x_max; startX+=m_perPixelBlockSize)
            {
                if((startX+distX) > x_max) {distX = x_max-startX;}
                if((startY+distY) > y_max) {distY = y_max-startY;}

                _pixelBlock_<BasicIOElement> block(startX, startY, distX, distY, area, false, ptr);
                m_subProcessors[m_subIndex++].PushTask(block);
                if(m_subIndex == m_subProcessorNum) m_subIndex=0;
            }
        }

	}

	//On screen space
	bool Rasterizer::BackFaceCulling()
	{
	   FLOAT v1x = m_pCurrentTriangle->v[1].vertex.x-m_pCurrentTriangle->v[0].vertex.x;
      FLOAT v1y = m_pCurrentTriangle->v[1].vertex.y-m_pCurrentTriangle->v[0].vertex.y;
      FLOAT v2x = m_pCurrentTriangle->v[2].vertex.x-m_pCurrentTriangle->v[0].vertex.x;
      FLOAT v2y = m_pCurrentTriangle->v[2].vertex.y-m_pCurrentTriangle->v[0].vertex.y;

      if(m_pConstantBuffer->CullMode == SRE_CULLMODE_CCW)
         return (v1x*v2y - v1y*v2x)>0? true: false;
      else if(m_pConstantBuffer->CullMode == SRE_CULLMODE_CW)
         return (v1y*v2x - v1x*v2y)>0? true: false;
      else
         return false;
	}

	void Rasterizer::AddSubProcessors(USINT num)
	{
	    if(m_Started) return;

	    INT id=m_subProcessors.size();
	    while(num>0)
       {
          m_subProcessors.push_back(PixelProcessor(id, &m_subObserver, m_sampleStep));
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

        if(nullptr == m_pRenderTarget || nullptr == m_pPixelShader || nullptr == m_pConstantBuffer)
		  {
#ifdef _SRE_DEBUG_
            if(nullptr == m_pZbuffer)
               g_log.Write("Rasterizer :  Zbuffer is null");

            g_log.Write("Rasterizer :  RenderTarget or Pixel Shader or ConstantBuffer is null!");
#endif // _SRE_DEBUG_
            m_pObserver->Notify(SRE_MESSAGE_RUNERROR, m_id);
            return;
		  }

		m_subProcessors[id].SetSampleStep(m_sampleStep);
		m_subProcessors[id].SetZBuffer(m_pZbuffer);
		m_subProcessors[id].SetRenderTarget(m_pRenderTarget);
		m_subProcessors[id].SetPixelShader(m_pPixelShader);
		m_subProcessors[id].SetConstantBuffer(m_pConstantBuffer);

   }

   void Rasterizer::SetAllProcessorArgus()
   {
      if(nullptr == m_pRenderTarget || nullptr == m_pPixelShader || nullptr == m_pConstantBuffer)
      {
#ifdef _SRE_DEBUG_
            if(nullptr == m_pZbuffer)
               g_log.Write("Rasterizer :  Zbuffer is null");

            g_log.Write("Rasterizer :  RenderTarget or Pixel Shader or ConstantBuffer is null!");
#endif // _SRE_DEBUG_
            m_pObserver->Notify(SRE_MESSAGE_RUNERROR, m_id);
            return;
      }

      for(USINT id=0; id<m_subProcessorNum; id++)
		{
		   m_subProcessors[id].SetSampleStep(m_sampleStep);
	   	m_subProcessors[id].SetZBuffer(m_pZbuffer);
		   m_subProcessors[id].SetRenderTarget(m_pRenderTarget);
	   	m_subProcessors[id].SetPixelShader(m_pPixelShader);
		   m_subProcessors[id].SetConstantBuffer(m_pConstantBuffer);
		}
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

    void Rasterizer::HandleMessage(SREVAR message, USINT id, void* data)
    {
        if(message == SRE_MESSAGE_ENDDRAW)
		  {
			   m_finishCount++;
			   if(m_finishCount == m_subProcessorNum)
			   {
                m_finishCount = 0;
                m_pObserver->Notify(SRE_MESSAGE_END, m_id);
			   }
		  }
		  else if(message == SRE_MESSAGE_RUNERROR)
        {
#ifdef _SRE_DEBUG_
             g_log.WriteKV("Pixel Processor Run Error:", id);
#endif // _SRE_DEBUG_
             m_pObserver->Notify(SRE_MESSAGE_RUNERROR, m_id);
        }
    }

    void Rasterizer::OnCancel()
    {
#ifdef _SRE_DEBUG_
        g_log.Write("Rasterizer Cancel!");
#endif // _SRE_DEBUG_
        m_subIndex = 0;
        m_finishCount = 0;
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
        USINT index=0;
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
           //m_thread.detach();
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
	PixelProcessor::GetTask(FLOAT& triArea)
	{
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cond.wait(lock, [this]{return !m_inputQueue.empty()||m_cancel;});
        if(m_cancel) {m_end = true;return  std::shared_ptr<BasicIOElement>();}

        _pixelBlock_<BasicIOElement> task = m_inputQueue.front();
        m_inputQueue.pop();

        if(task.isEnd)
		  {
		     m_observer->Notify(SRE_MESSAGE_ENDDRAW, m_id);
		  }
        else
        {
           m_sx = task.sx;
           m_sy = task.sy;
           m_distX = task.distX;
           m_distY = task.distY;
           triArea = task.area;
        }


        m_end = task.isEnd;

        return task.tempData;
	}



	void PixelProcessor::ScanConversion()
	{
        while(!m_cancel)
        {
            FLOAT area;
            std::shared_ptr<BasicIOElement> ptr=GetTask(area);
            if(m_end){m_end = false;continue;}

#ifdef _SRE_DEBUG_
        if(nullptr == m_pConstantBuffer || nullptr == m_pPixelShader ||
           nullptr == m_pRenderTarget || nullptr == m_pZbuffer)
        {
            m_observer->Notify(SRE_MESSAGE_RUNERROR, m_id);
            return;
        }

#endif // _SRE_DEBUG_

            m_pTri = (_Triangle_*)ptr.get();

            //simplified-EdgeFunction
            //stepX:w0_new - w0 = (b.y-a.y)*s
            //stepY:w0_new - w0 = -(b.x-a.x)*s
            USINT id1 = 1, id2 =2;
            if(m_pConstantBuffer->CullMode == SRE_CULLMODE_CW) {id1=2;id2=1;}

            m_wStepX[0] =  (m_pTri->v[id1].vertex.y - m_pTri->v[id2].vertex.y)*m_sampleStep;
            m_wStepX[1] =  (m_pTri->v[0].vertex.y    - m_pTri->v[id1].vertex.y)*m_sampleStep;
            m_wStepX[2] =  (m_pTri->v[id2].vertex.y - m_pTri->v[0].vertex.y)*m_sampleStep;

            m_wStepY[0] = -(m_pTri->v[id1].vertex.x - m_pTri->v[id2].vertex.x)*m_sampleStep;
            m_wStepY[1] = -(m_pTri->v[0].vertex.x    - m_pTri->v[id1].vertex.x)*m_sampleStep;
            m_wStepY[2] = -(m_pTri->v[id2].vertex.x - m_pTri->v[0].vertex.x)*m_sampleStep;

            FLOAT w00[3], w01[3], w10[3], w11[3];
            //calculate edgeFunction for 4 corners
            w00[0]=EdgeFunction2D(m_pTri->v[id2].vertex.x, m_pTri->v[id2].vertex.y, m_pTri->v[id1].vertex.x, m_pTri->v[id1].vertex.y, m_sx, m_sy);
            w00[1]=EdgeFunction2D(m_pTri->v[id1].vertex.x, m_pTri->v[id1].vertex.y, m_pTri->v[0].vertex.x, m_pTri->v[0].vertex.y, m_sx, m_sy);
            w00[2]=EdgeFunction2D(m_pTri->v[0].vertex.x, m_pTri->v[0].vertex.y, m_pTri->v[id2].vertex.x, m_pTri->v[id2].vertex.y, m_sx, m_sy);

            w01[0]=w00[0]+m_distY*(-(m_pTri->v[id1].vertex.x - m_pTri->v[id2].vertex.x));
            w01[1]=w00[1]+m_distY*(-(m_pTri->v[0].vertex.x    - m_pTri->v[id1].vertex.x));
            w01[2]=w00[2]+m_distY*(-(m_pTri->v[id2].vertex.x - m_pTri->v[0].vertex.x));

            w10[0]=w00[0]+m_distX*(m_pTri->v[id1].vertex.y - m_pTri->v[id2].vertex.y);
            w10[1]=w00[1]+m_distX*(m_pTri->v[0].vertex.y    - m_pTri->v[id1].vertex.y);
            w10[2]=w00[2]+m_distX*(m_pTri->v[id2].vertex.y - m_pTri->v[0].vertex.y);

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
			   else //if(all > 0)
            {//at least 1 corner is in the triangle
                if(m_pConstantBuffer->ZEnable == SRE_TRUE)
                   Scan_ZOn_IOn(w00, area);
                else
                   Scan_ZOff_IOn(w00, area);

            }
        }

        ClearTask();
        m_cancel = false;
        m_started = false;

	}

	void PixelProcessor::Scan_ZOn_IOff(FLOAT W00[3], FLOAT W10[3], FLOAT W01[3], FLOAT W11[3])
	{
	     USINT  sx, sy, ex = m_sx+m_distX, ey = m_sy+m_distY;
	     FLOAT interpolatedZ;
        bool   drawSquare = m_sampleStep>1 ? true: false;
        INT      targetWidth =  m_pRenderTarget->GetWidth();
        INT      deltaY=m_sampleStep*targetWidth, offsetY=m_sy*targetWidth;
        INT      drawpos;

        PSInput in00(m_pTri->v[0], W00[0], m_pTri->v[1], W00[1], m_pTri->v[2], W00[2]);
        PSInput in01(m_pTri->v[0], W01[0], m_pTri->v[1], W01[1], m_pTri->v[2], W01[2]);
        PSInput in10(m_pTri->v[0], W10[0], m_pTri->v[1], W10[1], m_pTri->v[2], W10[2]);
        PSInput in11(m_pTri->v[0], W11[0], m_pTri->v[1], W11[1], m_pTri->v[2], W11[2]);

        FLOAT factorX = (FLOAT)m_sampleStep/(m_distX+1), factorY = (FLOAT)m_sampleStep/(m_distY+1);
        FLOAT fx, fy;
        for(sy = m_sy, fy = 0; sy <=ey; sy+=m_sampleStep, fy+=factorY, offsetY += deltaY)
        {
            for(sx = m_sx, fx = 0; sx <=ex; sx+=m_sampleStep, fx+=factorX)
            {
                drawpos = offsetY + sx;
                interpolatedZ = Lerp(in00.vertex.z, in10.vertex.z, in01.vertex.z, in11.vertex.z, fx, fy);
                if(interpolatedZ < m_pZbuffer->GetData(drawpos))
                {
                    PSInput in(in00, in01, in10, in11, fx, fy, interpolatedZ);

                    if(drawSquare)
                    {
                       USINT _ex = (sx+m_sampleStep-1)>ex ? ex : sx+m_sampleStep-1;
                       USINT _ey  = (sy+m_sampleStep-1)>ey ? ey : sy + m_sampleStep-1;
                       m_pZbuffer->SetDataSquare(sx, sy, _ex, _ey, interpolatedZ);
                       m_pRenderTarget->DrawSquare(sx, sy,
                                                     _ex,
                                                     _ey,
                                                    m_pPixelShader->Run(in));

                    }
                    else
                     {
                        m_pZbuffer->SetData(drawpos, interpolatedZ);
                        m_pRenderTarget->Draw(drawpos, m_pPixelShader->Run(in));
                     }
                }
            }
        }

	}

	void PixelProcessor::Scan_ZOff_IOff(FLOAT W00[3], FLOAT W01[3], FLOAT W10[3], FLOAT W11[3])
	{
	     USINT  sx, sy, ex = m_sx+m_distX, ey = m_sy+m_distY;
        bool   drawSquare = m_sampleStep>1 ? true: false;
        INT      targetWidth =  m_pRenderTarget->GetWidth();
        INT      deltaY=m_sampleStep*targetWidth, offsetY=m_sy*targetWidth;

        PSInput in00(m_pTri->v[0], W00[0], m_pTri->v[1], W00[1], m_pTri->v[2], W00[2]);
        PSInput in01(m_pTri->v[0], W01[0], m_pTri->v[1], W01[1], m_pTri->v[2], W01[2]);
        PSInput in10(m_pTri->v[0], W10[0], m_pTri->v[1], W10[1], m_pTri->v[2], W10[2]);
        PSInput in11(m_pTri->v[0], W11[0], m_pTri->v[1], W11[1], m_pTri->v[2], W11[2]);

        FLOAT factorX = (FLOAT)m_sampleStep/(m_distX+1), factorY = (FLOAT)m_sampleStep/(m_distY+1);
        FLOAT fx, fy;
        for(sy = m_sy, fy = 0; sy <= ey; sy+=m_sampleStep, fy+=factorY, offsetY += deltaY)
        {
            for(sx = m_sx, fx = 0; sx <= ex; sx+=m_sampleStep, fx+=factorX)
            {
                PSInput in(in00, in01, in10, in11, fx, fy);

                if(drawSquare)
                    m_pRenderTarget->DrawSquare(sx, sy,
                                                   (sx+m_sampleStep-1)>ex ? ex : sx+m_sampleStep-1,
                                                   (sy+m_sampleStep-1)>ey ? ey : sy + m_sampleStep-1,
                                                    m_pPixelShader->Run(in));
                else
                    m_pRenderTarget->Draw(offsetY + sx, m_pPixelShader->Run(in));

            }
        }
	}

	void PixelProcessor::Scan_ZOn_IOn(FLOAT W00[3], FLOAT wTri)
	{
	     INT       sy=m_sy, sx=m_sx, ex = m_sx + m_distX, ey = m_sy + m_distY;
        INT       step=m_sampleStep;
        FLOAT  w0=W00[0], w1=W00[1], w2=W00[2];
        VEC2    Barycentric;
        FLOAT  interpolatedZ;
        bool    drawSquare = m_sampleStep>1? true: false;

        INT      targetWidth =  m_pRenderTarget->GetWidth();
        INT      deltaY = m_sampleStep*targetWidth, offsetY = m_sy*targetWidth;
        INT      drawpos = 0;

        for(INT distY = m_distY; ;sy+=m_sampleStep, distY-=m_sampleStep, offsetY += deltaY)
        {
            for(INT distX = m_distX; ;sx+=step, distX-=m_sampleStep)
            {
                if(w0>=0 && w1>=0 && w2>=0)
                {
                    drawpos = offsetY + sx;
                    Barycentric.x=w0/wTri;
                    Barycentric.y=w1/wTri;

                    interpolatedZ=Bicp(m_pTri->v[0].vertex.z, Barycentric.x,
												              m_pTri->v[1].vertex.z, Barycentric.y,
                                                  m_pTri->v[2].vertex.z);

                    if(interpolatedZ < m_pZbuffer->GetData(drawpos))
                    {

                       PSInput in(m_pTri->v[0], Barycentric.x,
                                       m_pTri->v[1], Barycentric.y,
                                       m_pTri->v[2], 1-Barycentric.x-Barycentric.y,
                                       interpolatedZ);

                       if(drawSquare)
                       {
                          USINT _ex = (sx+m_sampleStep-1)>ex ? ex : sx+m_sampleStep-1;
                          USINT _ey  = (sy+m_sampleStep-1)>ey ? ey : sy + m_sampleStep-1;
                          m_pZbuffer->SetDataSquare(sx, sy, _ex, _ey, interpolatedZ);
                          m_pRenderTarget->DrawSquare(sx, sy,
                                                       _ex,
                                                       _ey,
                                                       m_pPixelShader->Run(in));
                       }
                       else
                       {
                           m_pZbuffer->SetData(drawpos, interpolatedZ);
                           m_pRenderTarget->Draw(drawpos, m_pPixelShader->Run(in));
                       }
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
	      INT     sy=m_sy, sx=m_sx, ex = m_sx + m_distX, ey = m_sy + m_distY;
         INT     step=m_sampleStep;
         FLOAT w0=W00[0], w1=W00[1], w2=W00[2];
         VEC2  Barycentric;
         bool  drawSquare = m_sampleStep>1 ? true: false;

        INT      targetWidth =  m_pRenderTarget->GetWidth();
        INT      deltaY = m_sampleStep*targetWidth, offsetY = m_sy*targetWidth;

        for(INT distY = m_distY; ;sy+=m_sampleStep, distY-=m_sampleStep, offsetY += deltaY)
        {
            for(INT distX = m_distX; ;sx+=step, distX-=m_sampleStep)
            {
                if(w0>=0 && w1>=0 && w2>=0)
                {
                    Barycentric.x=w0/wTri;
                    Barycentric.y=w1/wTri;

                    PSInput in(m_pTri->v[0], Barycentric.x,
                                    m_pTri->v[1], Barycentric.y,
                                    m_pTri->v[2], 1-Barycentric.x-Barycentric.y);

                    if(drawSquare)
                        m_pRenderTarget->DrawSquare(sx, sy,
                                                       (sx+m_sampleStep-1)>ex ? ex : sx+m_sampleStep-1,
                                                       (sy+m_sampleStep-1)>ey ? ey : sy + m_sampleStep-1,
                                                       m_pPixelShader->Run(in));
                    else
                        m_pRenderTarget->Draw(offsetY + sx, m_pPixelShader->Run(in));

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

   void PixelProcessor::ClearTask()
   {
        std::lock_guard<std::mutex> lock(m_mutex);
        INT num = m_inputQueue.size();
        while(num>0)
        {
           m_inputQueue.pop();
           num--;
        }
   }




   //===========================================
	//Class SREPipeLine functions
	//
	//
	//===========================================
   RESULT SREPipeLine::Init(USINT frameWidth, USINT frameHeight, bool createZbuffer)
   {
      if(frameWidth==0 || frameHeight==0) return RESULT::INVALIDARG;

      if(createZbuffer)
      {
         bool re = m_zbuffer.ResetBuffer(nullptr, frameWidth, frameHeight);
         if(!re) return RESULT::FAIL;
      }

      m_GP.SetObserver(&m_observer);
      m_GP.SetOutputQueue(&m_GPoutputBuffer);

      m_RZ.SetObserver(&m_observer);
      m_RZ.SetInputQueue(&m_GPoutputBuffer);

      m_GP.SetConstantBuffer(&constbuffer);
      m_RZ.SetConstantBuffer(&constbuffer);

      m_GP.SetViewportWidth(frameWidth);
      m_GP.SetViewportHeight(frameHeight);

      m_RZ.AddSubProcessors(1);
      m_RZ.SetSamplePixelBlockSize(200);
      m_RZ.SetSampleStep(3);

      if(createZbuffer)
         m_RZ.SetZbuffer(&m_zbuffer);

      return RESULT::SUCC;

   }

   void SREPipeLine::Run()
   {
      if(nullptr == m_pExternObserver)
      {
#ifdef _SRE_DEBUG_
         g_log.Write("SREPipeLine Extern Observer is null !");
#endif // _SRE_DEBUG_
         return;
      }

      if(!m_started)
      {
         OnStart();

         m_GP.Start();
         m_RZ.Start();

         m_started = true;
      }
   }

   void SREPipeLine::Pause()
   {
      if(m_started)
      {
         OnPause();

         m_GP.Pause();
         m_RZ.Pause();

         m_waited = true;
      }
   }

   void SREPipeLine::Cancel()
   {
      if(m_started)
      {
          OnCancel();

          m_GP.Cancel();
          m_RZ.Cancel();

          m_RZ.Join();
          m_GP.Join();

          OnRunFinish();

          m_started = false;
          m_waited = false;
      }
   }

   void SREPipeLine::Resume()
   {
      if(m_waited)
      {
          OnResume();

          m_GP.Resume();
          m_RZ.Resume();

          m_waited = false;
      }
   }

   void SREPipeLine::OnCancel()
   {

   }

   void SREPipeLine::OnPause(){}

   void SREPipeLine::OnResume(){}

   void SREPipeLine::OnRunError(){}

   void SREPipeLine::OnRunFinish()
   {
      m_GPoutputBuffer.Clear();
   }

   void SREPipeLine::OnStart(){}

   void SREPipeLine::HandleMessage(SREVAR message, USINT id, void* data)
   {
       if(message == SRE_MESSAGE_END)
       {
          m_pExternObserver->Notify(SRE_MESSAGE_END, id);
       }
       else if(message == SRE_MESSAGE_RUNERROR)
       {
#ifdef _SRE_DEBUG_
          if(id == 1)
             g_log.Write("GeometryProcessor Run Error!");
          else if(id == 2)
             g_log.Write("Rasterizer Run Error!");
#endif // _SRE_DEBUG_

          m_pExternObserver->Notify(SRE_MESSAGE_RUNERROR, 0);
          Cancel();
       }
   }

   void SREPipeLine::BeginScene()
   {
      m_GP.BeginScene();
   }

   void SREPipeLine::EndScene()
   {
      m_RZ.SetAllProcessorArgus();
      m_GP.EndScene();
   }

   void SREPipeLine::SetSubProcessors(USINT num)
   {
      if(m_started) return;

      if(m_RZ.GetSubProcessorNum() > num)
         m_RZ.RemoveSubProcessors(m_RZ.GetSubProcessorNum()-num);
      else if(m_RZ.GetSubProcessorNum() < num)
         m_RZ.AddSubProcessors(num-m_RZ.GetSubProcessorNum());
   }


   RESULT SREPipeLine::ResetFrameSize(USINT width, USINT height)
   {
         if(width==0 || height==0) return RESULT::INVALIDARG;

         m_GP.SetViewportWidth(width);
         m_GP.SetViewportHeight(height);

         if(m_zbuffer.GetBufferSize()>=0)
         {
            if(m_zbuffer.ResetBuffer(nullptr, width, height))
               return RESULT::SUCC;
            else
               return RESULT::FAIL;
         }
         else
            return RESULT::SUCC;
   }

   void SREPipeLine::ResetZbuffer(FLOAT maxDepth)
   {
      m_zbuffer.ResetData(maxDepth);
   }



}
