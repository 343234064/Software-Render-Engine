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

#include <stdlib.h>
#include "SRE_GlobalsAndUtils.h"

namespace SREngine {

    //=============================
	//Function definitions
	//
	//=============================
	template <typename T>
	RESULT CreateBuffer(const BufferDescript* pBufferDescript, Buffer<T>** ppOutBuffer);



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
	//Class BufferDescript
	//
	//A buffer description, which used
	//to create a new buffer
	//
	//BufferSize: how many data are there
	//            in this buffer
	//PerDataSize: the size of each data(bit)
	//BufferUsage: what is this buffer uses for
	//DataFlag: data format
	//=============================
	class BufferDescript
	{
    public:
        BufferDescript(INT bufferSize = 0,
                       SREVAR bufferType = 0,
                       SREVAR dataFormat = 0):
            m_BufferSize(bufferSize),
            m_BufferType(bufferType),
            m_DataFormat(dataFormat)
            {}
        virtual ~BufferDescript(){}


    public:
        INT m_BufferSize;
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
	class Buffer: public BaseContainer
	{
    public:
        Buffer(const Buffer & other);
        virtual ~Buffer()
        {
           if(nullptr != m_pDescript)
              delete m_pDescript;

           m_data.reset(nullptr);
        }

        //void  SetData(INT pos, const T & data);
        //T  &  GetData(INT pos, T & output);
        void  Reset(const T & resetData);
        BufferDescript * GetDescript();
        Buffer & operator=(const Buffer & other);

    public:
        unique_ptr<T, array_deleter<T>>  m_data;

    protected:
        Buffer(BufferDescript* pBufferDescript = nullptr):
               BaseContainer(),
               m_pDescript(pBufferDescript),
               m_data(nullptr)
        {}

    protected:
        BufferDescript*  m_pDescript;


        friend RESULT CreateBuffer<>(const BufferDescript* pBufferDescript, Buffer<T>** ppOutBuffer);

	};



	//===========================================
	//Public functions
	//
	//
	//===========================================

    //===========================================
	//CreateBuffer
	//
	//
	//===========================================
	template <typename T>
    RESULT CreateBuffer(const BufferDescript* pBufferDescript, Buffer<T>** ppOutBuffer)
    {
#ifdef _SRE_DEBUG_
       if(nullptr == pBufferDescript || nullptr == ppOutBuffer)
       {
           _LOG(SRE_ERROR_NULLPOINTER);
           return RESULT::INVALIDARG;
       }

       if(pBufferDescript->m_BufferSize <=0)
       {
           _LOG(SRE_ERROR_INVALIDARG);
           return RESULT::INVALIDARG;
       }

#endif // _SRE_DEBUG_

       BufferDescript * pBdes = new BufferDescript(pBufferDescript->m_BufferSize,
                                                   pBufferDescript->m_BufferType,
                                                   pBufferDescript->m_DataFormat);
       if(nullptr == pBdes) return RESULT::OUTMEMORY;

       Buffer<T> * pbuffer = new Buffer<T>(pBdes);
       pbuffer->m_data.reset(new T[pBdes->m_BufferSize]);

       *ppOutBuffer = pbuffer;

       return RESULT::SUCC;
    }




    //=============================
	//Class Buffer functions
	//
	//
	//=============================
	template <typename T>
	Buffer<T>::Buffer(const Buffer & other):
	    BaseContainer(),
	    m_pDescript(new BufferDescript(other.m_pDescript->m_BufferSize,
                                       other.m_pDescript->m_BufferType,
                                       other.m_pDescript->m_DataFormat)),
        m_data(nullptr)
	{
        this->m_data.reset(new T[this->m_pDescript->m_BufferSize]);
        T * source = other.m_data.get();
        T * dest = this->m_data.get();
	    std::copy(source, source + this->m_pDescript->m_BufferSize, dest);
	}

	template <typename T>
	Buffer<T> & Buffer<T>::operator=(const Buffer & other)
	{
	    if(this != &other)
        {
           if(this->m_pDescript == nullptr) this->m_pDescript = new BufferDescript();

           this->m_pDescript->m_BufferSize = other.m_pDescript->m_BufferSize;
           this->m_pDescript->m_BufferType = other.m_pDescript->m_BufferType;
           this->m_pDescript->m_DataFormat = other.m_pDescript->m_DataFormat;

           this->m_data.reset(new T[this->m_pDescript->m_BufferSize]);
           T * source = other.m_data.get();
           T * dest = this->m_data.get();
	       std::copy(source, source + this->m_pDescript->m_BufferSize, dest);
        }

	    return *this;
	}

	template <typename T>
	BufferDescript * Buffer<T>::GetDescript()
	{
	    return this->m_pDescript;
	}

	template <typename T>
	void Buffer<T>::Reset(const T & resetData)
	{
#ifdef _SRE_DEBUG_
        if(nullptr == this->m_data)
        {
            _LOG(SRE_ERROR_NULLPOINTER);
            return;
	    }
#endif
        int i=0 , n=this->m_pDescript->m_BufferSize;
        while(i<n)
          this->m_data.get()[i++] = resetData;

        return;
	}
}

#endif
