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

    template<typename T>
    RESULT CreateBuffer(BufferDescript & descript, T * initData, Buffer<T>** out);


    //=============================
	//Class Color
	//
	//=============================
    class Color3
    {
    public:
        Color3(const BYTE & r, const BYTE & g, const BYTE & b)
        {
            rgb[0]=r;
            rgb[1]=g;
            rgb[2]=b;
        }

        ~Color3(){}

    public:
        BYTE rgb[3];

    };

    class Color4
    {
    public:
        Color4(const BYTE & a, const BYTE & r, const BYTE & g, const BYTE & b)
        {
            argb[0]=a;
            argb[1]=r;
            argb[2]=g;
            argb[3]=b;
        }
        ~Color4();

    public:
        BYTE argb[4];
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


	};



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
        Buffer(const Buffer & other);
        virtual ~Buffer()
        {
           m_data.reset(nullptr);
        }

        void  SetData(INT pos, const T & data);
        T  &  GetData(INT pos);
        void  Reset(const T & resetData);
        INT   GetBufferSize() const
        {
            return m_descript.m_BufferSize;
        }
        SREVAR GetBufferUsage() const
        {
            return m_descript.m_BufferType;
        }
        SREVAR GetBufferDataFormat() const
        {
            return m_descript.m_DataFormat;
        }


        Buffer & operator=(const Buffer & other) = delete;

    protected:
        Buffer(BufferDescript & bufferDescript, T * initData):
               m_descript(bufferDescript),
               m_data(nullptr)
        {
            m_data.reset(new T[m_descript.m_BufferSize]);
            if(nullptr != initData)
            {
               T * dest = this->m_data.get();
	           std::copy(initData, initData + m_descript.m_BufferSize, dest);
            }
        }

        friend RESULT CreateBuffer(BufferDescript & descript, T * initData, Buffer<T>** out);
    protected:
        std::unique_ptr<T, array_deleter<T>>  m_data;
        BufferDescript                        m_descript;

	};




    //=============================
	//Class Buffer functions
	//
	//
	//=============================
	template <typename T>
	Buffer<T>::Buffer(const Buffer & other):
	    BaseContainer(),
	    m_descript(other.m_descript),
        m_data(nullptr)
	{
        this->m_data.reset(new T[this->m_descript.m_BufferSize]);
        T * source = other.m_data.get();
        T * dest = this->m_data.get();
	    std::copy(source, source + this->m_descript.m_BufferSize, dest);
	}

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

        int i=0 , n=this->m_Descript->m_BufferSize;
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

	    if(pos >= this->m_descript.m_BufferSize || pos < 0)
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
            return;
	    }

	    if(pos >= this->m_descript.m_BufferSize || pos < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return;
        }
#endif

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
        }


        void    GetAttributes(INT pos, BYTE** output);
        FLOAT   GetVertexX(INT pos);
        FLOAT   GetVertexY(INT pos);
        FLOAT   GetVertexZ(INT pos);
        FLOAT   GetVertexW(INT pos);
        VERTEX2 GetVertex2(INT pos);
        VERTEX3 GetVertex3(INT pos);
        VERTEX4 GetVertex4(INT pos);


        VertexBuffer & operator=(const VertexBuffer & other) = delete;
        VertexBuffer(const VertexBuffer & other) = delete;

    protected:
        VertexBuffer(INT vertexNumber=0, INT vertexSize=0, SREVAR dataFormat=0, void* vertexes=nullptr):
             m_vertexes(nullptr),
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
        INT            m_vertexNum;
        INT            m_attriSize;
        INT            m_vertexDimen;
        INT            m_vertexSize;
        SREVAR         m_vertexFormat;
	};



    //=============================
	//Functions
	//
	//=============================
    template<typename T>
    RESULT CreateBuffer(BufferDescript & descript, T * initData, Buffer<T>** out)
    {
        if(descript.m_BufferSize <=0) return RESULT::FAIL;
        if(descript.m_perDataSize <=0 || descript.m_perUnitSize <=0) return RESULT::FAIL;

        *out = new Buffer<T>(descript, initData);
        if(nullptr == *out) return RESULT::OUTMEMORY;

        return RESULT::SUCC;
    }
}
#endif
