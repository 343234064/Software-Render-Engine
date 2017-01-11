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

#include <fstream>
#include <time.h>
#include <stdio.h>
#include <mutex>

#define LOG_FILE_NAME  ".\\DebugLog.txt"
#define _ERRORLOG(error) LOG(__FILE__, __LINE__, error)

namespace SRE{

    //==============================
    //Type definitions
    //
    //==============================
    typedef const unsigned int ERROR;

    const int TIME_FORMAT_LENGTH = 22;



    //==============================
    //Error type
    //
    //==============================
    ERROR SRE_ERROR_FAIL=0xFFFFFFFF;
    ERROR SRE_ERROR_INVALIDARG=0xFFFFFFFE;
    ERROR SRE_ERROR_OUTOFMEMORY=0xFFFFFFFD;
    ERROR SRE_ERROR_NULLPOINTER=0xFFFFFFFC;
    ERROR SRE_ERROR_DIVIDEBYZERO=0xFFFFFFFB;




    //==============================
    //Function declarations
    //
    //==============================
    void LOG(const char * filename, const int line, ERROR error);
    char* GetTime();


    //==============================
    //class Log Writer
    //
    //==============================
    class LogWriter
    {
    public:
        template<typename T>
        static void Write(T x)
        {
            std::lock_guard<std::mutex> lock(LogWriter::m_mutex);
            std::ofstream fout(LOG_FILE_NAME, std::ios_base::app);
            fout.seekp(std::ios_base::end);
            char* time = GetTime();
            fout.write(time, TIME_FORMAT_LENGTH);
            fout<<x<<std::endl;
            fout.close();
            delete[] time;
        }


        template<typename KEY, typename VAL>
        static void WriteKV(KEY k, VAL v)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::ofstream fout(LOG_FILE_NAME, std::ios_base::app);
            fout.seekp(std::ios_base::end);
            char* time = GetTime();
            fout.write(time, TIME_FORMAT_LENGTH);
            fout<<k<<" = "<<v<<std::endl;
            fout.close();
            delete[] time;
        }

        template<typename FILE, typename LINE, typename ERROR>
        static void Error(FILE f, LINE l, ERROR e)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::ofstream fout(LOG_FILE_NAME, std::ios_base::app);
            fout.seekp(std::ios_base::end);
            char* time = GetTime();
            fout.write(time, TIME_FORMAT_LENGTH);
            fout<<f<<", line "<<l<<":"<<e<<std::endl;
            fout.close();
            delete[] time;
        }

    private:
        static std::mutex m_mutex;
    };
    std::mutex LogWriter::m_mutex;

}

#endif // SRE_DEBUGLOG_
