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
    RESULT CreateBuffer(const BufferDescript* pBufferDescript, Buffer** ppOutBuffer)
    {
#ifdef _SRE_DEBUG_
       if(nullptr == pBufferDescript || nullptr == ppOutBuffer)
       {
           _LOG(SRE_ERROR_NULLPOINTER);
           return INVALIDARG;
       }

       if(pBufferDescript->m_BufferSize <=0 || pBufferDescript->m_PerDataSize<=0)
       {
           _LOG(SRE_ERROR_INVALIDARG);
           return INVALIDARG;
       }

#endif // _SRE_DEBUG_


       BufferDescript* bd = new BufferDescript(pBufferDescript->m_BufferSize,
                                               pBufferDescript->m_PerDataSize,
                                               pBufferDescript->m_BufferUsage,
                                               pBufferDescript->m_DataFormat);
       if(nullptr == bd)
         return OUTMEMORY;

       Buffer* pOutBuffer = new Buffer(bd);

       pOutBuffer->m_data =new unsigned char[bd->m_BufferSize * bd->m_PerDataSize];
       if(nullptr == pOutBuffer->m_data)
         return OUTMEMORY;

       *ppOutBuffer = pOutBuffer;

       return SUCC;
    }




    //=============================
	//Class Buffer functions
	//
	//
	//=============================
    void Buffer::SetData(INT pos, void* data)
    {
#ifdef _SRE_DEBUG_
       if(nullptr == data)
       {
           _LOG(SRE_ERROR_NULLPOINTER);
           return;
       }

       if(nullptr == this->m_pDescript || nullptr == this->m_data)
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


       memcpy(this->m_data+pos, data, this->m_pDescript->m_PerDataSize);

       return;
    }


    const void* Buffer::GetData(INT pos)
    {
#ifdef _SRE_DEBUG_
       if(nullptr == this->m_pDescript || nullptr == this->m_data)
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

       const void* pData = (const void*)(this->m_data+pos);

       return pData;

    }


}
