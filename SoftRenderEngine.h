//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SoftRenderEngine.h
// Date: 2016/4/28
// Description:
//       Software Render Engine library.
//
//
//*****************************************************
#ifndef _SOFTRENDERENGINE_
#define _SOFTRENDERENGINE_


//==============================
//For debug use
//
//Do not define _SRE_DEBUG_ when
//you finish debugging the entire
//project
//==============================
#ifndef _SRE_DEBUG_
#define _SRE_DEBUG_
#endif


//==============================
//The following files using
//name space SRE
//==============================
#ifdef _SRE_DEBUG_
#include "SRE_DebugLog.h"
#endif


#include "SRE_GlobalsAndUtils.h"
#include "SRE_Math.h"
#include "SRE_Resources.h"
#include "SRE_Mesh.h"
#include "SRE_PipeLine.h"










#endif
