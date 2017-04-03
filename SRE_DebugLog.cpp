//*****************************************************
//
// Software Render Engine
// Version 0.01 by XJL
//
// File: SRE_DebugLog.cpp
// Date: 2016/5/11
// Description:
//       Implements of functions which declared
//       in SRE_DebugLog.h
//
//
//*****************************************************
#include "SRE_DebugLog.h"


namespace SRE {
    //==============================
    //Return system time
    //
    //example:
    //[2000/01/01 00:00:00]
    //==============================
    char* GetTime()
    {
        time_t t=time(nullptr);
        char* out=new char[TIME_FORMAT_LENGTH];
        strftime(out, TIME_FORMAT_LENGTH*sizeof(char), "[%Y/%m/%d %X]",localtime(&t));
        return out;
    }


    //==============================
    //Output error message
    //
    //==============================
    void LOG(const char * filename, const int line, SR_ERR error)
    {

        switch (error)
        {
        case SRE_ERROR_FAIL:

            g_log.Error(filename, line, "Function executed failed!");
            break;
        case SRE_ERROR_INVALIDARG:
            g_log.Error(filename, line, "Invalid arguments!");
            break;
        case SRE_ERROR_OUTOFMEMORY:
            g_log.Error(filename, line, "Memory is not enough!");
            break;
        case SRE_ERROR_NULLPOINTER:
            g_log.Error(filename, line, "There is a NULL pointer!");
            break;
        case SRE_ERROR_DIVIDEBYZERO:
            g_log.Error(filename, line, "Value divide by a zero!");
            break;
        default:
            break;
        }

    }
}
