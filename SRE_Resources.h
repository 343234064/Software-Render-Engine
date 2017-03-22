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
    RESULT CreateVertexBuffer(INT vertexNumber, INT vertexSize, SREVAR dataFormat, void* vertexes, VertexBuffer*  out);


    //=============================
	//Class Color
	//
	//=============================
    class Color3
    {
    public:
        Color3(const BYTE & _b=0, const BYTE & _g=0, const BYTE & _r=0)
        {
            bgr[0]=_b;
            bgr[1]=_g;
            bgr[2]=_r;
        }

        ~Color3(){}

    public:
        BYTE bgr[3];

    };

    class Color4:public Color3
    {
    public:
        Color4(const BYTE & _b=0, const BYTE & _g=0, const BYTE & _r=0, const BYTE & _a=0):
            Color3(_b, _g, _r),
            a(_a)
        {}

        ~Color4(){}

    public:
        BYTE a;

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
        Buffer(INT bufferSize, T * initData=nullptr):
               m_bufferSize(bufferSize),
               m_data(nullptr),
               m_pdata(nullptr)
        {
            if(m_bufferSize > 0)
            {
                m_data.reset(new T[m_bufferSize]);
                if(nullptr != initData)
                {
                   T * dest = this->m_data.get();
	               std::copy(initData, initData + m_bufferSize, dest);
                }
                m_pdata = m_data.get();
            }
            else
                m_bufferSize = 0;
        }
        virtual ~Buffer()
        {
           m_data.reset(nullptr);
        }

        inline void  SetData(INT pos, const T & data);
        inline T  &  GetData(INT pos);
        void  ResetData(const T & resetData);
        void  ResetData(T * resetData, INT size);
        void  ResetSize(INT size);
        INT    GetBufferSize() const
        {
            return m_bufferSize;
        }


        Buffer(const Buffer & other) = delete;
        Buffer & operator=(const Buffer & other) = delete;

    protected:
        INT                                   m_bufferSize;
        std::unique_ptr<T, array_deleter<T>>  m_data;
        T*                                    m_pdata;



	};



    //=============================
	//Class Buffer functions
	//
	//
	//=============================
	template <typename T>
	void Buffer<T>::ResetData(const T & resetData)
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
          m_pdata[i++] = resetData;

        return;
	}

	template <typename T>
	void  Buffer<T>::ResetData(T * resetData, INT size)
	{
#ifdef _SRE_DEBUG_
        if(nullptr == this->m_data)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return;
	    }
