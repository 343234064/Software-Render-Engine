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
#include "SRE_Math.h"
#include "SRE_GlobalsAndUtils.h"
using std::map;
using std::list;

namespace SREngine {
    //=============================
	//Class BasicInput
	//
	//
	//User defines what need to be output
	//Need to be inherited
	//=============================
	class BasicInput:public BaseContainer
	{
    public:
        BasicInput();
        virtual ~BasicInput();

	};



    //=============================
	//Class BasicOutput
	//
	//
	//User defines what need to be output
	//Need to be inherited
	//=============================
	class BasicOutput:public BaseContainer
	{
    public:
        BasicOutput();
        virtual ~BasicOutput();

	};



    //=============================
	//Class BasicProcessor
	//
	//
	//=============================
	class BasicProcessor:public BaseTask
	{
    public:
        BasicProcessor():
            BaseTask(),
            nextStage(nullptr)
        {}
        virtual ~BasicProcessor(){}

        virtual void NextStage()=0;

    protected:
        BasicProcessor * nextStage;

        friend class BasePileLineBuilder;
	};



    //=============================
	//Class BasePileLineBuilder
	//
	//
	//
	//=============================
	class BasePileLineBuilder
	{
    public:
        BasePileLineBuilder():
            m_pPileLine(nullptr)
        {}
        BasePileLineBuilder(const BasePileLineBuilder & other)
        {
            this->m_pPileLine = other.m_pPileLine;
        }

        virtual ~BasePileLineBuilder(){}


        virtual BasicProcessor* BuildPileLine()=0;
        virtual void            AddProcessor(INT index, BasicProcessor * processor)=0;
        virtual void            RemoveProcessor(INT index)=0;

        BasicProcessor ** GetNextStage(BasicProcessor * processor)
        {
            return &(processor->nextStage);
        }

        BasePileLineBuilder & operator=(const BasePileLineBuilder & other)
        {
            if(this != &other)
            {
               this->m_pPileLine = other.m_pPileLine;
            }
            return *this;
        }

    protected:
        BasicProcessor * m_pPileLine;

	};




    //=============================
	//Class PileLineBuilder
	//
	//
	//
	//=============================
	class PileLineBuilder:public BasePileLineBuilder
	{
    public:
        PileLineBuilder():
            BasePileLineBuilder()
        {}

        virtual ~PileLineBuilder(){}

        BasicProcessor* BuildPileLine();
        void            AddProcessor(INT index, BasicProcessor * processor);
        void            RemoveProcessor(INT index);


	};







    //=============================
	//Class InputAssembler
	//
	//
	//
	//=============================
	class InputAssembler:public BasicProcessor
	{
    public:
        InputAssembler():
            BasicProcessor()
        {}
        virtual ~InputAssembler(){}


	};



    //=============================
	//Class VertexPostProcesser
	//
	//
	//
	//=============================
    class VertexPostProcesser:public BasicProcessor
    {
    public:
        VertexPostProcesser():
            BasicProcessor()
        {}
        virtual ~VertexPostProcesser();

    };



    //=============================
	//Class Rasterizer
	//
	//
	//
	//=============================
    class Rasterizer:public BasicProcessor
    {
    public:
        Rasterizer():
            BasicProcessor()
        {}
        virtual ~Rasterizer();

    };



    //=============================
	//Class OutputMerger
	//
	//
	//
	//=============================
    class OutputMerger:public BasicProcessor
    {
    public:
        OutputMerger():
            BasicProcessor()
        {}
        virtual ~OutputMerger();

    };





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
	//Class VariableBuffer
	//
	//
	//
	//=============================
    class VariableBuffer
    {
    public:
        VariableBuffer(){}
        virtual ~VariableBuffer(){}

    public:
        RenderStates renderStates;
        VEC2   ViewportSize;
        MAT44  WorldViewProj;
        MAT44  WorldView;
        MAT44  ViewProj;
        MAT44  World;
        MAT44  View;
        MAT44  Project;

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
            m_pVarBuffer(new VariableBuffer()),
            m_MeshList()
        {}
        RunTimeData(const RunTimeData & other);

        virtual ~RunTimeData()
        {
            if(nullptr != m_pVarBuffer)
                delete m_pVarBuffer;

             m_MeshList.clear();
            //ReleaseMeshList();
        }

        void       AddMesh(BaseMesh * mesh, INT key);
        void       RemoveMesh(INT key);
        BaseMesh * GetMesh(INT key);
        INT        GetMeshCount();

        void       ReleaseMeshList();
        void       SetRenderState(SREVAR renderState, SREVAR value);
        void       SetMatrix(SREVAR matrixType, const MAT44 & matrix);

        RunTimeData & operator=(const RunTimeData & other);

    protected:
        VariableBuffer * m_pVarBuffer;
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
        void         RemoveRenderPassByName(std::string name);
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
        RenderPass():
            BaseTask(),
            m_pVShader(nullptr),
            m_pPShader(nullptr)
        {}
        virtual ~RenderPass()
        {
            /*
            if(nullptr != m_pVShader)
                delete m_pVShader;
            if(nullptr != m_pPShader)
                delete m_pPShader;
            */
        }

        RenderPass(const RenderPass &);

        void        Run();
        void        SetName(std::string name);
        std::string GetName();

        void        SetVertexShader(const VertexShader * vs);
        void        SetPixelShader(const PixelShader * ps);
        void        SetRenderState();
        void        SetMatrix();
        void        SetOutputTarget();
        //void        SetInputTarget();
        void        SetRenderMesh();

        RenderPass & operator=(const RenderPass &);

    protected:
        void        StartPileLine();


    protected:
        std::string     m_name;
        VertexShader *  m_pVShader;
        PixelShader  *  m_pPShader;



	};





}

#endif // SRE_PILELINE_H_INCLUDED
