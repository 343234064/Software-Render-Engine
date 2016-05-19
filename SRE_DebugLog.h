#pragma once
//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_DebugLog.h
// Date: 2016/5/11
// Description:
//       Handle the error messages.
//       Mainly for debug use.
//
//*****************************************************
#ifndef SRE_DEBUGLOG_
#define SRE_DEBUGLOG_




namespace SREngine{

    //==============================
    //Type definitions
    //
    //==============================
    typedef const int ERROR;

    //==============================
    //Error type
    //
    //==============================
    ERROR SRE_ERROR_NULLPOINTER=128;
    ERROR SRE_ERROR_DIVIDEBYZERO=129;



    //==============================
    //Function declarations
    //
    //==============================
    void LOG   (const char * filename, int line, ERROR error);
    #define _LOG(error) LOG(__FILE__, __LINE__, error)


}
#endif // SRE_DEBUGLOG_
