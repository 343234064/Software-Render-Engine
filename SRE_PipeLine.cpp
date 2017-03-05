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
        }
    }


    void BasicProcessor::Run()
    {
        while(true)
        {
            if(m_Cancel)
            {
                g_log.Write("m_cancel true");
                if(nullptr != m_callBacks)
                   m_callBacks->OnCancel();
                break;
            }

            if(m_Pause)
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                if(nullptr != m_callBacks)
                   m_callBacks->OnPause();
                m_cond.wait(lock);
            }


            try
            {

                if(nullptr != m_callBacks)
                   m_callBacks->HandleElement();

            }
            catch(...)
            {
                if(nullptr != m_callBacks)
                   m_callBacks->OnRunError();
                if(nullptr != m_pObserver)
                   m_pObserver->Notify(SRE_MESSAGE_RUNERROR);
                throw;
            }

        }

        if(nullptr != m_callBacks)
            m_callBacks->OnRunFinish();

        m_Started = false;
        return;
    }

    void BasicProcessor::Pause()
    {
        m_Pause = true;
    }

    void BasicProcessor::Cancel()
    {
        m_Cancel = true;
    }

    void BasicProcessor::Resume()
    {
        m_Pause = false;
        if(nullptr != m_callBacks)
           m_callBacks->OnResume();
        m_cond.notify_one();

    }

    BasicIOElement* BasicProcessor::GetInput()
    {
        BasicIOElement * element;
        m_pInputQueue->wait_and_pop(element);

        return element;
    }

    void BasicProcessor::Output(BasicIOElement* out)
    {
        m_pOutputQueue->push(out);
    }




    //===========================================
	//Class InputAssembler functions
	//
	//
	//===========================================
    void InputAssembler::SetVertexAndIndexBuffers(VertexBuffer* vertexBuffer, Buffer<INT>* indexBuffer)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_vertexBuffers.push(vertexBuffer);
        m_indexBuffers.push(indexBuffer);

        m_cond.notify_one();
    }

    void InputAssembler::SetConstantBuffer(const ConstantBuffer * cbuffer)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_pConstantBuffer = cbuffer;
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

            g_log.Write("InputAssem Output:");

            SendVertex(actualIndex1);
            SendVertex(actualIndex2);
            SendVertex(actualIndex3);
        }
        else
        {
            m_vertexBuffers.wait_and_pop(m_pCurrentHandleVbuffer);
            m_pCurrentHandleIbuffer = m_indexBuffers.front();
            m_indexBuffers.pop();
            m_currentIndex = 0;
            m_pCurrentHandleVbuffer->ResetMark(false);
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
        g_log.WriteKV("vertex index", actualIndex);
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
    void VertexProcessor::SetVertexShader(VertexShader * vshader)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_pVertexShader = vshader;
    }

    void VertexProcessor::SetVariableBuffer(VariableBuffer* varbuffer)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_pVariableBuffer = varbuffer;
    }

    void VertexProcessor::HandleElement()
    {
        _index_ver_* v = (_index_ver_*)(GetInput());
        g_log.Write("Vertex Process:");
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
                g_log.WriteKV("Found", v->index);
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
                g_log.WriteKV("Not Found, replace", v->index, (INT)coverIndex);
            }

        }

        delete v;
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
        if(nullptr == m_pVertexShader) this->Cancel();

