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

#include "SRE_GlobalsAndUtils.h"

namespace SREngine {

    //=============================
	//Function definitions
	//
	//=============================
	template <typename T>
	RESULT CreateBuffer(const BufferDescript* pBufferDescript, Buffer<T>** ppOutBuffer);





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
	class Buffer: public IContainer
	{
    public:
        Buffer(const Buffer & other);
        virtual ~Buffer()
        {
           if(nullptr != m_pDescript)
              delete m_pDescript;

           m_data.reset(nullptr);
        }

        void  SetData(INT pos, const T & data);
        T     GetData(INT pos);
        BufferDescript * GetDescript();
        Buffer & operator=(const Buffer & other);


    protected:
        Buffer(BufferDescript* pBufferDescript = nullptr):
               IContainer(),
               m_pDescript(pBufferDescript),
               m_data(nullptr)
        {}

        BufferDescript*  m_pDescript;
        unique_ptr<T[]>  m_data;

        friend RESULT CreateBuffer<>(const BufferDescript* pBufferDescript, Buffer<T>** ppOutBuffer);

	};

}

#endif
