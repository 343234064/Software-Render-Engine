//*****************************************************
//
// Software Render Engine
// Version 0.01 by XJL
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
	//Class Basic I/O Buffer
	//
	//
	//=============================
	template<typename T>
	class BasicIOBuffer
	{
    public:
        BasicIOBuffer():
            m_queue(),
            m_mutex(),
            m_cond(),
            m_stopwait(false)
        {}
        virtual ~BasicIOBuffer()
        {}

        void push(T data)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stopwait = false;
            m_queue.push(std::make_shared(data));
            m_cond.notify_one();
        }

        void push(T* pdata)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_stopwait = false;
            m_queue.push(std::shared_ptr<T>(pdata));
            m_cond.notify_one();
        }

        void push(std::shared_ptr<T> pdata)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stopwait = false;
            m_queue.push(pdata);
            m_cond.notify_one();
        }

        std::shared_ptr<T> wait_and_pop()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cond.wait(lock, [this]{return !m_queue.empty()||m_stopwait;});
            if(m_stopwait) {return std::shared_ptr<T>();}

            std::shared_ptr<T> res = m_queue.front();
            m_queue.pop();

            return res;
        }

        std::shared_ptr<T> top()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::shared_ptr<T> res = m_queue.front();
            return res;
        }

        void wait_and_pop(T ** out)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cond.wait(lock, [this]{return !m_queue.empty()||m_stopwait;});
            if(m_stopwait) {*out=nullptr;return;}

            std::shared_ptr<T> p = m_queue.front();
            *out = p.get();
            m_queue.pop();
        }

        void top(T ** out)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            std::shared_ptr<T> p = m_queue.front();
            *out = p.get();
        }

        bool empty() const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_queue.empty();
        }

        void StopWait()
        {
        	   std::lock_guard<std::mutex> lock(m_mutex);
           	m_stopwait = true;
            m_cond.notify_all();
        }

        void Clear()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            INT num=m_queue.size();
            while(num>0)
            {
               m_queue.pop();
               num--;
            }
        }

        INT Size()
        {
           std::lock_guard<std::mutex> lock(m_mutex);
           return m_queue.size();
        }

        BasicIOBuffer(const BasicIOBuffer & other) = delete;
        BasicIOBuffer & operator=(const BasicIOBuffer & other) = delete;

    protected:
        std::queue<std::shared_ptr<T>>
                                         m_queue;
        std::mutex                m_mutex;
        std::condition_variable
                                         m_cond;
		bool                           m_stopwait;



	};

   template<typename T>
	class BasicIOBufferEx
	{
    public:
        BasicIOBufferEx():
            m_queue(),
            m_mutex(),
            m_cond(),
            m_stopwait(false)
        {}
        virtual ~BasicIOBufferEx()
        {}

        void push(T data)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stopwait = false;
            m_queue.push(data);
            m_cond.notify_one();
        }

        T top()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_queue.front();
        }

        void wait_and_pop(T & out)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cond.wait(lock, [this]{return !m_queue.empty()||m_stopwait;});
            if(m_stopwait) {return;}

            out = m_queue.front();
            m_queue.pop();
        }

        void top(T & out)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            out= m_queue.front();
        }

        bool empty() const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_queue.empty();
        }

        void StopWait()
        {
        	   std::lock_guard<std::mutex> lock(m_mutex);
           	m_stopwait = true;
            m_cond.notify_all();
        }

        void Clear()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            INT num=m_queue.size();
            while(num>0)
            {
               m_queue.pop();
               num--;
            }
        }

        BasicIOBufferEx(const BasicIOBufferEx & other) = delete;
        BasicIOBufferEx & operator=(const BasicIOBufferEx & other) = delete;

    protected:
        std::queue<T>           m_queue;
        std::mutex                  m_mutex;
        std::condition_variable
                                           m_cond;
		  bool                           m_stopwait;



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
        Buffer(INT bufferSize=0, T * initData=nullptr):
               m_width(0),
               m_height(0),
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
        Buffer(INT width, INT height, T * initData=nullptr):
               m_width(0),
               m_height(0),
               m_bufferSize(width*height),
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
                m_width = width;
                m_height = height;
            }
            else
            {
                m_bufferSize = 0;
            }
        }
        virtual ~Buffer()
        {
           m_data.reset(nullptr);
        }

        inline void  SetData(INT pos, const T & data);
        inline T  &   GetData(INT pos);
        inline bool  Empty() const;
        void  SetDataSquare(INT sx, INT sy, INT ex, INT ey, const T & data);
        void  ResetData(const T & resetData);
        bool  ResetBuffer(T * resetData, INT size);
        bool  ResetBuffer(T * resetData, INT width, INT height);
        INT    GetBufferSize() const
        {
            return m_bufferSize;
        }


        Buffer(const Buffer & other) = delete;
        Buffer & operator=(const Buffer & other) = delete;

    protected:
        INT                                   m_width;
        INT                                   m_height;
        INT                                   m_bufferSize;
        std::unique_ptr<T, array_deleter<T>>  m_data;
        T*                                     m_pdata;



	};



   //=============================
	//Class Buffer functions
	//
	//
	//=============================
	template <typename T>
	bool Buffer<T>::Empty() const
	{
	   return m_bufferSize<=0;
	}

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
	bool  Buffer<T>::ResetBuffer(T * resetData, INT size)
	{
        if(size<=0) return false;
        if(nullptr == m_data || size>m_bufferSize)
         {
                m_data.reset(new T[size]);
                if(nullptr != resetData)
                {
                   T * dest = m_data.get();
	                std::copy(resetData, resetData + size, dest);
                }
         }
        else
        {
               std::copy(resetData, resetData + size, m_pdata);
        }

        m_bufferSize = size;
        m_pdata = m_data.get();

        return true;
	}

	template <typename T>
	bool  Buffer<T>::ResetBuffer(T * resetData, INT width, INT height)
	{
	     INT size = width*height;
        if(size<=0) return false;
        if(nullptr == m_data || size>m_bufferSize)
         {
                m_data.reset(new T[size]);
                if(nullptr != resetData)
                {
                   T * dest = m_data.get();
	                std::copy(resetData, resetData + size, dest);
                }
         }
        else
        {
               std::copy(resetData, resetData + size, m_pdata);
        }

        m_width = width;
        m_height = height;
        m_bufferSize = size;
        m_pdata = m_data.get();

        return true;
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
    void  Buffer<T>::SetDataSquare(INT sx, INT sy, INT ex, INT ey, const T & data)
    {
#ifdef _SRE_DEBUG_
        if(nullptr == this->m_data)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return;
	    }

	    if(m_width <= 0 || m_height <= 0)
        {
            _ERRORLOG(SRE_ERROR_FAIL);
            return;
        }

	    if(m_width <= ex || m_height <= ey || sx<0 || sy <0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return;
        }
#endif

		for(INT py=sy; py<=ey; py++)
         for(INT px=sx; px<=ex; px++)
				*(m_pdata+py*m_width+px) = data;

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
        }

        BYTE*     GetVertexAddr(INT index);
        BYTE*     GetVertex(INT index);
        BYTE*     GetAttributes(INT index);
        FLOAT    GetVertexX(INT index);
        FLOAT    GetVertexY(INT index);
        FLOAT    GetVertexZ(INT index);
        FLOAT    GetVertexW(INT index);
        VERTEX2 GetVertex2(INT index);
        VERTEX3 GetVertex3(INT index);
        VERTEX4 GetVertex4(INT index);
        inline INT     GetVertexNumber();
        inline INT     GetVertexDimension();
        inline SREVAR  GetVertexFormat();


        VertexBuffer & operator=(const VertexBuffer & other) = delete;
        VertexBuffer(const VertexBuffer & other) = delete;

    public:
        BYTE *         m_vertexes;
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
	//Class Texture
	//
	//=============================
    class Texture:public BasicIOElement
    {
    public:
          Texture():
             m_width(0),
             m_height(0),
             m_bytesPerColor(0),
             m_data(nullptr)
          {}
          Texture(USINT width, USINT height, USINT bytesPerColor, BYTE* bits):
             m_width(width),
             m_height(height),
             m_bytesPerColor(bytesPerColor),
             m_data(bits)
          {}
          virtual ~Texture()
          {
             if(nullptr != m_data)
               delete[] m_data;
          }

          void   Set(BYTE* image, USINT width, USINT height, USINT bytesPerColor);
          inline USINT  GetWidth() const;
          inline USINT  GetHeight() const;
          inline USINT  GetBytesPerColor() const;


    protected:
          USINT m_width;
          USINT m_height;
          USINT m_bytesPerColor;
          BYTE* m_data;

          friend class Sampler;
    };

   //=============================
	//Class Texture functions
	//=============================
   USINT Texture::GetWidth() const
   {
      return m_width;
   }

   USINT Texture::GetHeight() const
   {
      return m_height;
   }

   USINT Texture::GetBytesPerColor() const
   {
      return m_bytesPerColor;
   }


   //=============================
	//Class RenderTexture
	//
	//=============================
	class RenderTexture:public Texture
	{
    public:
        RenderTexture():
            Texture(),
            m_mutex(),
            m_cond(),
            m_lock(false),
            m_pdata(nullptr)
        {}
        RenderTexture(const RenderTexture & other);
        virtual ~RenderTexture()
        {
        	   if(nullptr != m_pdata)
               delete[] m_pdata;
            m_data = nullptr;
        }

        RESULT         Create(USINT width, USINT height);
        RESULT         Resize(USINT width, USINT height);
        void            DrawSquare(INT sx, INT sy, INT ex, INT ey, RTCOLOR color);
        void            Clear(RTCOLOR color);
        inline void  Draw(INT px, INT py, RTCOLOR color);
        inline void  Draw(INT pos, RTCOLOR color);
        inline void  Clear(INT grayLevel);
        //bool CreateFromFile()

        RenderTexture & operator=(const RenderTexture & other);

    public:
        void          Lock();
        void          Unlock();
        RTCOLOR*  Wait_Get();

    protected:
        inline RTCOLOR* Get();
        friend class Device;

    protected:
        std::mutex                       m_mutex;
        std::condition_variable  m_cond;
        bool                                m_lock;
        RTCOLOR*                        m_pdata;

        friend class Sampler;
	};

   //=============================
	//Class RenderTexture functions
	//=============================
    void  RenderTexture::Draw(INT px, INT py, RTCOLOR color)
    {
        *(m_pdata + py*m_width + px) = color;
    }

   void  RenderTexture::Draw(INT pos, RTCOLOR color)
    {
        *(m_pdata + pos) = color;
    }


    void  RenderTexture::Clear(INT grayLevel)
    {
        memset(m_pdata, grayLevel, m_width*m_height*m_bytesPerColor);
    }


    RTCOLOR* RenderTexture::Get()
    {
       return m_pdata;
    }


   //=============================
	//Class Sampler
	//
	//=============================
    class Sampler
    {
    public:
         Sampler():
            WrapMode(SRE_WRAPMODE_BORDER),
            FilterMode(SRE_FILTERMODE_NEAREST)
         {}
         virtual ~Sampler(){}

         Color3 getcolor3(Texture* tex, FLOAT u, FLOAT v);
         Color4 getcolor4(Texture* tex, FLOAT u, FLOAT v);

    public:
         SREVAR WrapMode;
         SREVAR FilterMode;

    };


   //=============================
	//Class Light
	//
	//=============================
    class Light
    {
    public:
         Light():
            position(),
            direction(),
            diffuse(),
            intensity(0.0f)
         {}
         Light(VEC3 pos, VEC3 direct, Color3 diff, FLOAT intens):
            position(pos),
            direction(direct),
            diffuse(diff),
            intensity(intens)
         {}
         ~Light(){}

    public:
         VEC3    position;
         VEC3    direction;
         Color3  diffuse;
         FLOAT   intensity;
    };

}
#endif
