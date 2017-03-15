//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_Resources.h
// Date: 2016/6/17
// Description:
//       Defines resources relative classes and functions,
//       including sampler, texture, buffer, color.
//
//
//
//*****************************************************
#ifndef _SRE_BUFFER_
#define _SRE_BUFFER_

#include "SRE_Math.h"
#include "SRE_GlobalsAndUtils.h"

namespace SRE {
    //=============================
	//Functions declaration
	//
	//=============================
    RESULT CreateVertexBuffer(INT vertexNumber, INT vertexSize, SREVAR dataFormat, void* vertexes, VertexBuffer** out);



    //=============================
	//Class Color
	//
	//=============================
    class Color3
    {
    public:
        Color3(const BYTE & b=0, const BYTE & g=0, const BYTE & r=0)
        {
            bgr[0]=b;
            bgr[1]=g;
            bgr[2]=r;
        }

        ~Color3(){}

    public:
        BYTE bgr[3];

    };

    class Color4
    {
    public:
        Color4(const BYTE & b=0, const BYTE & g=0, const BYTE & r=0, const BYTE & a=0)
        {
            bgra[0]=b;
            bgra[1]=g;
            bgra[2]=r;
            bgra[3]=a;
        }

        ~Color4(){}

    public:
        BYTE bgra[4];

    };



    //=============================
	//Class BufferDescript<typename T>
	//
	//A buffer description, which used
	//to create a new buffer
	//
	//
	//BufferSize: how many T are there
	//            in this buffer
	//PerUnitSize: the number of T that a whole meaningful data contains
	//PerDataSize: the size of T
	//BufferType: what is this buffer uses for
	//DataFlag: data format
	//=============================
	/*
	class BufferDescript
	{
    public:
        BufferDescript(INT bufferSize = 0,
                       INT perUnitSize = 1,
                       INT perDataSize = 1,
                       SREVAR bufferType = 0,
                       SREVAR dataFormat = 0):
            m_BufferSize(bufferSize),
            m_perUnitSize(perUnitSize),
            m_perDataSize(perDataSize),
            m_BufferType(bufferType),
            m_DataFormat(dataFormat)
            {}
        virtual ~BufferDescript(){}


    public:
        INT m_BufferSize;
        INT m_perUnitSize;
        INT m_perDataSize;
        SREVAR m_BufferType;
        SREVAR m_DataFormat;


	};*/



    //=============================
	//Class Buffer
	//
	//
	//Buffer which is for storing data
	//
	//=============================
	template <typename T>
	class Buffer: public BaseContainer, public BasicIOElement
	{
    public:
        Buffer(INT bufferSize, T * initData):
               m_bufferSize(bufferSize),
               m_data(nullptr)
        {
            if(m_bufferSize > 0)
            {
                m_data.reset(new T[m_bufferSize]);
                if(nullptr != initData)
                {
                   T * dest = this->m_data.get();
	               std::copy(initData, initData + m_bufferSize, dest);
                }
            }
            else
                m_bufferSize = 0;
        }
        virtual ~Buffer()
        {
           m_data.reset(nullptr);
        }

        void  SetData(INT pos, const T & data);
        T  &  GetData(INT pos);
        void  Reset(const T & resetData);
        INT   GetBufferSize() const
        {
            return m_bufferSize;
        }


        Buffer(const Buffer & other) = delete;
        Buffer & operator=(const Buffer & other) = delete;

    protected:
        INT                                   m_bufferSize;
        std::unique_ptr<T, array_deleter<T>>  m_data;



	};



    //=============================
	//Class Buffer functions
	//
	//
	//=============================
	template <typename T>
	void Buffer<T>::Reset(const T & resetData)
	{
#ifdef _SRE_DEBUG_
        if(nullptr == this->m_data)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return;
	    }
#endif

        int i=0 , n=m_bufferSize;
        while(i<n)
          this->m_data.get()[i++] = resetData;

        return;
	}

	template<typename T>
	void Buffer<T>::SetData(INT pos, const T & data)
	{
#ifdef _SRE_DEBUG_
        if(nullptr == this->m_data)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return;
	    }

	    if(pos >= m_bufferSize || pos < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return;
        }
#endif
        this->m_data.get()[pos] = data;
	}

	template<typename T>
	T & Buffer<T>::GetData(INT pos)
	{
#ifdef _SRE_DEBUG_
        if(nullptr == this->m_data)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            throw;
	    }

	    if(pos >= m_bufferSize || pos < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            throw;
        }
