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
#include "SRE_Resources.h"
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
            texcoord(),
            color()
        {}
        VSOutput(VERTEX4& v):
            vertex(v),
            normal(),
            texcoord(),
            color()
        {}
        VSOutput(VERTEX4& v, VEC3& n, VEC2& uv, VCOLOR& col):
            vertex(v),
            normal(n),
            texcoord(uv),
            color(col)
        {}
        VSOutput(VERTEX4* v, VEC3* n, VEC2* uv, VCOLOR* col):
            vertex(*v),
            normal(*n),
            texcoord(*uv),
            color(*col)
        {}
        VSOutput(const VSOutput & other):
            vertex(other.vertex),
            normal(other.normal),
            texcoord(other.texcoord),
            color(other.color)
        {}

        VSOutput(VERTEX4& v, VSOutput& vs1, VSOutput& vs2, FLOAT t):
            vertex(v),
            normal(vs1.normal.x + t*(vs2.normal.x-vs1.normal.x),
                   vs1.normal.y + t*(vs2.normal.y-vs1.normal.y),
                   vs1.normal.z + t*(vs2.normal.z-vs1.normal.z)),
            texcoord(vs1.texcoord.x + t*(vs2.texcoord.x-vs1.texcoord.x),
                           vs1.texcoord.y + t*(vs2.texcoord.y-vs1.texcoord.y)),
            color(vs1.color.r + t*(vs2.color.r-vs1.color.r),
                     vs1.color.g + t*(vs2.color.g-vs1.color.g),
                     vs1.color.b + t*(vs2.color.g-vs1.color.b),
                     vs1.color.a + t*(vs2.color.g-vs1.color.a))
        {}

        virtual ~VSOutput(){}


    public:
        VERTEX4   vertex;
        VEC3        normal;
        VEC2        texcoord;
        VCOLOR   color;

    };



    //=============================
	//Class PixelShaderInput
	//
	//
	//=============================
	class PSInput:public BasicIOElement
	{
    public:
        PSInput():
            vertex(),
            texcoord(),
            normal(),
            color()
        {}
        PSInput(VERTEX3& _vertex, VEC3& _normal, VEC2& _uv, VCOLOR& _color):
            vertex(_vertex), texcoord(_uv), normal(_normal), color(_color)
        {}

        //barycentric interpolation, use custom z value
        PSInput(VSOutput& v0, FLOAT w0,
                    VSOutput& v1, FLOAT w1,
                    VSOutput& v2, FLOAT w2,
                    FLOAT interpolatedZ):
            vertex((v0.vertex.x-v2.vertex.x)*w0 + (v1.vertex.x-v2.vertex.x)*w1 + v2.vertex.x,
                      (v0.vertex.y-v2.vertex.y)*w0 + (v1.vertex.y-v2.vertex.y)*w1 + v2.vertex.y,
                      interpolatedZ),
             texcoord(),
             normal(),
             color()
        {
           FLOAT W0 = w0*v0.vertex.w*interpolatedZ, W1 = w1*v1.vertex.w*interpolatedZ, W2 = w2*v1.vertex.w*interpolatedZ;
           texcoord.x = v0.texcoord.x*W0 + v1.texcoord.x*W1 + v2.texcoord.x*W2;
           texcoord.y = v0.texcoord.y*W0 + v1.texcoord.y*W1 + v2.texcoord.y*W2;

           normal.x = v0.normal.x*W0 + v1.normal.x*W1 + v2.normal.x*W2;
           normal.y = v0.normal.y*W0 + v1.normal.y*W1 + v2.normal.y*W2;
           normal.z = v0.normal.z*W0 + v1.normal.z*W1 + v2.normal.z*W2;

           color.r  = v0.color.r*W0 + v1.color.r*W1 + v2.color.r*W2;
           color.g = v0.color.g*W0 + v1.color.g*W1 + v2.color.g*W2;
           color.b = v0.color.b*W0 + v1.color.b*W1 + v2.color.b*W2;
           color.a = v0.color.a*W0 + v1.color.a*W1 + v2.color.a*W2;
        }

        //barycentric interpolation
        PSInput(VSOutput& v0, FLOAT w0,
                    VSOutput& v1, FLOAT w1,
                    VSOutput& v2, FLOAT w2):
             vertex((v0.vertex.x-v2.vertex.x)*w0 + (v1.vertex.x-v2.vertex.x)*w1 + v2.vertex.x,
                    (v0.vertex.y-v2.vertex.y)*w0 + (v1.vertex.y-v2.vertex.y)*w1 + v2.vertex.y,
                    (v0.vertex.z-v2.vertex.z)*w0 + (v1.vertex.z-v2.vertex.z)*w1 + v2.vertex.z),
             texcoord(),
             normal(),
             color()
        {
           FLOAT W0 = w0*v0.vertex.w*vertex.z, W1 = w1*v1.vertex.w*vertex.z, W2 = w2*v1.vertex.w*vertex.z;
           texcoord.x = v0.texcoord.x*W0 + v1.texcoord.x*W1 + v2.texcoord.x*W2;
           texcoord.y = v0.texcoord.y*W0 + v1.texcoord.y*W1 + v2.texcoord.y*W2;

           normal.x = v0.normal.x*W0 + v1.normal.x*W1 + v2.normal.x*W2;
           normal.y = v0.normal.y*W0 + v1.normal.y*W1 + v2.normal.y*W2;
           normal.z = v0.normal.z*W0 + v1.normal.z*W1 + v2.normal.z*W2;

           color.r  = v0.color.r*W0 + v1.color.r*W1 + v2.color.r*W2;
           color.g = v0.color.g*W0 + v1.color.g*W1 + v2.color.g*W2;
           color.b = v0.color.b*W0 + v1.color.b*W1 + v2.color.b*W2;
           color.a = v0.color.a*W0 + v1.color.a*W1 + v2.color.a*W2;

        }
        //fx*fy*(x1y1 - x1y2 - x2y1 + x2y2) - fy*(x1y1 - x1y2)  - fx*(x1y1 - x2y1) + x1y1;
        //2d-linear interpolation, use custom z value
        PSInput(PSInput& v00, PSInput& v10, PSInput& v01, PSInput& v11,
                FLOAT fx, FLOAT fy,
                FLOAT interpolatedZ):
             vertex(Lerp(v00.vertex.x, v10.vertex.x, v01.vertex.x, v11.vertex.x, fx, fy),
                       Lerp(v00.vertex.y, v10.vertex.y, v01.vertex.y, v11.vertex.y, fx, fy),
                    interpolatedZ),
             texcoord(Lerp(v00.texcoord.x, v10.texcoord.x, v01.texcoord.x, v11.texcoord.x, fx, fy),
                      Lerp(v00.texcoord.y, v10.texcoord.y, v01.texcoord.y, v11.texcoord.y, fx, fy)),
             normal(Lerp(v00.normal.x, v10.normal.x, v01.normal.x, v11.normal.x, fx, fy),
                        Lerp(v00.normal.y, v10.normal.y, v01.normal.y, v11.normal.y, fx, fy),
                        Lerp(v00.normal.z, v10.normal.z, v01.normal.z, v11.normal.z, fx, fy)),
             color(Lerp(v00.color.r, v10.color.r, v01.color.r, v11.color.r, fx, fy),
                      Lerp(v00.color.g, v10.color.g, v01.color.g, v11.color.g, fx, fy),
                      Lerp(v00.color.b, v10.color.b, v01.color.b, v11.color.b, fx, fy),
                      Lerp(v00.color.a, v10.color.a, v01.color.a, v11.color.a, fx, fy))
         {}

        //2d-linear interpolation
        PSInput(PSInput& v00, PSInput& v10, PSInput& v01, PSInput& v11,
                FLOAT fx, FLOAT fy):
             vertex(Lerp(v00.vertex.x, v10.vertex.x, v01.vertex.x, v11.vertex.x, fx, fy),
                        Lerp(v00.vertex.y, v10.vertex.y, v01.vertex.y, v11.vertex.y, fx, fy),
                        Lerp(v00.vertex.z, v10.vertex.z, v01.vertex.x, v11.vertex.z, fx, fy)),
             texcoord(Lerp(v00.texcoord.x, v10.texcoord.x, v01.texcoord.x, v11.texcoord.x, fx, fy),
                            Lerp(v00.texcoord.y, v10.texcoord.y, v01.texcoord.y, v11.texcoord.y, fx, fy)),
             normal(Lerp(v00.normal.x, v10.normal.x, v01.normal.x, v11.normal.x, fx, fy),
                        Lerp(v00.normal.y, v10.normal.y, v01.normal.y, v11.normal.y, fx, fy),
                        Lerp(v00.normal.z, v10.normal.z, v01.normal.z, v11.normal.z, fx, fy)),
              color(Lerp(v00.color.r, v10.color.r, v01.color.r, v11.color.r, fx, fy),
                      Lerp(v00.color.g, v10.color.g, v01.color.g, v11.color.g, fx, fy),
                      Lerp(v00.color.b, v10.color.b, v01.color.b, v11.color.b, fx, fy),
                      Lerp(v00.color.a, v10.color.a, v01.color.a, v11.color.a, fx, fy))
        {}

        virtual ~PSInput()
        {

        }

    public:
        VERTEX3  vertex;
        VEC2       texcoord;
        VEC3       normal;
        VCOLOR  color;

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
        VertexShader(CallBackVShader* shader=nullptr):
            m_pCallBackVShader(shader)
        {}
        virtual ~VertexShader(){}

        inline VSOutput*  Run(BYTE* vs_in, VariableBuffer* varbuffer);
        inline void  SetCallBackShader(CallBackVShader * callbackshader);

        VertexShader(const VertexShader & other) = delete;
        VertexShader & operator=(const VertexShader & other) = delete;

    protected:
        CallBackVShader*  m_pCallBackVShader;

	};

    //=============================
	//Class VertexShader functions
	//=============================
	void VertexShader::SetCallBackShader(CallBackVShader* shader)
	{
	    m_pCallBackVShader = shader;
	}

	VSOutput* VertexShader::Run(BYTE* vs_in, VariableBuffer* varbuffer)
	{
	   return m_pCallBackVShader(vs_in, varbuffer);
	}



    //=============================
	//Class PixelShader
	//
	//
	//
	//=============================
	class PixelShader
	{
    public:
        PixelShader(CallBackPShader* shader=nullptr, SREVAR inputformat=SRE_SHADERINPUTFORMAT_TEXCOORD):
            InputFormat(inputformat),
            m_pCallBackPShader(shader)
        {}
        virtual ~PixelShader(){};

        inline Color4 Run(PSInput& ps_in);
        inline void   SetCallBackShader(CallBackPShader * callbackshader);

        PixelShader(const PixelShader & other) = delete;
        PixelShader & operator=(const PixelShader & other) = delete;

    public:
        SREVAR InputFormat;

    protected:
        CallBackPShader* m_pCallBackPShader;



	};


    //=============================
	//Class PixelShader functions
	//=============================
    void PixelShader::SetCallBackShader(CallBackPShader* shader)
    {
        m_pCallBackPShader = shader;
    }

    Color4 PixelShader::Run(PSInput& ps_in)
    {
        return m_pCallBackPShader(ps_in);
    }




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
            m_PassList.clear();
        }

        void             Run();
        void             SetName(std::string name);
        std::string   GetName();

        void             AddRenderPass(RenderPass * renderPass, INT index);
        void             AddRenderPassBack(RenderPass * renderPass);
        void             RemoveRenderPassByIndex(INT index);
        void             RemoveRenderPassByName(std::string name);
        void             ClearPassList();
        RenderPass * GetRenderPassByIndex(INT index);
        RenderPass * GetRenderPassByName(std::string name);

        inline INT     GetRenderPassNumber() const;


    protected:
        std::string                   m_name;
        std::list<RenderPass>  m_PassList;

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
            m_pPShader(nullptr)
        {}
        virtual ~RenderPass()
        {
        }

        RenderPass(const RenderPass &);

        void        Run();
        void        SetName(std::string name);
        std::string GetName();

        void        SetVertexShader(const VertexShader * vs);
        void        SetPixelShader(const PixelShader * ps);
        void        SetRenderState(SREVAR stateType, SREVAR val);
        void        SetMatrix(SREVAR matrixType, MAT44& mat);
        void        SetOutputTarget(RenderTexture* target);

        //RenderPass & operator=(const RenderPass &);

    protected:
        std::string                  m_name;
        const VertexShader *   m_pVShader;
        const PixelShader  *     m_pPShader;


	};


}


#endif // SRE_SHADER_H_INCLUDED
