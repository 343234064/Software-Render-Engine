//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_PileLine.h
// Date: 2016/11/16
// Description:
//       Defines all PileLine relative classes and functions
//
//
//
//
//*****************************************************
#ifndef _SRE_PILELINE_
#define _SRE_PILELINE_

#include <string>
#include <map>
#include <list>
#include "SRE_GlobalsAndUtils.h"
using std::map;
using std::list;

namespace SREngine {
    //=============================
	//Class RenderStates
	//
	//
	//
	//=============================
	class RenderStates
	{
    public:
        RenderStates():
            FillMode(SRE_FILLMODE_SOLID),
            CullMode(SRE_CULLMODE_CCW),
            ZEnable(SRE_TRUE)
        {}
        RenderStates(const RenderStates & other):
            FillMode(other.FillMode),
            CullMode(other.CullMode),
            ZEnable(other.ZEnable)
        {}

        RenderStates & operator=(const RenderStates & other)
        {
            if(this != &other)
            {
                this->FillMode = other.FillMode;
                this->CullMode = other.CullMode;
                this->ZEnable = other.ZEnable;
            }

            return *this;
        }
        ~RenderStates(){}

    public:
        SREVAR FillMode;
        SREVAR CullMode;
        SREVAR ZEnable;
	};


    //=============================
	//Class RunTimeData
	//
	//
	//
	//=============================
    class RunTimeData:public BaseContainer
    {
    public:
        RunTimeData():
            BaseContainer(),
            m_pRenderState(new RenderStates()),
            m_MeshList()
        {}
        RunTimeData(const RunTimeData & other);

        virtual ~RunTimeData()
        {
            if(nullptr != m_pRenderState)
                delete m_pRenderState;

             m_MeshList.clear();
            //ReleaseMeshList();
        }

        void       AddMesh(BaseMesh * mesh, INT key);
        void       RemoveMesh(INT key);
        BaseMesh * GetMesh(INT key);
        INT        GetMeshCount();

        void       ReleaseMeshList();
        void       SetRenderState(SREVAR renderState, SREVAR value);

        RunTimeData & operator=(const RunTimeData & other);

    protected:
        RenderStates * m_pRenderState;
        map<INT, BaseMesh*>
                       m_MeshList;


    };


    //=============================
	//Class Technique
	//
	//
	//
	//=============================
    class Technique:public BaseTask
    {
    public:
        Technique(std::string name="\0"):
            BaseTask(),
            m_name(name),
            m_PassList()
        {}

        Technique(const Technique & other):
            BaseTask(),
            m_name(other.m_name),
            m_PassList()
        {}

        virtual ~Technique()
        {
            //
            m_PassList.clear();
        }

        void         Run();
        void         SetName(std::string name);
        std::string  GetName();

        void         AddRenderPass(RenderPass * renderPass, INT index);
        void         AddRenderPassBack(RenderPass * renderPass);
        void         RemoveRenderPassByIndex(INT index);
        void         RemoveRenderPassByName(INT index);
        void         ReleasePassList();
        INT          GetRenderPassNumber();
        RenderPass * GetRenderPassByIndex(INT index);
        RenderPass * GetRenderPassByName(std::string name);


        Technique &  operator=(const Technique & other);

    protected:
        std::string        m_name;
        list<RenderPass*>  m_PassList;

    };


    //=============================
	//Class RenderPass
	//
	//
	//
	//=============================
	class RenderPass:public BaseTask
	{
    public:
        RenderPass();
        virtual ~RenderPass();

        void Run();
        void SetName(std::string name);
        std::string GetName();

    protected:
        std::string m_name;


	};
}

#endif // SRE_PILELINE_H_INCLUDED
