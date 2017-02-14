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

        *out = new VertexBuffer(vertexNumber, vertexSize, dataFormat, vertexes);
        if(nullptr == *out) return RESULT::OUTMEMORY;

        return RESULT::SUCC;
    }
}