#ifdef _SRE_DEBUG_
        if(nullptr == m_pVariableBuffer)
           g_log.Write("VertexProcessor->variable buffer is null");

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
	    VSOutput* v1=(VSOutput*)GetInput();
	    VSOutput* v2=(VSOutput*)GetInput();
	    VSOutput* v3=(VSOutput*)GetInput();

        if(nullptr != v1 && nullptr != v2 && nullptr != v3)
        {
            _Triangle_* out = new _Triangle_(*v1, *v2, *v3);

            if(nullptr != out)
            {
                 Output((BasicIOElement*)out);
                 g_log.Write("Primitive Output:");
                 g_log.WriteKV("Triangle:", v1->vertex.x, v2->vertex.x, v3->vertex.x);
            }
            else
            {
#ifdef _SRE_DEBUG_
               _ERRORLOG(SRE_ERROR_OUTOFMEMORY);
#endif // _SRE_DEBUG_
            }
        }

        delete v1;
        delete v2;
        delete v3;
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
    void VertexPostProcessor::SetClipPlaneX(FLOAT Distance_PlaneToOrigin, VEC3& normal)
    {
        m_clipPlaneDistance[0] = Distance_PlaneToOrigin;
        m_clipPlaneDistance[1] = Distance_PlaneToOrigin;

        m_clipPlaneNormal[0] = normal;
        m_clipPlaneNormal[1] = -normal;
    }

    void VertexPostProcessor::SetClipPlaneY(FLOAT Distance_PlaneToOrigin, VEC3& normal)
    {
        m_clipPlaneDistance[2] = Distance_PlaneToOrigin;
        m_clipPlaneDistance[3] = Distance_PlaneToOrigin;

        m_clipPlaneNormal[2] = normal;
        m_clipPlaneNormal[3] = -normal;
    }

    void VertexPostProcessor::SetClipPlaneZ(FLOAT Distance_PlaneToOrigin, VEC3& normal)
    {
        m_clipPlaneDistance[4] = Distance_PlaneToOrigin;
        m_clipPlaneDistance[5] = Distance_PlaneToOrigin;

        m_clipPlaneNormal[4] = normal;
        m_clipPlaneNormal[5] = -normal;
    }

	void VertexPostProcessor::TriangleClipping()
	{
	    if(m_pCurrentTriangle->v[0].vertex.w <= 0) return;
	    if(m_pCurrentTriangle->v[1].vertex.w <= 0) return;
	    if(m_pCurrentTriangle->v[2].vertex.w <= 0) return;

	    m_vlist[0].push_back(_vertex_(m_pCurrentTriangle->v[0].vertex, 0, 0, 0));
	    m_vlist[0].push_back(_vertex_(m_pCurrentTriangle->v[1].vertex, 0, 1, 1));
	    m_vlist[0].push_back(_vertex_(m_pCurrentTriangle->v[2].vertex, 0, 2, 2));

	    USINT src = 0, des = 1;
        bool  discard = false;
        std::list<_vertex_>::iterator it,it2;

        for(USINT i=0; i<6; i++)
        {//for all 6 plane
            it=m_vlist[src].begin();

            while(true)
            {//for each vertex in m_vlist[src]

                _vertex_ va = *it++;
                if(it == m_vlist[src].end())
                   it++;
                _vertex_ vb = *it;

                FLOAT Da = va.v.x*m_clipPlaneNormal[i].x+va.v.y*m_clipPlaneNormal[i].y+va.v.z*m_clipPlaneNormal[i].z+m_clipPlaneDistance[i];
                FLOAT Db = vb.v.x*m_clipPlaneNormal[i].x+vb.v.y*m_clipPlaneNormal[i].y+vb.v.z*m_clipPlaneNormal[i].z+m_clipPlaneDistance[i];

                if(Da >= m_clipEpsilon)
                {
                    if(Db >= m_clipEpsilon)
                    {
                        m_vlist[des].push_back(vb);
                    }
                    else
                    {
                        FLOAT t = fabs(Da)/(fabs(Da)+fabs(Db));
                        if(t>0)
                        {
                            VERTEX4 lerpVertex = Lerp(va.v, vb.v, t);
                            m_vlist[des].push_back(_vertex_(lerpVertex, t, va.s, vb.e));
                        }
                    }
                }
                else if(Db >= m_clipEpsilon)
                {
                    FLOAT t = fabs(Da)/(fabs(Da)+fabs(Db));
                    if(t<1)
                    {
                        VERTEX4 lerpVertex = Lerp(va.v, vb.v, t);
                        m_vlist[des].push_back(_vertex_(lerpVertex, t, va.s, vb.e));
                    }
                    m_vlist[des].push_back(vb);
                }

                if(it == m_vlist[src].begin())
                   break;
            }

            //check if we have vertex output and then swap the destination and source
            if(!m_vlist[des].empty())
            {
                m_vlist[src].clear();
                src = !src;
                des = !des;
            }
            else
            {
                discard = true;
                break;
            }

        }

         g_log.Write("Clipping Output");
         it=m_vlist[src].begin();
         while(it!=m_vlist[src].end())
         {
            _vertex_ v = *it++;
            g_log.WriteKV("VLIST:",v.v.x, v.v.y, v.v.z, v.v.w);
         }


        if(discard)
        {//we need to clip all vertexes

            g_log.WriteKV("CTri v1:", m_pCurrentTriangle->v[0].vertex.x, m_pCurrentTriangle->v[0].vertex.y, m_pCurrentTriangle->v[0].vertex.z);
            g_log.WriteKV("CTri v2:", m_pCurrentTriangle->v[1].vertex.x, m_pCurrentTriangle->v[1].vertex.y, m_pCurrentTriangle->v[1].vertex.z);
            g_log.WriteKV("CTri v3:", m_pCurrentTriangle->v[2].vertex.x, m_pCurrentTriangle->v[2].vertex.y, m_pCurrentTriangle->v[2].vertex.z);
            g_log.Write("Do Clip All");
            return;
        }
        if(m_vlist[src].size()<=3)
        {//do not need to clip any vertex
            OtherTranforms(m_pCurrentTriangle->v[0]);
            OtherTranforms(m_pCurrentTriangle->v[1]);
            OtherTranforms(m_pCurrentTriangle->v[2]);
            SendTriangle(m_pCurrentTriangle->v[0], m_pCurrentTriangle->v[1], m_pCurrentTriangle->v[2]);

            g_log.WriteKV("CTri v1:", m_pCurrentTriangle->v[0].vertex.x, m_pCurrentTriangle->v[0].vertex.y, m_pCurrentTriangle->v[0].vertex.z);
            g_log.WriteKV("CTri v2:", m_pCurrentTriangle->v[1].vertex.x, m_pCurrentTriangle->v[1].vertex.y, m_pCurrentTriangle->v[1].vertex.z);
            g_log.WriteKV("CTri v3:", m_pCurrentTriangle->v[2].vertex.x, m_pCurrentTriangle->v[2].vertex.y, m_pCurrentTriangle->v[2].vertex.z);
            g_log.Write("Do Not Clip");
            return;
        }
        else
        {//we need to cut the triangle
            g_log.Write("Do Clipped ");
            it = m_vlist[src].begin();

            VSOutput v1(it->v);
            v1.LerpAttriValue(m_pCurrentTriangle->v[it->s], m_pCurrentTriangle->v[it->e], it->t);
            OtherTranforms(v1);
            it++;

            VSOutput  v2(it->v);
            v2.LerpAttriValue(m_pCurrentTriangle->v[it->s], m_pCurrentTriangle->v[it->e], it->t);
            OtherTranforms(v2);
            it++;

            while(it != m_vlist[src].end())
            {

                if(it->t > 0)
                {
                    VSOutput v3(it->v);
                    v3.LerpAttriValue(m_pCurrentTriangle->v[it->s], m_pCurrentTriangle->v[it->e], it->t);
                    OtherTranforms(v3);

                    SendTriangle(v1, v2, v3);


            g_log.WriteKV("CTri v1:", v1.vertex.x, v1.vertex.y, v1.vertex.z);
            g_log.WriteKV("CTri v2:", v2.vertex.x, v2.vertex.y, v2.vertex.z);
            g_log.WriteKV("CTri v3:", v3.vertex.x, v3.vertex.y, v3.vertex.z);
            g_log.Write("Clipped");
                    v2 = v3;
                }
                else
                {
                    OtherTranforms(m_pCurrentTriangle->v[it->s]);
                    SendTriangle(v1, v2, m_pCurrentTriangle->v[it->s]);


            g_log.WriteKV("CTri v1:", v1.vertex.x, v1.vertex.y, v1.vertex.z);
            g_log.WriteKV("CTri v2:", v2.vertex.x, v2.vertex.y, v2.vertex.z);
            g_log.WriteKV("CTri v3:", m_pCurrentTriangle->v[it->s].vertex.x, m_pCurrentTriangle->v[it->s].vertex.y, m_pCurrentTriangle->v[it->s].vertex.z);
            g_log.Write("Clipped");

                    v2 = m_pCurrentTriangle->v[it->s];
                }
                it++;
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
        input.vertex.x = input.vertex.x*  m_viewportWidthHalf   + m_viewportWidthHalf;
        input.vertex.y = input.vertex.y*(-m_viewportHeightHalf) + m_viewportHeightHalf;

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
        m_pCurrentTriangle = (_Triangle_*)(GetInput());
        g_log.Write("HANDLE ELEMENT");
        if(nullptr != m_pCurrentTriangle)
        {
           TriangleClipping();

           m_vlist[0].clear();
           m_vlist[1].clear();

        }

        g_log.Write("HANDLE ELEMENT END");
        delete m_pCurrentTriangle;
        m_pCurrentTriangle = nullptr;
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
	//Class Technique functions
	//
	//
	//===========================================
    /*
    Technique & Technique::operator=(const Technique & other)
    {
        if(this != &other)
        {
            this->m_name = other.m_name;
            this->m_PassList = other.m_PassList;
        }

        return *this;
    }*/


    void Technique::ReleasePassList()
    {
        std::list<RenderPass*>::iterator it;
        RenderPass * pass;
        for(it=m_PassList.begin(); it!=m_PassList.end();)
        {
            pass = (*it);
            delete pass;
            m_PassList.erase(it++);
        }
    }


    void Technique::SetName(std::string name)
    {
        m_name = name;
    }


    std::string Technique::GetName()
    {
        return m_name;
    }


    INT Technique::GetRenderPassNumber()
    {
        return m_PassList.size();
    }


    void Technique::AddRenderPass(RenderPass * renderPass, INT index)
    {
        if(nullptr == renderPass)
        {
#ifdef _SRE_DEBUG_
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
#endif
            return;
        }
        std::list<RenderPass*>::iterator it;
        INT i=0;
        for(it=m_PassList.begin(); it!=m_PassList.end(); it++, i++)
        {
            if(i==index)
            {
                m_PassList.insert(it, renderPass);
                break;
            }
        }
    }


    void Technique::AddRenderPassBack(RenderPass * renderPass)
    {
        if(nullptr == renderPass)
        {
#ifdef _SRE_DEBUG_
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
#endif
            return;
        }

        m_PassList.push_back(renderPass);
    }


    void Technique::RemoveRenderPassByIndex(INT index)
    {
        std::list<RenderPass*>::iterator it;
        INT i=0;
        for(it=m_PassList.begin(); it!=m_PassList.end(); it++, i++)
        {
            if(i==index)
            {
                it=m_PassList.erase(it);
                break;
            }
        }
    }


    void Technique::RemoveRenderPassByName(std::string name)
    {
        std::list<RenderPass*>::iterator it;
        for(it=m_PassList.begin(); it!=m_PassList.end(); it++)
        {
            if(name==(*it)->GetName())
            {
                it=m_PassList.erase(it);
                break;
            }
        }
    }


    RenderPass * Technique::GetRenderPassByIndex(INT index)
    {
        std::list<RenderPass*>::iterator it;
        INT i=0;
        for(it=m_PassList.begin(); it!=m_PassList.end(); it++, i++)
        {
            if(i==index)
            {
                return (*it);
            }
        }

        return nullptr;

    }


    RenderPass * Technique::GetRenderPassByName(std::string name)
    {
        std::list<RenderPass*>::iterator it;
        for(it=m_PassList.begin(); it!=m_PassList.end(); it++)
        {
            if(name==(*it)->GetName())
            {
                return (*it);
            }
        }

        return nullptr;
    }


    void Technique::Run()
    {
        std::list<RenderPass*>::iterator it;
        for(it=m_PassList.begin(); it!=m_PassList.end(); it++)
        {
            (*it)->Run();
        }

    }







    //===========================================
	//Class RenderPass functions
	//
	//
	//===========================================
    std::string RenderPass::GetName()
    {
        return this->m_name;
    }











}
