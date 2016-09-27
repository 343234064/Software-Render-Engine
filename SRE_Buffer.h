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
	RESULT CreateBuffer(const BufferDescript* bufferDescript, Buffer** ppOutBuffer);





    //=============================
	//Class BufferDescript
	//
	//A buffer description, which used
	//to create a new buffer
	//=============================
	class BufferDescript
	{
    public:
        BufferDescript():
            BufferSize(0), PerDataSize(0), BufferUsage(0), DataFlags(0)
            {}
        ~BufferDescript(){}


    public:
        INT BufferSize;
        INT PerDataSize;
        SREVAR BufferUsage;
        SREVAR DataFlags;


	};



    //=============================
	//Class SRE_Buffer
	//
	//
	//Buffer
	//=============================
	class Buffer: public IContainer
	{
    public:
        Buffer():
            m_pDescript(nullptr), data(nullptr)
            {}
        Buffer(const Buffer & other);
        virtual ~Buffer();


        BufferDescript* GetDescript(){return m_pDescript;}
        Buffer & operator=(const Buffer & other);



    protected:
        BufferDescript* m_pDescript;
        void* data;



	};

}

#endif
