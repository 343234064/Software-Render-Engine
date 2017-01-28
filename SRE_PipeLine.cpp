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

        m_thread.StartThread(&BasicProcessor::Run, this);
        if(nullptr != m_callBacks)
            m_callBacks->OnStart();
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
                if(m_input)
                  m_pInputQueue->wait_and_pop(m_pCurrentElement);
                else
                  m_pCurrentElement = nullptr;

                //BasicIOElement * output;
                if(nullptr != m_callBacks)
                   m_callBacks->HandleElement(m_pCurrentElement);
                //delete m_pCurrentElement;

                if(m_output)
                  m_pOutputQueue->push(m_pCurrentElement);

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



    //===========================================
	//Class InputAssembler functions
	//
	//
	//===========================================
    /*
    void InputAssembler::SetVertexAndIndexBuffers(Buffer<void*>* vertexBuffer, Buffer<INT*>* indexBuffer)
    {
        InputElement((BasicIOElement*)vertexBuffer);

        std::lock_guard<std::mutex> lock(m_mutex);
        m_indexBuffers.push(indexBuffer);

        m_cond.notify_one();
    }*/

    void InputAssembler::SetPrimitiveTopology(SREVAR primitiveTopo)
    {
        m_primitiveTopology = primitiveTopo;
    }

    void InputAssembler::HandleElement(BasicIOElement * element)
    {
        /*
        if(!currentHandleVbuffer.empty())
        {
            getNextTriangle;
            NewTriangle;
            FillInData;
            element = NewTriangle;
        }
        else
        {
            currentHandleVbuffer = wait to get a vbuffer;
        }*/
    }

    void InputAssembler::OnCancel()
    {

    }

    void InputAssembler::OnPause()
    {

    }

    void InputAssembler::OnResume()
    {

    }

    void InputAssembler::OnRunError()
    {

    }

    void InputAssembler::OnRunFinish()
    {

    }

    void InputAssembler::OnStart()
    {

    }


    //===========================================
	//Class RunTimeData functions
	//
	//
	//===========================================
	void RunTimeData::ReleaseMeshList()
	{
        std::map<INT, BaseMesh* >::iterator it;
        BaseMesh * mesh;
        for(it=m_MeshList.begin(); it!=m_MeshList.end(); it++)
        {
            mesh = it->second;
            delete mesh;
        }
        m_MeshList.clear();

	}


	RunTimeData::RunTimeData(const RunTimeData & other):
	    BaseContainer(),
	    m_VarBuffer(other.m_VarBuffer),
	    m_MeshList(other.m_MeshList)
	{}


	RunTimeData & RunTimeData::operator=(const RunTimeData & other)
	{
	    if(this != &other)
        {
            this->m_VarBuffer = other.m_VarBuffer;
            //ReleaseMeshList();
            this->m_MeshList = other.m_MeshList;
        }

        return *this;
	}


	void RunTimeData::AddMesh(BaseMesh * mesh, INT key)
	{
	    if(nullptr != mesh)
        {
            this->m_MeshList.insert(std::map<INT, BaseMesh*>::value_type(key, mesh));
        }
        else
        {
#ifdef _SRE_DEBUG_
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
#endif
            return;
        }
	}


	void RunTimeData::RemoveMesh(INT key)
	{
        this->m_MeshList.erase(key);
	}


	BaseMesh * RunTimeData::GetMesh(INT key)
	{
	    if(this->m_MeshList.count(key)<1)
            return nullptr;
        else
            return (BaseMesh*)(this->m_MeshList.find(key)->second);
	}


	INT RunTimeData::GetMeshCount()
	{
	    return this->m_MeshList.size();
	}


    void RunTimeData::SetRenderState(SREVAR renderState, SREVAR value)
    {
        switch (renderState)
        {
        case SRE_RENDERSTATE_CULLMODE:
            this->m_VarBuffer.renderStates.CullMode=value;break;
        case SRE_RENDERSTATE_FILLMODE:
            this->m_VarBuffer.renderStates.FillMode=value;break;
        case SRE_RENDERSTATE_ZENABLE:
            this->m_VarBuffer.renderStates.ZEnable=value;break;
        default:break;
        }
    }


    void RunTimeData::SetMatrix(SREVAR matrixType, const MAT44 & matrix)
    {
        switch (matrixType)
        {
        case SRE_MATRIXTYPE_WORLD:
            this->m_VarBuffer.World=matrix;break;
        case SRE_MATRIXTYPE_VIEW:
            this->m_VarBuffer.View=matrix;break;
        case SRE_MATRIXTYPE_PROJECT:
            this->m_VarBuffer.Project=matrix;break;
        case SRE_MATRIXTYPE_WORLDVIEW:
            this->m_VarBuffer.WorldView=matrix;break;
        case SRE_MATRIXTYPE_VIEWPROJECT:
            this->m_VarBuffer.ViewProj=matrix;break;
        case SRE_MATRIXTYPE_WORLDVIEWPROJECT:
            this->m_VarBuffer.WorldViewProj=matrix;break;
        default:break;
        }

    }



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
