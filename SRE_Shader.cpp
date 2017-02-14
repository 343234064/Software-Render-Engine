//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_Shader.cpp
// Date: 2017/02/01
// Description:
//       Implements of mesh functions and classes which declared
//       in SRE_Shader.h
//
//
//*****************************************************
#include "SRE_Shader.h"



namespace SRE {
    //=============================
	//Class VertexShader functions
	//
	//
	//=============================
	void VertexShader::SetCallBackShader(CallBackVShader* shader)
	{
	    m_pCallBackVShader = shader;
	}

	void VertexShader::SetVariableBuffer(VariableBuffer* varbuffer)
	{
	    m_pVarBuffer = varbuffer;
	}

	VSOutput* VertexShader::Run(BYTE* vs_in)
	{
	   return m_pCallBackVShader(vs_in, m_pVarBuffer);
	}





}
