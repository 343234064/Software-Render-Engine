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
    typedef const unsigned int ERROR;

    //==============================
    //Error type
    //
    //==============================
    ERROR SRE_ERROR_FAIL=0x0000000A;
    ERROR SRE_ERROR_INVALIDARG=0x0000000B;
    ERROR SRE_ERROR_OUTOFMEMORY=0x0000000C;
    ERROR SRE_ERROR_NULLPOINTER=0x0000000D;
    ERROR SRE_ERROR_DIVIDEBYZERO=0x0000000E;




    //==============================
    //Function declarations
    //
    //==============================
    void LOG   (const char * filename, int line, ERROR error);
    #define _LOG(error) LOG(__FILE__, __LINE__, error)


}
#endif // SRE_DEBUGLOG_
