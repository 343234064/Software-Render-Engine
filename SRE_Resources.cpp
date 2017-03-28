//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_Resources.cpp
// Date: 2016/10/10
// Description:
//       Implements of buffer functions and classes which declared
//       in SRE_Resources.h
//
//
//*****************************************************

#include "SRE_Resources.h"


namespace SRE {
    //=============================
	//Class VertexBuffer functions
	//
	//
	//=============================
	BYTE* VertexBuffer::GetVertex(INT index)
	{
#ifdef _SRE_DEBUG_
	    if(index >= m_vertexNum|| index < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return nullptr;
        }
#endif
        BYTE* output = new BYTE[m_vertexSize];
        if(nullptr == output) return nullptr;
        memcpy(output, m_vertexes + index*m_vertexSize, m_vertexSize);

        return output;
	}

    BYTE* VertexBuffer::GetAttributes(INT index)
    {
#ifdef _SRE_DEBUG_
	    if(index >= m_vertexNum|| index < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return nullptr;
        }
#endif

        BYTE* output = new BYTE[m_attriSize];
        if(nullptr == output) return nullptr;
        memcpy(output, m_vertexes + index*m_vertexSize + m_vertexSize - m_attriSize, m_attriSize);
        return output;
    }

    FLOAT VertexBuffer::GetVertexX(INT index)
    {
#ifdef _SRE_DEBUG_
	    if(index >= m_vertexNum || index < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return 0.0f;
        }
#endif

        BYTE* val = m_vertexes + index*m_vertexSize;
        return *((FLOAT*)val);
    }

    FLOAT VertexBuffer::GetVertexY(INT index)
    {
#ifdef _SRE_DEBUG_
	    if(index >= m_vertexNum || index < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return 0.0f;
        }
#endif

        BYTE* val = m_vertexes + index*m_vertexSize + sizeof(FLOAT);
        return *((FLOAT*)val);
    }

    FLOAT VertexBuffer::GetVertexZ(INT index)
    {
#ifdef _SRE_DEBUG_
	    if(index >= m_vertexNum || index < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return 0.0f;
        }
#endif

        BYTE* val = m_vertexes + index*m_vertexSize + 2*sizeof(FLOAT);
        return *((FLOAT*)val);
    }

    FLOAT VertexBuffer::GetVertexW(INT index)
    {
#ifdef _SRE_DEBUG_
	    if(index >= m_vertexNum || index < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return 0.0f;
        }
#endif
        BYTE* val = m_vertexes + index*m_vertexSize + 3*sizeof(FLOAT);
        return *((FLOAT*)val);
    }

    VERTEX2 VertexBuffer::GetVertex2(INT index)
    {
#ifdef _SRE_DEBUG_
	    if(index >= m_vertexNum || index < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return VERTEX2();
        }
#endif

        BYTE* v = m_vertexes + index*m_vertexSize;
        return *(VERTEX2*)v;
    }

    VERTEX3 VertexBuffer::GetVertex3(INT index)
    {
#ifdef _SRE_DEBUG_
	    if(index >= m_vertexNum || index < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return VERTEX3();
        }
#endif

        BYTE* v = m_vertexes + index*m_vertexSize;
        return *(VERTEX3*)v;
    }

    VERTEX4 VertexBuffer::GetVertex4(INT index)
    {
#ifdef _SRE_DEBUG_
	    if(index >= m_vertexNum || index < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return VERTEX4();
        }
#endif

        BYTE* v = m_vertexes + index*m_vertexSize;
        return *(VERTEX4*)v;
    }




    //=============================
	//CreateVertexBuffer functions
	//
	//
	//=============================
    RESULT CreateVertexBuffer(INT vertexNumber, INT vertexSize, SREVAR dataFormat, void* vertexes, VertexBuffer** out)
    {
        if(vertexNumber <= 0) return RESULT::INVALIDARG;
        if(vertexSize <= 0) return RESULT::INVALIDARG;
        if(nullptr == vertexes) return RESULT::INVALIDARG;

        INT vertexDimen=0, attriSize=0;
		if((dataFormat & SRE_FORMAT_VERTEX_XY)==SRE_FORMAT_VERTEX_XY)
			vertexDimen = 2;
		else if((dataFormat & SRE_FORMAT_VERTEX_XYZ)==SRE_FORMAT_VERTEX_XYZ)
			vertexDimen = 3;
		else if((dataFormat & SRE_FORMAT_VERTEX_XYZW)==SRE_FORMAT_VERTEX_XYZW)
			vertexDimen = 4;
        else
			return RESULT::INVALIDARG;

		attriSize = vertexSize - vertexDimen*sizeof(FLOAT);
		if(attriSize<0) return RESULT::FAIL;

        VertexBuffer* buffer=nullptr;
        buffer = new VertexBuffer();
        if(nullptr == *out) return RESULT::OUTMEMORY;

		INT length = vertexNumber * vertexSize;
		buffer->m_vertexes = new BYTE[length];
		if(nullptr == buffer->m_vertexes) return RESULT::OUTMEMORY;
		memcpy(buffer->m_vertexes, vertexes, length);

		buffer->m_marks = new bool[vertexNumber];
		if(nullptr == buffer->m_marks) return RESULT::OUTMEMORY;
		memset(buffer->m_marks, 0, vertexNumber*sizeof(bool));

		buffer->m_vertexDimen = vertexDimen;
		buffer->m_attriSize = attriSize;
        buffer->m_vertexFormat = dataFormat;
        buffer->m_vertexSize = vertexSize;
        buffer->m_vertexNum = vertexNumber;

        *out = buffer;

        return RESULT::SUCC;
    }

