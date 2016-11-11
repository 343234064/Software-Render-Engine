//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_Buffer.cpp
// Date: 2016/10/10
// Description:
//       Implements of buffer functions and classes which declared
//       in SRE_Buffer.h
//
//
//*****************************************************
#include<cstring>
#include "SRE_Buffer.h"


namespace SREngine {

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
       if(nullptr = pBdes) return RESULT::OUTMEMORY;

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
	Buffer<T>::Buffer(const Buffer & other)
	{
	    this->m_pDescript = new BufferDescript(other.m_pDescript->m_BufferSize,
                                               other.m_pDescript->m_BufferType,
                                               other.m_pDescript->m_DataFormat);
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
    void Buffer<T>::SetData(INT pos, const T & data)
    {
#ifdef _SRE_DEBUG_
       if(nullptr == data)
       {
           _LOG(SRE_ERROR_NULLPOINTER);
           return;
       }

       if(nullptr == this->m_data)
       {
           _LOG(SRE_ERROR_FAIL);
           return;
       }

       if(pos<0 || pos>=this->m_pDescript->m_BufferSize)
       {
           _LOG(SRE_ERROR_INVALIDARG);
           return;
       }
#endif // _SRE_DEBUG_

       this->m_data.get()[pos] = data;

       return;
    }

    template <typename T>
    T Buffer<T>::GetData(INT pos)
    {
#ifdef _SRE_DEBUG_
       if(nullptr == this->m_data)
       {
           _LOG(SRE_ERROR_FAIL);
           return nullptr;
       }

       if(pos<0 || pos>=this->m_pDescript->m_BufferSize)
       {
           _LOG(SRE_ERROR_INVALIDARG);
           return nullptr;
       }

#endif // _SRE_DEBUG_

       return this->m_data.get()[pos];

    }


}
