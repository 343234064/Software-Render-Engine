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


namespace SREngine {
    //===========================================
	//Class PipeLineBuilder functions
	//
	//
	//===========================================
	BasicProcessor * PipeLineBuilder::BuildPipeLine()
	{
	    if(nullptr != this->m_pPipeLine)
	       return this->m_pPipeLine;
        else
           return nullptr;
	}

	void PipeLineBuilder::AddProcessor(INT index, BasicProcessor * processor)
	{
	    if(nullptr != this->m_pPipeLine)
        {
            if(index == 0)
            {
                *(GetNextStage(processor)) = this->m_pPipeLine;
                this->m_pPipeLine = processor;
            }
            else
            {
                INT i=1;
                BasicProcessor * curr = this->m_pPipeLine;
                BasicProcessor ** next;
                while(true)
                {
                    next = GetNextStage(curr);
                    if(i == index)
                    {
                        *(GetNextStage(processor)) = *next;
                        *next = processor;
                        break;
                    }
                    else
                    {
                        i++;
                        if(*next != nullptr)
                            curr = *next;
                        else
                            break;
                    }
                }
            }
        }
        else
            this->m_pPipeLine = processor;
	}

	void PipeLineBuilder::RemoveProcessor(INT index)
	{
	    if(nullptr == this->m_pPipeLine)
            return;
	    else if(index == 0)
        {
            this->m_pPipeLine = *(GetNextStage(this->m_pPipeLine));
        }
        else
        {
            INT i=1;
            BasicProcessor * curr = this->m_pPipeLine;
            BasicProcessor ** next = GetNextStage(curr);
            while(*next != nullptr)
            {
                if(i == index)
                {
                    *(GetNextStage(curr)) = *(GetNextStage(*next));
                    break;
                }
                else
                {
                    i++;
                    curr = *next;
                    next = GetNextStage(curr);
                }
            }
        }

	}


    //===========================================
	//Class InputAssembler functions
	//
	//
	//===========================================
    void InputAssembler::Run()
    {
        this->NextStage();
    }


    void InputAssembler::NextStage()
    {
        if(nullptr != this->m_pNextStage)
           this->m_pNextStage.Run();
    }


    void InputAssembler::PassArgument(SREVAR usage, void * argu)
    {
#ifdef _SRE_DEBUG_
        if(nullptr == argu)
        {
            _LOG(SRE_ERROR_NULLPOINTER);
            return;
        }
#endif
        if(usage == SRE_USAGE_NEXTINPUT)
        {
            if(nullptr != this->m_pRunTimeData)
            {
                TriangleMesh * pTriangle = new TriangleMesh((TriangleMesh)(*argu));
                this->m_pRunTimeData.AddMesh((BaseMesh*)pTriangle, this->m_pRunTimeData.GetMeshCount()+1);
            }
        }
        else
            if(nullptr != this->m_pNextStage)
               this->m_pNextStage.PassArgument(usage, argu);

    }


    //===========================================
	//Class RunTimeData functions
	//
	//
	//===========================================
	void RunTimeData::ReleaseMeshList()
	{
        map<INT, BaseMesh* >::iterator it;
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
	    m_pVarBuffer(new VariableBuffer(*(other.m_pVarBuffer))),
	    m_MeshList(other.m_MeshList)
	{}


	RunTimeData & RunTimeData::operator=(const RunTimeData & other)
	{
	    if(this != &other)
        {
            if(nullptr != this->m_pVarBuffer)
            {
                *(this->m_pVarBuffer) = *(other.m_pVarBuffer);
            }
            else
            {
                this->m_pVarBuffer = new VariableBuffer(*(other.m_pVarBuffer));
            }

            //ReleaseMeshList();
            this->m_MeshList = other.m_MeshList;
        }

        return *this;
	}


	void RunTimeData::AddMesh(BaseMesh * mesh, INT key)
	{
	    if(nullptr != mesh)
        {
            this->m_MeshList.insert(map<INT, BaseMesh*>::value_type(key, mesh));
        }
        else
        {
#ifdef _SRE_DEBUG_
            _LOG(SRE_ERROR_NULLPOINTER);
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


	VariableBuffer* RunTimeData::GetVaribleBuffer()
	{
	    return this->m_pVarBuffer;
	}


    void RunTimeData::SetRenderState(SREVAR renderState, SREVAR value)
    {
        switch (renderState)
        {
        case SRE_RENDERSTATE_CULLMODE:
            this->m_pVarBuffer->renderStates.CullMode=value;break;
        case SRE_RENDERSTATE_FILLMODE:
            this->m_pVarBuffer->renderStates.FillMode=value;break;
        case SRE_RENDERSTATE_ZENABLE:
            this->m_pVarBuffer->renderStates.ZEnable=value;break;
        default:break;
        }
    }


    void RunTimeData::SetMatrix(SREVAR matrixType, const MAT44 & matrix)
    {
        switch (matrixType)
        {
        case SRE_MATRIXTYPE_WORLD:
            this->m_pVarBuffer->World=matrix;break;
        case SRE_MATRIXTYPE_VIEW:
            this->m_pVarBuffer->View=matrix;break;
        case SRE_MATRIXTYPE_PROJECT:
            this->m_pVarBuffer->Project=matrix;break;
        case SRE_MATRIXTYPE_WORLDVIEW:
            this->m_pVarBuffer->WorldView=matrix;break;
        case SRE_MATRIXTYPE_VIEWPROJECT:
            this->m_pVarBuffer->ViewProj=matrix;break;
        case SRE_MATRIXTYPE_WORLDVIEWPROJECT:
            this->m_pVarBuffer->WorldViewProj=matrix;break;
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
        list<RenderPass*>::iterator it;
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
            _LOG(SRE_ERROR_NULLPOINTER);
#endif
            return;
        }
        list<RenderPass*>::iterator it;
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
            _LOG(SRE_ERROR_NULLPOINTER);
#endif
            return;
        }

        m_PassList.push_back(renderPass);
    }


    void Technique::RemoveRenderPassByIndex(INT index)
    {
        list<RenderPass*>::iterator it;
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
        list<RenderPass*>::iterator it;
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
        list<RenderPass*>::iterator it;
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
        list<RenderPass*>::iterator it;
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
        list<RenderPass*>::iterator it;
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
