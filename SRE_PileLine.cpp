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
	    m_pRenderState(new RenderStates(*(other.m_pRenderState))),
	    m_MeshList(other.m_MeshList)
	{}


	RunTimeData & RunTimeData::operator=(const RunTimeData & other)
	{
	    if(this != &other)
        {
            if(nullptr != this->m_pRenderState)
            {
                *(this->m_pRenderState) = *(other.m_pRenderState);
            }
            else
            {
                this->m_pRenderState = new RenderStates(*(other.m_pRenderState));
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


    void RunTimeData::SetRenderState(SREVAR renderState, SREVAR value)
    {
        switch (renderState)
        {
        case SRE_RENDERSTATE_CULLMODE:
            this->m_pRenderState->CullMode=value;break;
        case SRE_RENDERSTATE_FILLMODE:
            this->m_pRenderState->FillMode=value;break;
        case SRE_RENDERSTATE_ZENABLE:
            this->m_pRenderState->ZEnable=value;break;
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
        for(it=m_PassList.begin(); it!=m_PassList.end(); it++)
        {
            pass = (*it);
            delete pass;
        }
        m_PassList.clear();
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
        for(INT i=0, it=m_PassList.begin(); it!=m_PassList.end(); it++, i++)
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















}
