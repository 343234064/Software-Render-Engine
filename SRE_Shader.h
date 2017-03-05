//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_Shader.h
// Date: 2016/11/23
// Description:
//       Defines all Shader relative classes and functions
//
//
//
//
//*****************************************************
#ifndef _SRE_SHADER_
#define _SRE_SHADER_

#include "SRE_Math.h"
#include "SRE_GlobalsAndUtils.h"


namespace SRE {
    //=============================
	//Class VertexShaderOutput
	//
	//
	//=============================
    class VSOutput:public BasicIOElement
    {
    public:
        VSOutput():
            vertex(),
            normal(),
            texcoord()
        {}
        VSOutput(VERTEX4& v):
            vertex(v),
            normal(),
            texcoord()
        {}
        VSOutput(VERTEX4& v, VEC3& n, VEC2& uv):
            vertex(v),
            normal(n),
            texcoord(uv)
        {}
        VSOutput(VERTEX4& v, VEC3& n, VEC3& uvw):
            vertex(v),
            normal(n),
            texcoord(uvw)
        {}
        VSOutput(VERTEX4* v, VEC3* n, VEC2* uv):
            vertex(*v),
            normal(*n),
            texcoord(*uv)
        {}
        VSOutput(VERTEX4* v, VEC3* n, VEC3* uvw):
            vertex(*v),
            normal(*n),
            texcoord(*uvw)
        {}
        VSOutput(const VSOutput & other):
            vertex(other.vertex),
            normal(other.normal),
            texcoord(other.texcoord)
        {}

        virtual ~VSOutput(){}

        VSOutput & LerpAllValue(VSOutput & v1, VSOutput & v2, FLOAT t)
        {
            vertex = Lerp(v1.vertex, v2.vertex, t);
            normal = Lerp(v1.normal, v2.normal, t);
            texcoord = Lerp(v1.texcoord, v2.texcoord, t);

            return *this;
        }

        VSOutput & LerpAttriValue(VSOutput & v1, VSOutput & v2, FLOAT t)
        {
            normal = Lerp(v1.normal, v2.normal, t);
            texcoord = Lerp(v1.texcoord, v2.texcoord, t);

            return *this;
        }

    public:
        VERTEX4 vertex;
        VEC3    normal;
        VEC3    texcoord;

    };



    //=============================
	//Class VertexShader
	//
	//
	//
	//=============================
	class VertexShader
	{
    public:
        VertexShader(CallBackVShader* shader, VariableBuffer* varbuffer=nullptr):
            m_pCallBackVShader(shader),
            m_pVarBuffer(varbuffer)
        {}
        virtual ~VertexShader(){}

        VSOutput*  Run(BYTE* vs_in);
        void  SetCallBackShader(CallBackVShader * callbackshader);
        void  SetVariableBuffer(VariableBuffer * varBuffer);

        VertexShader(const VertexShader & other) = delete;
        VertexShader & operator=(const VertexShader & other) = delete;

    protected:
        CallBackVShader*  m_pCallBackVShader;
        VariableBuffer*   m_pVarBuffer;

	};


    //=============================
	//Class PixelShader
	//
	//
	//
	//=============================
	class PixelShader
	{
    public:
        PixelShader();
        virtual ~PixelShader();

    protected:



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

        //2 ways to set mesh?
        //void        SetRenderMesh(BaseMesh* mesh);
        //void        SetVertexSource(INT index, void* vertexData, void* indexData);
        //move to device layer

        //Technique &  operator=(const Technique & other);

    protected:
        std::string             m_name;
        std::list<RenderPass*>  m_PassList;

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
            m_name(),
            m_pVShader(nullptr),
            m_pPShader(nullptr),
            m_pPipeLine(nullptr)
        {}
        virtual ~RenderPass()
        {
            /*
            if(nullptr != m_pVShader)
                delete m_pVShader;
            if(nullptr != m_pPShader)
                delete m_pPShader;
            if(nullptr != m_pPipeLine)
                delete m_pPipeLine;
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
        void        SetInputTarget();



        //RenderPass & operator=(const RenderPass &);

    protected:
        void        StartPipeLine();


    protected:
        std::string     m_name;
        VertexShader *  m_pVShader;
        PixelShader  *  m_pPShader;
        BasicProcessor * m_pPipeLine;



	};


}


#endif // SRE_SHADER_H_INCLUDED