#endif

        return this->m_data.get()[pos];
	}



    //=============================
	//Class SynBuffer
	//
	//*Synchronized*
	//Buffer which is for storing data
	//
	//=============================
	template <typename T>
	class SynBuffer: public BaseContainer, public BasicIOElement
	{
    public:
        SynBuffer(INT bufferSize, T * initData):
               m_bufferSize(bufferSize),
               m_mutex(),
               m_data(nullptr)
        {
            if(m_bufferSize > 0)
            {
                m_data.reset(new T[m_bufferSize]);
                if(nullptr != initData)
                {
                   T * dest = this->m_data.get();
	               std::copy(initData, initData + m_bufferSize, dest);
                }
            }
            else
                m_bufferSize = 0;
        }
        virtual ~SynBuffer()
        {
           m_data.reset(nullptr);
        }

        void  SetData(INT pos, const T & data);
        T  &  GetData(INT pos);
        void  Reset(const T & resetData);
        INT   GetBufferSize() const
        {
            return m_bufferSize;
        }


        SynBuffer(const SynBuffer & other) = delete;
        SynBuffer & operator=(const SynBuffer & other) = delete;

    protected:
        INT                                   m_bufferSize;
        std::mutex                            m_mutex;
        std::unique_ptr<T, array_deleter<T>>  m_data;

	};



    //=============================
	//Class SynBuffer functions
	//
	//
	//=============================
	template <typename T>
	void SynBuffer<T>::Reset(const T & resetData)
	{
        std::lock_guard<std::mutex> lock(m_mutex);
        int i=0 , n=m_bufferSize;
        while(i<n)
          this->m_data.get()[i++] = resetData;

        return;
	}

	template<typename T>
	void SynBuffer<T>::SetData(INT pos, const T & data)
	{
        std::lock_guard<std::mutex> lock(m_mutex);
        this->m_data.get()[pos] = data;
	}

	template<typename T>
	T & SynBuffer<T>::GetData(INT pos)
	{
        std::lock_guard<std::mutex> lock(m_mutex);
        return this->m_data.get()[pos];
	}




    //=============================
	//Class VertexBuffer
	//
	//
    //Buffer to store vertexes
	//=============================
	class VertexBuffer:public BaseContainer, public BasicIOElement
	{
    public:
        virtual ~VertexBuffer()
        {
            if(nullptr != m_vertexes)
                delete[] m_vertexes;
            if(nullptr != m_marks)
                delete[] m_marks;
        }

        BYTE*   GetVertex(INT index);
        BYTE*   GetAttributes(INT index);
        FLOAT   GetVertexX(INT index);
        FLOAT   GetVertexY(INT index);
        FLOAT   GetVertexZ(INT index);
        FLOAT   GetVertexW(INT index);
        VERTEX2 GetVertex2(INT index);
        VERTEX3 GetVertex3(INT index);
        VERTEX4 GetVertex4(INT index);
        INT     GetVertexNumber();
        INT     GetVertexDimension();
        SREVAR  GetVertexFormat();

        void    SetMark(INT index, bool m){m_marks[index]=m;}
        bool    GetMark(INT index){return m_marks[index];}
        void    ResetMark(bool val){memset(m_marks, val, m_vertexNum*sizeof(bool));}

        VertexBuffer & operator=(const VertexBuffer & other) = delete;
        VertexBuffer(const VertexBuffer & other) = delete;

    protected:
        VertexBuffer(INT vertexNumber=0, INT vertexSize=0, SREVAR dataFormat=0, void* vertexes=nullptr):
             m_vertexes(nullptr),
             m_marks(nullptr),
             m_vertexNum(vertexNumber),
             m_attriSize(0),
             m_vertexDimen(0),
             m_vertexSize(vertexSize),
             m_vertexFormat(dataFormat)
        {
            if(nullptr != vertexes)
            {
                int length = vertexNumber * vertexSize;
                m_vertexes = new BYTE[length];
                memcpy(m_vertexes, vertexes, length);
            }

            m_marks = new bool[vertexNumber];
            memset(m_marks, 0, vertexNumber*sizeof(bool));

            if((m_vertexFormat & SRE_FORMAT_VERTEX_XY)==SRE_FORMAT_VERTEX_XY)
               m_vertexDimen = 2;
            else if((m_vertexFormat & SRE_FORMAT_VERTEX_XYZ)==SRE_FORMAT_VERTEX_XYZ)
               m_vertexDimen = 3;
            else if((m_vertexFormat & SRE_FORMAT_VERTEX_XYZW)==SRE_FORMAT_VERTEX_XYZW)
               m_vertexDimen = 4;

            m_attriSize = vertexSize - m_vertexDimen*sizeof(FLOAT);

        }

        friend RESULT CreateVertexBuffer(INT vertexNumber, INT vertexSize, SREVAR dataFormat, void* vertexes, VertexBuffer** out);
    protected:
        BYTE *         m_vertexes;
        bool *         m_marks;
        INT            m_vertexNum;
        INT            m_attriSize;
        INT            m_vertexDimen;
        INT            m_vertexSize;
        SREVAR         m_vertexFormat;
	};


}
#endif
