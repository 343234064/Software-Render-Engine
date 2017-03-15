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
#ifndef _SRE_DEBUGLOG_
#define _SRE_DEBUGLOG_

#include <fstream>
#include <time.h>
#include <mutex>

#define LOG_FILE_NAME  ".\\DebugLog.txt"
#define _ERRORLOG(error) LOG(__FILE__, __LINE__, error)
#define _ERRORMSG(error) g_log.Error(__FILE__, __LINE__, error)

namespace SRE{
    //==============================
    //Type definitions
    //
    //==============================
    typedef const unsigned int SR_ERR;

    const int TIME_FORMAT_LENGTH = 22;


    //==============================
    //Error type
    //
    //==============================
    SR_ERR SRE_ERROR_FAIL=0xFFFFFFFF;
    SR_ERR SRE_ERROR_INVALIDARG=0xFFFFFFFE;
    SR_ERR SRE_ERROR_OUTOFMEMORY=0xFFFFFFFD;
    SR_ERR SRE_ERROR_NULLPOINTER=0xFFFFFFFC;
    SR_ERR SRE_ERROR_DIVIDEBYZERO=0xFFFFFFFB;




    //==============================
    //Function declarations
    //
    //==============================
    void LOG(const char * filename, const int line, SR_ERR error);
    char* GetTime();


    //==============================
    //class Log Writer
    //
    //==============================
    class LogWriter
    {
    public:
        LogWriter():
            m_mutex()
        {}
        ~LogWriter(){}

        template<typename T>
        void Write(T x)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::ofstream fout(LOG_FILE_NAME, std::ios_base::app);
            fout.seekp(std::ios_base::end);
            char* time = GetTime();
            fout.write(time, TIME_FORMAT_LENGTH);
            fout<<x<<std::endl;
            fout.close();
            delete[] time;

        }


        template<typename KEY, typename VAL>
        void WriteKV(KEY k, VAL v)
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

        template<typename KEY, typename VAL>
        void WriteKV(KEY k, VAL v1, VAL v2)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::ofstream fout(LOG_FILE_NAME, std::ios_base::app);
            fout.seekp(std::ios_base::end);
            char* time = GetTime();
            fout.write(time, TIME_FORMAT_LENGTH);
            fout<<k<<" = "<<v1<<", "<<v2<<std::endl;
            fout.close();
            delete[] time;
        }

        template<typename KEY, typename VAL>
        void WriteKV(KEY k, VAL v1, VAL v2, VAL v3)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::ofstream fout(LOG_FILE_NAME, std::ios_base::app);
            fout.seekp(std::ios_base::end);
            char* time = GetTime();
            fout.write(time, TIME_FORMAT_LENGTH);
            fout<<k<<" = "<<v1<<", "<<v2<<", "<<v3<<std::endl;
            fout.close();
            delete[] time;
        }

        template<typename KEY, typename VAL>
        void WriteKV(KEY k, VAL v1, VAL v2, VAL v3, VAL v4)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::ofstream fout(LOG_FILE_NAME, std::ios_base::app);
            fout.seekp(std::ios_base::end);
            char* time = GetTime();
            fout.write(time, TIME_FORMAT_LENGTH);
            fout<<k<<" = "<<v1<<", "<<v2<<", "<<v3<<", "<<v4<<std::endl;
            fout.close();
            delete[] time;
        }
        template<typename FILE, typename LINE, typename _ERROR>
        void Error(FILE f, LINE l, _ERROR e)
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
        std::mutex  m_mutex;


    };
    static LogWriter g_log;

}

#endif // SRE_DEBUGLOG_
