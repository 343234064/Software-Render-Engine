//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_DebugLog.cpp
// Date: 2016/5/11
// Description:
//       Implements of functions which declared
//       in SRE_DebugLog.h
//
//
//*****************************************************
#include <stdio.h>
#include "SRE_DebugLog.h"


namespace SREngine {


    //==============================
    //Output error message
    //
    //==============================
    void LOG(const char * filename, const int line, ERROR error)
    {

        printf("Log: %s, Line:%d :\n", filename, line);

        switch (error)
        {
        case SRE_ERROR_FAIL:
            printf("  %s \n", "Function executed failed!");
            break;
        case SRE_ERROR_INVALIDARG:
            printf("  %s \n", "Invalid arguments!");
            break;
        case SRE_ERROR_OUTOFMEMORY:
            printf("  %s \n", "Memory is not enough!");
            break;
        case SRE_ERROR_NULLPOINTER:
            printf("  %s \n", "There is a NULL pointer!");
            break;
        case SRE_ERROR_DIVIDEBYZERO:
            printf("  %s \n", "Value divide by a zero!");
            break;
        default:
            break;
        }

    }
}
