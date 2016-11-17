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
#include "SRE_GlobalsAndUtils.h"
using std::map;

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

        virtual ~RunTimeData()
        {
            if(nullptr != m_pRenderState)
                delete m_pRenderState;

            map<INT, BaseMesh* >::iterator it;
            BaseMesh * mesh;
            for(it=m_MeshList.begin(); it!=m_MeshList.end(); it++)
            {
                mesh = it->second;
                delete mesh;
            }
            m_MeshList.clear();
        }

        void       AddMesh(BaseMesh * mesh, INT key);
        void       RemoveMesh(INT key);
        BaseMesh * GetMesh(INT key);
        INT        GetMeshCount();

        void       SetRenderState(SREVAR renderState, SREVAR value);

    protected:
        RenderStates * m_pRenderState;
        map<INT, BaseMesh* >
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
        Technique();
        virtual ~Technique();

        void Run();
        void SetName(std::string name);
        std::string GetName();

        void AddRenderPass();
        void GetRenderPassByIndex(INT index);
        INT  GetRenderPassNumber();

    protected:
        std::string  m_name;
        INT          m_RenderPassNumber;
        RenderPass * m_pPassList;

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


	};
}

#endif // SRE_PILELINE_H_INCLUDED
