//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_GlobalsAndUtils.cpp
// Date: 2016/4/28
// Description:
//       Implements of functions which declared
//       in SRE_GlobalsAndUtils.h
//
//
//*****************************************************

#include "SRE_GlobalsAndUtils.h"



namespace SREngine {



    //===========================================
	//Utility functions
	//
	//
	//===========================================
    void Release(void ** mem, bool array)
    {
        if(nullptr==mem) return;
        if(nullptr!=(*mem))
        {
            if(array)
                delete [] (*mem);
            else
                delete (*mem);
        }
    }

}