	RESULT CreateVertexBuffer(INT vertexNumber, INT vertexSize, SREVAR dataFormat, void* vertexes, VertexBuffer* out)
    {
        if(vertexNumber <= 0) return RESULT::INVALIDARG;
        if(vertexSize <= 0) return RESULT::INVALIDARG;
        if(nullptr == vertexes) return RESULT::INVALIDARG;
        if(nullptr == out) return RESULT::INVALIDARG;

		if((dataFormat & SRE_FORMAT_VERTEX_XY)==SRE_FORMAT_VERTEX_XY)
			out->m_vertexDimen = 2;
		else if((dataFormat & SRE_FORMAT_VERTEX_XYZ)==SRE_FORMAT_VERTEX_XYZ)
			out->m_vertexDimen = 3;
		else if((dataFormat & SRE_FORMAT_VERTEX_XYZW)==SRE_FORMAT_VERTEX_XYZW)
			out->m_vertexDimen = 4;
        else
			return RESULT::INVALIDARG;

		out->m_attriSize = vertexSize - out->m_vertexDimen*sizeof(FLOAT);
		if(out->m_attriSize<0) return RESULT::FAIL;

		INT length = vertexNumber * vertexSize;
		out->m_vertexes = new BYTE[length];
		if(nullptr == out->m_vertexes) return RESULT::OUTMEMORY;
		memcpy(out->m_vertexes, vertexes, length);

		out->m_marks = new bool[vertexNumber];
		if(nullptr == out->m_marks) return RESULT::OUTMEMORY;
		memset(out->m_marks, 0, vertexNumber*sizeof(bool));

        out->m_vertexFormat = dataFormat;
        out->m_vertexSize = vertexSize;
        out->m_vertexNum = vertexNumber;

        return RESULT::SUCC;
    }




    //=============================
	//RenderTexture functions
	//
	//
	//=============================
	RenderTexture::RenderTexture(const RenderTexture & other):
       m_mutex(),
       m_cond(),
       m_lock(false),
	    m_width(other.m_width),
	    m_height(other.m_height),
	    m_pdata(nullptr),
	    m_byteCount(other.m_byteCount)
	{
	    m_pdata = new RTCOLOR[m_width*m_height];
	    if(nullptr == m_pdata) return;

	    if(nullptr != other.m_pdata)
	       memcpy(m_pdata, other.m_pdata, m_width*m_height*sizeof(RTCOLOR));
	}

	RenderTexture & RenderTexture::operator=(const RenderTexture & other)
	{
        if(this == &other) return *this;

        if(m_width != other.m_width || m_height != other.m_height || m_byteCount != other.m_byteCount)
        {
            delete[] m_pdata;
            m_pdata = new RTCOLOR[other.m_width*other.m_height];
	        if(nullptr == m_pdata) return *this;
        }

        memcpy(m_pdata, other.m_pdata, other.m_width*other.m_height*sizeof(RTCOLOR));
        m_width = other.m_width;
        m_height = other.m_height;
        m_byteCount = other.m_byteCount;

        return *this;
	}

    RESULT RenderTexture::Create(USINT width, USINT height)
    {
        if(width <=0) return  RESULT::INVALIDARG;
        if(height <=0) return  RESULT::INVALIDARG;

        /*
        if(Format == SRE_FORMAT_PIXEL_R8G8B8)
        {
            m_byteCount = 3;
        }
        else(Format == SRE_FORMAT_PIXEL_R8G8B8A8)
        {
            m_byteCount = 4;
        }
        else
        {
            return RESULT::INVALIDARG;
        }*/
        m_byteCount = sizeof(RTCOLOR);

        if(nullptr != m_pdata) delete[] m_pdata;

        m_pdata = new RTCOLOR[width*height];
        if(nullptr == m_pdata) return RESULT::OUTMEMORY;

        m_width = width;
        m_height = height;

        return RESULT::SUCC;
    }

    RESULT RenderTexture::Resize(USINT width, USINT height)
    {
        if(width <=0) return  RESULT::INVALIDARG;
        if(height <=0) return  RESULT::INVALIDARG;

        std::unique_lock<std::mutex> lock(m_mutex);
        m_cond.wait(lock, [this]{return !m_lock;});

        if(nullptr != m_pdata)
		   	delete[] m_pdata;

        if(nullptr != m_pdata) delete[] m_pdata;

        m_pdata = new RTCOLOR[width*height];
        if(nullptr == m_pdata) return RESULT::OUTMEMORY;

        m_width = width;
        m_height = height;

        return RESULT::SUCC;
    }


    void  RenderTexture::DrawSquare(INT sx, INT sy, INT ex, INT ey, RTCOLOR color)
    {
		for(USINT py=sy; py<=ey; py++)
            for(USINT px=sx; px<=ex; px++)
			{
				*(m_pdata+py*m_width+px) = color;
			}
    }


	 void  RenderTexture::Clear(RTCOLOR color)
    {
    	INT size=m_width*m_height;
		for(INT i=0; i<size; i++)
				*(m_pdata+i) = color;
    }

    void RenderTexture::Lock()
    {
       std::unique_lock<std::mutex> lock(std::mutex);
       m_lock = true;
    }

    void RenderTexture::Unlock()
    {
       std::unique_lock<std::mutex> lock(std::mutex);
       m_lock = false;
       m_cond.notify_one();
    }

    RTCOLOR* RenderTexture::Wait_Get()
    {
       std::unique_lock<std::mutex> lock(m_mutex);
       m_cond.wait(lock, [this]{return !m_lock;});

       return m_pdata;
    }

}

