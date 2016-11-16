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
#include "SRE_GlobalsAndUtils.h"


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
            m_MeshListSize(0),
            m_pMeshList(nullptr),
            m_pRenderState(nullptr)
        {}

        virtual ~RunTimeData();

    protected:
        INT m_MeshListSize;
        BaseMesh * m_pMeshList;
        RenderStates * m_pRenderState;

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
        std::string m_name;
        INT m_RenderPassNumber;
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
