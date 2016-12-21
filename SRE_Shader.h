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

#include "SRE_GlobalsAndUtils.h"


namespace SRE {
    //=============================
	//Class VertexShaderInput
	//
	//
	//=============================
	class VSInput:public BasicIOElement
	{
    public:
        VSInput();
        virtual ~VSInput();

        void * Position();
        void * Normal();
        void * Diffuse();
        void * Specular();
        void * Binormal();
        void * Tangent();

	};



    //=============================
	//Class VertexShader
	//
	//
	//
	//=============================
	class VertexShader:public BaseTask
	{
    public:
        VertexShader();
        virtual ~VertexShader();

        void  Run();
        void  SetCallBackShader(CallBackVShader * callbackshader);
        void  SetInputArgumentFormat(SREVAR format);

    protected:
        CallBackVShader * m_callBackVShader;
        SREVAR            m_inputArguFormat;


	};


    //=============================
	//Class PixelShader
	//
	//
	//
	//=============================
	class PixelShader:public BaseTask
	{
    public:
        PixelShader();
        virtual ~PixelShader();

    protected:



	};





}


#endif // SRE_SHADER_H_INCLUDED
