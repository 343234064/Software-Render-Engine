#pragma once
//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_Buffer.h
// Date: 2016/6/17
// Description:
//       Defines resources relative classes and functions,
//       including sampler, texture, buffer.
//
//
//
//*****************************************************
#ifndef _SRE_BUFFER_
#define _SRE_BUFFER_


#include "SoftRenderEngine.h"


namespace SREngine {


    //=============================
	//Class definitions
	//
	//=============================
	class BufferDescript;
	class Buffer;





    //=============================
	//Function definitions
	//
	//=============================
	RESULT CreateBuffer(const BufferDescript* pBufferDescript, Buffer** ppOutBuffer);





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
                       INT perDataSize = 0,
                       SREVAR bufferUsage = 0,
                       SREVAR dataFormat = 0):
            m_BufferSize(bufferSize),
            m_PerDataSize(perDataSize),
            m_BufferUsage(bufferUsage),
            m_DataFormat(dataFormat)
            {}
        virtual ~BufferDescript(){}


    public:
        INT m_BufferSize;
        INT m_PerDataSize;
        SREVAR m_BufferUsage;
        SREVAR m_DataFormat;


	};



    //=============================
	//Class Buffer
	//
	//
	//Buffer which is for storing
	//data
	//
	//** DO NOT STORE POINTER TYPE DATA **
	//=============================
	class Buffer: public IContainer
	{
    public:
        Buffer(BufferDescript* pBufferDescript):
               m_pDescript(pBufferDescript),
               m_data(nullptr),
               IContainer()
               {}
        Buffer(const Buffer & other);
        virtual ~Buffer()
        {
           if(nullptr != m_pDescript)
              delete m_pDescript;
           if(nullptr != m_data)
              delete[] m_data;
        }


        void  SetData(INT pos, void* data);
        const void* GetData(INT pos);
        BufferDescript* GetDescript(){return m_pDescript;}
        void Clear();
        Buffer & operator=(const Buffer & other);

        friend RESULT CreateBuffer(const BufferDescript* pBufferDescript, Buffer** ppOutBuffer);

    protected:
        BufferDescript*  m_pDescript;
        unsigned char* m_data;



	};

}

#endif
