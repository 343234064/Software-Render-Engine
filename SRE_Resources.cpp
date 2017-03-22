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
        if(vertexNumber <= 0) return RESULT::FAIL;
        if(vertexSize <= 0) return RESULT::FAIL;
        if(((dataFormat & SRE_FORMAT_VERTEX_XY) != SRE_FORMAT_VERTEX_XY) &&
           ((dataFormat & SRE_FORMAT_VERTEX_XYZ) != SRE_FORMAT_VERTEX_XYZ) &&
           ((dataFormat & SRE_FORMAT_VERTEX_XYZW) != SRE_FORMAT_VERTEX_XYZW))
           return RESULT::FAIL;
        if(nullptr == vertexes) return RESULT::FAIL;

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

		if((buffer->m_vertexFormat & SRE_FORMAT_VERTEX_XY)==SRE_FORMAT_VERTEX_XY)
			buffer->m_vertexDimen = 2;
		else if((buffer->m_vertexFormat & SRE_FORMAT_VERTEX_XYZ)==SRE_FORMAT_VERTEX_XYZ)
			buffer->m_vertexDimen = 3;
		else if((buffer->m_vertexFormat & SRE_FORMAT_VERTEX_XYZW)==SRE_FORMAT_VERTEX_XYZW)
			buffer->m_vertexDimen = 4;

		buffer->m_attriSize = vertexSize - buffer->m_vertexDimen*sizeof(FLOAT);

		*out = buffer;

        return RESULT::SUCC;
    }

	RESULT CreateVertexBuffer(INT vertexNumber, INT vertexSize, SREVAR dataFormat, void* vertexes, VertexBuffer* out)
    {
        if(vertexNumber <= 0) return RESULT::FAIL;
        if(vertexSize <= 0) return RESULT::FAIL;
        if(((dataFormat & SRE_FORMAT_VERTEX_XY) != SRE_FORMAT_VERTEX_XY) &&
           ((dataFormat & SRE_FORMAT_VERTEX_XYZ) != SRE_FORMAT_VERTEX_XYZ) &&
           ((dataFormat & SRE_FORMAT_VERTEX_XYZW) != SRE_FORMAT_VERTEX_XYZW))
           return RESULT::FAIL;
        if(nullptr == vertexes) return RESULT::FAIL;
        if(nullptr == out) return RESULT::FAIL;

		INT length = vertexNumber * vertexSize;
		out->m_vertexes = new BYTE[length];
		if(nullptr == out->m_vertexes) return RESULT::OUTMEMORY;
		memcpy(out->m_vertexes, vertexes, length);

		out->m_marks = new bool[vertexNumber];
		if(nullptr == out->m_marks) return RESULT::OUTMEMORY;
		memset(out->m_marks, 0, vertexNumber*sizeof(bool));

		if((out->m_vertexFormat & SRE_FORMAT_VERTEX_XY)==SRE_FORMAT_VERTEX_XY)
			out->m_vertexDimen = 2;
		else if((out->m_vertexFormat & SRE_FORMAT_VERTEX_XYZ)==SRE_FORMAT_VERTEX_XYZ)
			out->m_vertexDimen = 3;
		else if((out->m_vertexFormat & SRE_FORMAT_VERTEX_XYZW)==SRE_FORMAT_VERTEX_XYZW)
			out->m_vertexDimen = 4;

		out->m_attriSize = vertexSize - out->m_vertexDimen*sizeof(FLOAT);
        return RESULT::SUCC;
    }




    //=============================
	//RenderTexture functions
	//
	//
	//=============================
	RenderTexture::RenderTexture(const RenderTexture & other):
        Format(other.Format),
	    Width(other.Width),
	    Height(other.Height),
	    m_pdata(nullptr),
	    m_byteCount(other.m_byteCount)
	{
	    m_pdata = new BYTE[Width*Height*m_byteCount];
	    if(nullptr == m_pdata) return;

	    memcpy(m_pdata, other.m_pdata, Width*Height*m_byteCount);
	}

	RenderTexture & RenderTexture::operator=(const RenderTexture & other)
	{
        if(this == &other) return *this;

        if(Width != other.Width || Height != other.Height || m_byteCount != other.m_byteCount)
        {
            delete[] m_pdata;
            m_pdata = new BYTE[Width*Height*m_byteCount];
	        if(nullptr == m_pdata) return *this;
        }

        memcpy(m_pdata, other.m_pdata, other.Width*other.Height*other.m_byteCount);
        Format = other.Format;
        Width = other.Width;
        Height = other.Height;
        m_byteCount = other.m_byteCount;

        return *this;
	}

    RESULT RenderTexture::Create()
    {
        if(Width <=0) return  RESULT::INVALIDARG;
        if(Height <=0) return  RESULT::INVALIDARG;

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
        m_byteCount = 4;

        if(nullptr != m_pdata) delete[] m_pdata;

        m_pdata = new BYTE[Width*Height*m_byteCount];
        if(nullptr == m_pdata) return RESULT::OUTMEMORY;

        return RESULT::SUCC;
    }

    BYTE*   RenderTexture::CopyTo(BYTE* dest, INT destOffset)
    {
    	if(nullptr == dest) return nullptr;
        return (BYTE*)memcpy(dest+destOffset, m_pdata, Width*Height*m_byteCount);
    }

    /*
    void  RenderTexture::Draw(USINT px, USINT py, BYTE* color)
    {
        memcpy(m_pdata+py*Width*m_byteCount + px*m_byteCount, color, m_byteCount);
    }*/


    void  RenderTexture::DrawSquare(USINT sx, USINT sy, USINT ex, USINT ey, Color4 color)
    {
        Color4* addr = (Color4*)m_pdata;
        for(USINT py=sy; sy<=ey; sy++)
            for(USINT px=sx; sx<=ex; sx++)
                *(addr+py*Width+px) = color;
    }


}