#endif

        if(nullptr == resetData || size<=0) return;
        m_pdata = this->m_data.get();
        INT offset = size>m_bufferSize? m_bufferSize: size;
		std::copy(resetData, resetData + offset, m_pdata);

	}

	template <typename T>
	void  Buffer<T>::ResetSize(INT size)
	{
        if(size<=0) return;
        m_data.reset(new T[size]);

        if(nullptr != m_data)
		{
			m_bufferSize = size;
			m_pdata = m_data.get();
		}

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
        m_pdata[pos] = data;
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

        return m_pdata[pos];
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
        SynBuffer(INT bufferSize, T * initData=nullptr):
               m_bufferSize(bufferSize),
               m_mutex(),
               m_data(nullptr),
               m_pdata(nullptr)
        {
            if(m_bufferSize > 0)
            {
                m_data.reset(new T[m_bufferSize]);
                if(nullptr != initData)
                {
                   T * dest = this->m_data.get();
	               std::copy(initData, initData + m_bufferSize, dest);
                }
                m_pdata = m_data.get();
            }
            else
                m_bufferSize = 0;
        }
        virtual ~SynBuffer()
        {
           m_data.reset(nullptr);
        }

        inline void  SetData(INT pos, const T & data);
        inline T  &  GetData(INT pos);
        void  ResetData(const T & resetData);
        void  ResetData(T * resetData, INT size);
        void  ResetSize(INT size);
        INT   GetBufferSize() const
        {
            return m_bufferSize;
        }


        SynBuffer(const SynBuffer & other) = delete;
        SynBuffer & operator=(const SynBuffer & other) = delete;

    protected:
        INT                                   m_bufferSize;
        std::mutex                       m_mutex;
        std::unique_ptr<T, array_deleter<T>>  m_data;
        T*                                    m_pdata;

	};



    //=============================
	//Class SynBuffer functions
	//
	//
	//=============================
	template <typename T>
	void SynBuffer<T>::ResetData(const T & resetData)
	{
        std::lock_guard<std::mutex> lock(m_mutex);
        int i=0 , n=m_bufferSize;
        while(i<n)
          m_pdata[i++] = resetData;

        return;
	}


	template <typename T>
	void  SynBuffer<T>::ResetData(T * resetData, INT size)
	{
        if(nullptr == resetData || size<=0) return;
        std::lock_guard<std::mutex> lock(m_mutex);
        m_pdata = this->m_data.get();
		std::copy(resetData, resetData + size>m_bufferSize? m_bufferSize: size, m_pdata);

	}

	template <typename T>
	void  SynBuffer<T>::ResetSize(INT size)
	{
        if(size<=0) return;
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.reset(new T[size]);

        if(nullptr != m_data)
		{
			m_bufferSize = size;
			m_pdata = m_data.get();
		}

	}

	template<typename T>
	void SynBuffer<T>::SetData(INT pos, const T & data)
	{
        std::lock_guard<std::mutex> lock(m_mutex);
        m_pdata[pos] = data;
	}

	template<typename T>
	T & SynBuffer<T>::GetData(INT pos)
	{
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_pdata[pos];
	}





    //=============================
	//Class SynMatBuffer
	//
	//*Synchronized*
	//2D Buffer which is for storing data
	//
	//=============================
	template <typename T>
	class SynMatBuffer: public BaseContainer, public BasicIOElement
	{
    public:
        SynMatBuffer():
               m_width(0),
               m_height(0),
               m_mutex(),
               m_pdata(nullptr)
        {}
        virtual ~SynMatBuffer()
        {
            for(INT i=0; i<m_height; i++)
                 delete[] m_pdata[i];
            delete[] m_pdata;
        }

        RESULT Create(INT width, INT height);
        inline void  SetData(INT x, INT y, const T & data);
        inline T &   GetData(INT x, INT y);
        void  Reset(const T & resetData);
        INT   GetBufferHeight() const
        {
            return m_height;
        }
        INT   GetBufferWidth() const
        {
            return m_width;
        }


        SynMatBuffer(const SynMatBuffer & other) = delete;
        SynMatBuffer & operator=(const SynMatBuffer & other) = delete;

    protected:
        INT                                   m_width;
        INT                                   m_height;
        std::mutex                       m_mutex;
        T**                                   m_pdata;

	};



    //=============================
	//Class SynMatBuffer functions
	//
	//
	//=============================
	template <typename T>
	RESULT SynMatBuffer<T>::Create(INT width, INT height)
	{
	    if(width<=0 || height<=0) return RESULT::INVALIDARG;

	    if(nullptr != m_pdata)
		{
            for(INT i=0; i<m_height; i++)
                 delete[] m_pdata[i];
            delete[] m_pdata;
		}

        m_pdata = new T*[height];
        if(nullptr == m_pdata) return RESULT::OUTMEMORY;

        for(INT i=0; i<height; i++)
        {
            m_pdata[i] = new T[width];
            if(nullptr == m_pdata[i]) return RESULT::OUTMEMORY;
        }

        m_width = width;
        m_height = height;

        return RESULT::SUCC;
	}

	template <typename T>
	void SynMatBuffer<T>::Reset(const T & resetData)
	{
        std::lock_guard<std::mutex> lock(m_mutex);
        for(INT i=0; i<m_height; i++)
          for(INT j=0; j<m_width; j++)
              m_pdata[i][j] = resetData;

	}

	template<typename T>
	void SynMatBuffer<T>::SetData(INT x, INT y, const T & data)
	{
        std::lock_guard<std::mutex> lock(m_mutex);
        m_pdata[y][x] = data;
	}

	template<typename T>
	T & SynMatBuffer<T>::GetData(INT x, INT y)
	{
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_pdata[y][x];
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
    	VertexBuffer():
			 m_vertexes(nullptr),
             m_marks(nullptr),
             m_vertexNum(0),
             m_attriSize(0),
             m_vertexDimen(0),
             m_vertexSize(0),
             m_vertexFormat(0)
		{}
        virtual ~VertexBuffer()
        {
            if(nullptr != m_vertexes)
                delete[] m_vertexes;
            if(nullptr != m_marks)
                delete[] m_marks;
        }

        BYTE*    GetVertex(INT index);
        BYTE*    GetAttributes(INT index);
        FLOAT   GetVertexX(INT index);
        FLOAT   GetVertexY(INT index);
        FLOAT   GetVertexZ(INT index);
        FLOAT   GetVertexW(INT index);
        VERTEX2 GetVertex2(INT index);
        VERTEX3 GetVertex3(INT index);
        VERTEX4 GetVertex4(INT index);
        inline INT     GetVertexNumber();
        inline INT     GetVertexDimension();
        inline SREVAR  GetVertexFormat();

        inline void  SetMark(INT index, bool m){m_marks[index]=m;}
        inline bool  GetMark(INT index){return m_marks[index];}
        void    ResetMark(bool val){memset(m_marks, val, m_vertexNum*sizeof(bool));}

        VertexBuffer & operator=(const VertexBuffer & other) = delete;
        VertexBuffer(const VertexBuffer & other) = delete;

    protected:
        BYTE *         m_vertexes;
        bool *         m_marks;
        INT              m_vertexNum;
        INT              m_attriSize;
        INT              m_vertexDimen;
        INT              m_vertexSize;
        SREVAR        m_vertexFormat;

    protected:
		friend RESULT CreateVertexBuffer(INT vertexNumber, INT vertexSize, SREVAR dataFormat, void* vertexes, VertexBuffer** out);
        friend RESULT CreateVertexBuffer(INT vertexNumber, INT vertexSize, SREVAR dataFormat, void* vertexes, VertexBuffer*   out);
	};

    //=============================
	//Class VertexBuffer functions
	//=============================
    INT VertexBuffer::GetVertexNumber()
    {
        return m_vertexNum;
    }

    INT VertexBuffer::GetVertexDimension()
    {
        return m_vertexDimen;
    }

    SREVAR VertexBuffer::GetVertexFormat()
    {
        return m_vertexFormat;
    }


    //=============================
	//Class RenderTexture
	//
	//=============================
	class RenderTexture:public BaseContainer, public BasicIOElement
	{
    public:
        RenderTexture():
            Format(0),
            Width(0),
            Height(0),
            m_pdata(nullptr),
            m_byteCount(0)
        {}
        RenderTexture(const RenderTexture & other);
        virtual ~RenderTexture()
        {
            delete[] m_pdata;
        }

        RESULT         Create();
        BYTE*           CopyTo(BYTE* dest, INT destOffset=0);
        void            DrawSquare(USINT sx, USINT sy, USINT ex, USINT ey, Color4);
        inline void  Draw(USINT px, USINT py, Color4);
        //inline void  Draw(USINT px, USINT py, BYTE*);
        inline void  Clear(INT grayLevel);

        //bool CreateFromFile()

        RenderTexture & operator=(const RenderTexture & other);

    public:
        SREVAR Format;
        USINT  Width;
        USINT  Height;

    protected:
        inline BYTE*  Get(){return m_pdata;}

    protected:
        BYTE*  m_pdata;
        USINT  m_byteCount;

	};

    //=============================
	//Class RenderTexture functions
	//=============================
    void  RenderTexture::Draw(USINT px, USINT py, Color4 color)
    {
        *((Color4*)m_pdata + py*Width + px) = color;
    }

    void  RenderTexture::Clear(INT grayLevel)
    {
        memset(m_pdata, grayLevel, Width*Height*m_byteCount);
    }
}
#endif
