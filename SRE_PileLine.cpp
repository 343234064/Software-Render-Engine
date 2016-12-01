//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_PileLine.cpp
// Date: 2016/11/26
// Description:
//       Implements of mesh functions and classes which declared
//       in SRE_PileLine.h
//
//
//*****************************************************
#include <string>
#include "SRE_PileLine.h"


namespace SREngine {
    //===========================================
	//Class PileLineBuilder functions
	//
	//
	//===========================================
	BasicProcessor * PileLineBuilder::BuildPileLine()
	{
	    if(nullptr != this->m_pPileLine)
	    return this->m_pPileLine;
	}

	void PileLineBuilder::AddProcessor(INT index, BasicProcessor * processor)
	{
	    if(nullptr != this->m_pPileLine)
        {
            if(index == 0)
            {
                *(GetNextStage(processor)) = this->m_pPileLine;
                this->m_pPileLine = processor;
            }
            else
            {
                INT i=1;
                BasicProcessor * curr = this->m_pPileLine;
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
            this->m_pPileLine = processor;
	}

	void PileLineBuilder::RemoveProcessor(INT index)
	{
	    if(nullptr == this->m_pPileLine)
            return;
	    else if(index == 0)
        {
            this->m_pPileLine = *(GetNextStage(this->m_pPileLine));
        }
        else
        {
            INT i=1;
            BasicProcessor * curr = this->m_pPileLine;
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
        if(nullptr == renderPass) return;

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
        if(nullptr == renderPass) return;

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
