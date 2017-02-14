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
        if(nullptr == this->m_pInputQueue ||
           nullptr == this->m_pOutputQueue ||
           nullptr == this->m_pObserver)
           return;

        if(nullptr != m_callBacks)
            m_callBacks->OnStart();
        m_thread.StartThread(&BasicProcessor::Run, this);

    }


    void BasicProcessor::Run()
    {
        while(true)
        {
            if(m_Cancel)
            {
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
                m_pObserver->Notify(SRE_MESSAGE_RUNERROR);
                throw;
            }

        }

        if(nullptr != m_callBacks)
            m_callBacks->OnRunFinish();

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
                    Output(nullptr);
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
                Output(nullptr);
                return;
            }

            if(!(m_pCurrentHandleVbuffer->GetMark(actualIndex1)))
            {
                BYTE* vertex1=m_pCurrentHandleVbuffer->GetVertex(actualIndex1);
                if(nullptr == vertex1)
                {
#ifdef _SRE_DEBUG_
                   _ERRORLOG(SRE_ERROR_OUTOFMEMORY);
#endif // _SRE_DEBUG_
                   return;
                }
                Output((BasicIOElement*)vertex1);
                m_pCurrentHandleVbuffer->SetMark(actualIndex1, true);
            }
            if(!(m_pCurrentHandleVbuffer->GetMark(actualIndex2)))
            {
                BYTE* vertex2=m_pCurrentHandleVbuffer->GetVertex(actualIndex2);
                if(nullptr == vertex2)
                {
#ifdef _SRE_DEBUG_
                   _ERRORLOG(SRE_ERROR_OUTOFMEMORY);
#endif // _SRE_DEBUG_
                   return;
                }
                Output((BasicIOElement*)vertex2);
                m_pCurrentHandleVbuffer->SetMark(actualIndex2, true);
            }
            if(!(m_pCurrentHandleVbuffer->GetMark(actualIndex3)))
            {
                BYTE* vertex3=m_pCurrentHandleVbuffer->GetVertex(actualIndex3);
                if(nullptr == vertex3)
                {
#ifdef _SRE_DEBUG_
                   _ERRORLOG(SRE_ERROR_OUTOFMEMORY);
#endif // _SRE_DEBUG_
                   return;
                }
                Output((BasicIOElement*)vertex3);
                m_pCurrentHandleVbuffer->SetMark(actualIndex3, true);
            }

        }
        else
        {
            m_vertexBuffers.wait_and_pop(m_pCurrentHandleVbuffer);
            m_pCurrentHandleIbuffer = m_indexBuffers.front();
            m_indexBuffers.pop();
            m_currentIndex = 0;
        }
    }

    void InputAssembler::OnCancel()
    {}

    void InputAssembler::OnPause()
    {}

    void InputAssembler::OnResume()
    {}

    void InputAssembler::OnRunError()
    {}

    void InputAssembler::OnRunFinish()
    {}

    void InputAssembler::OnStart()
    {
        m_pCurrentHandleVbuffer->ResetMark(false);
    }



    //===========================================
	//Class VertexProcessor functions
	//
	//
	//===========================================
    void VertexProcesser::SetVertexShader(VertexShader * vshader)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_pVertexShader = vshader;
    }

    void VertexProcesser::SetVariableBuffer(VariableBuffer* varbuffer)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_pVariableBuffer = varbuffer;
    }

    void VertexProcesser::HandleElement()
    {
        BYTE* vertex = (BYTE*)(GetInput());
        if(nullptr == vertex)
        {
            Output(nullptr);
            return;
        }
        else
        {
            VSOutput * out = m_pVertexShader->Run(vertex);
            if(nullptr != out)
                Output((BasicIOElement*)out);
        }
        delete vertex;
    }

    void VertexProcesser::OnCancel()
    {}

    void VertexProcesser::OnPause()
    {}

    void VertexProcesser::OnResume()
    {}

    void VertexProcesser::OnRunError()
    {}

    void VertexProcesser::OnRunFinish()
    {}

    void VertexProcesser::OnStart()
    {
        if(nullptr == m_pVertexShader) this->Cancel();
        if(nullptr == m_pVariableBuffer) this->Cancel();
    }





    //===========================================
	//Class PrimitiveAssembler functions
	//
	//
	//===========================================
    void PrimitiveAssembler::SetConstantBuffer(const ConstantBuffer * cbuffer)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_pConstantBuffer = cbuffer;
    }

	void PrimitiveAssembler::TriangleList()
	{
	    VSOutput* v1=(VSOutput*)GetInput();
	    VSOutput* v2=(VSOutput*)GetInput();
	    VSOutput* v3=(VSOutput*)GetInput();

        if(nullptr == v1 || nullptr == v2 || nullptr == v3)
            return;

        _Triangle_* out = new _Triangle_(*v1, *v2, *v3);
        delete v1;
        delete v2;
        delete v3;

        if(nullptr == out)
        {
#ifdef _SRE_DEBUG_
                   _ERRORLOG(SRE_ERROR_OUTOFMEMORY);
#endif // _SRE_DEBUG_
                   return;
        }
        Output((BasicIOElement*)out);
	}

	void PrimitiveAssembler::TriangleFan()
	{
	    VSOutput* v3 = nullptr;
	    if(nullptr == m_pCachedVertex1)
        {
            m_pCachedVertex1 = (BYTE*)(GetInput());
            m_pCachedVertex2 = (BYTE*)(GetInput());
        }
        else
        {
            v3 = (VSOutput*)(GetInput());
            if(nullptr == v3)
            {
                delete m_pCachedVertex1;
                delete m_pCachedVertex2;
                m_pCachedVertex1 = nullptr;
                return;
            }
        }

        _Triangle_* out = new _Triangle_(*((VSOutput*)m_pCachedVertex1), *((VSOutput*)m_pCachedVertex2), *v3);
        if(nullptr == out)
        {
#ifdef _SRE_DEBUG_
                   _ERRORLOG(SRE_ERROR_OUTOFMEMORY);
#endif // _SRE_DEBUG_
                   return;
        }
        Output((BasicIOElement*)out);

        delete m_pCachedVertex2;
        m_pCachedVertex2 = (BYTE*)v3;
	}

	void PrimitiveAssembler::TriangleStrip()
	{
	    VSOutput* v3 = nullptr;
	    if(nullptr == m_pCachedVertex1)
        {
            m_pCachedVertex1 = (BYTE*)GetInput();
            m_pCachedVertex2 = (BYTE*)GetInput();
        }
        else
        {
            v3 = (VSOutput*)GetInput();
            if(nullptr == v3)
            {
                delete m_pCachedVertex1;
                delete m_pCachedVertex2;
                m_pCachedVertex1 = nullptr;
                return;
            }
        }

        _Triangle_* out = new _Triangle_(*((VSOutput*)m_pCachedVertex1), *((VSOutput*)m_pCachedVertex2), *v3);
        if(nullptr == out)
        {
#ifdef _SRE_DEBUG_
                   _ERRORLOG(SRE_ERROR_OUTOFMEMORY);
#endif // _SRE_DEBUG_
                   return;
        }
        Output((BasicIOElement*)out);

        delete m_pCachedVertex1;
        m_pCachedVertex1 = m_pCachedVertex2;
        m_pCachedVertex2 = (BYTE*)v3;
	}

    void PrimitiveAssembler::HandleElement()
    {
        if(m_pConstantBuffer->primitiveTopology == SRE_PRIMITIVETYPE_TRIANGLELIST)
        {
            TriangleList();
        }
        else if(m_pConstantBuffer->primitiveTopology == SRE_PRIMITIVETYPE_TRIANGLEFAN)
        {
            TriangleFan();
        }
        else if(m_pConstantBuffer->primitiveTopology == SRE_PRIMITIVETYPE_TRIANGLESTRIP)
        {
            TriangleStrip();
        }

    }

    void PrimitiveAssembler::OnCancel()
    {}

    void PrimitiveAssembler::OnPause()
    {}

    void PrimitiveAssembler::OnResume()
    {}

    void PrimitiveAssembler::OnRunError()
    {}

    void PrimitiveAssembler::OnRunFinish()
    {}

    void PrimitiveAssembler::OnStart()
    {}



    //===========================================
	//Class VertexPostProcesser functions
	//
    //
    //I use Sutherland and Hodgman Clipping Algorithm
	//===========================================
	bool VertexPostProcesser::TriangleClipping()
	{
	    if(m_pCurrentTriangle->v[0].vertex.w <= 0) return false;
	    if(m_pCurrentTriangle->v[1].vertex.w <= 0) return false;
	    if(m_pCurrentTriangle->v[2].vertex.w <= 0) return false;

	    m_vlist[0].push_back(_vertex_(m_pCurrentTriangle->v[0].vertex, -1, 0, 0));
	    m_vlist[0].push_back(_vertex_(m_pCurrentTriangle->v[1].vertex, -1, 1, 1));
	    m_vlist[0].push_back(_vertex_(m_pCurrentTriangle->v[2].vertex, -1, 2, 2));

	    INT src = 0, des = 1;
	    INT counter = 0;
        std::list<_vertex_>::iterator it;

        for(INT i=0; i<6; i++)
        {//for all 6 plane

            while(true)
            {//for each edge in m_vlist[src]

                it=m_vlist[src].begin();
                _vertex_ va = *it++;
                _vertex_ vb = *it;

                FLOAT Da = Dot((VEC3*)&(va.v), m_clipPlaneNormal+i)-m_clipPlane[i];
                FLOAT Db = Dot((VEC3*)&(vb.v), m_clipPlaneNormal+i)-m_clipPlane[i];

                if(Da >= m_clipEpsilon)
                {
                    if(Db >= m_clipEpsilon)
                    {
                        m_vlist[des].push_back(vb);
                        counter++;
                    }
                    else
                    {
                        FLOAT t = fabs(Da)/(fabs(Da)+fabs(Db));
                        VERTEX4 lerpVertex = Lerp(va.v, vb.v, t);
                        m_vlist[des].push_back(_vertex_(lerpVertex, t, va.s, vb.e));
                    }

                }
                else if(Db >= m_clipEpsilon)
                {
                    FLOAT t = fabs(Da)/(fabs(Da)+fabs(Db));
                    VERTEX4 lerpVertex = Lerp(va.v, vb.v, t);
                    m_vlist[des].push_back(_vertex_(lerpVertex, t, va.s, vb.e));

                    m_vlist[des].push_back(vb);
                }
                else
                {
                    counter--;
                }

                if(it == m_vlist[src].begin())
                {
                    break;
                }

            }

            m_vlist[src].clear();
            src = !src;
            des = !des;
        }


        if(counter == 18)
        {//do not need to clip any vertex
            m_triangles.push(*m_pCurrentTriangle);
            return true;
        }
        else if(counter == -18)
        {//we need to clip all vertexes
            return false;
        }
        else
        {//we need to cut the triangle
            it = m_vlist[src].begin();

            VEC3 lerpNormal = Lerp(m_pCurrentTriangle->v[it->s].normal, m_pCurrentTriangle->v[it->e].normal, it->t);
            VEC3 lerpUVW = Lerp(m_pCurrentTriangle->v[it->s].texcoord,  m_pCurrentTriangle->v[it->e].texcoord,it->t);
            VSOutput v1(it->v, lerpNormal, lerpUVW);
            it++;

            lerpNormal = Lerp(m_pCurrentTriangle->v[it->s].normal, m_pCurrentTriangle->v[it->e].normal,  it->t);
            lerpUVW = Lerp(m_pCurrentTriangle->v[it->s].texcoord,  m_pCurrentTriangle->v[it->e].texcoord,it->t);
            VSOutput _v2(it->v, lerpNormal, lerpUVW);
            VSOutput& v2 = _v2;

            while(it != m_vlist[src].end())
            {
                it++;
                lerpNormal = Lerp(m_pCurrentTriangle->v[it->s].normal, m_pCurrentTriangle->v[it->e].normal,  it->t);
                lerpUVW = Lerp(m_pCurrentTriangle->v[it->s].texcoord,  m_pCurrentTriangle->v[it->e].texcoord,it->t);
                VSOutput v3(it->v, lerpNormal, lerpUVW);

                _Triangle_ triangle(v1, v2, v3);
                m_triangles.push(triangle);
                v2 = v3;
            }

        }

        m_vlist[0].clear();
        m_vlist[1].clear();

        return true;
	}

	void VertexPostProcesser::PerspectiveDivide()
	{
	    while(!m_triangles.empty())
        {
            _Triangle_ triangle = m_triangles.front();
            m_triangles.pop();

            for(INT i=0; i<3; i++)
            {
               triangle.v[i].vertex.x = triangle.v[i].vertex.x/triangle.v[i].vertex.w;
               triangle.v[i].vertex.y = triangle.v[i].vertex.y/triangle.v[i].vertex.w;
               triangle.v[i].vertex.z = triangle.v[i].vertex.z/triangle.v[i].vertex.w;
            }
        }
	}

	void VertexPostProcesser::ViewportTranform()
	{
	    while(!m_triangles.empty())
        {

        }

	}

    void VertexPostProcesser::HandleElement()
    {
        m_pCurrentTriangle = (_Triangle_*)GetInput();

        if(nullptr != m_pCurrentTriangle)
          if(TriangleClipping())
          {
              PerspectiveDivide();
              ViewportTranform();
          }

        delete m_pCurrentTriangle;
        m_pCurrentTriangle = nullptr;
    }

    void VertexPostProcesser::OnCancel()
    {}

    void VertexPostProcesser::OnPause()
    {}

    void VertexPostProcesser::OnResume()
    {}

    void VertexPostProcesser::OnRunError()
    {}

    void VertexPostProcesser::OnRunFinish()
    {}

    void VertexPostProcesser::OnStart()
    {}



    //===========================================
	//Class Technique functions
	//
	//
	//===========================================
    Technique & Technique::operator=(const Technique & other)
    {
        if(this != &other)
        {
            this->m_name = other.m_name;
            this->m_PassList = other.m_PassList;
        }

        return *this;
    }


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
