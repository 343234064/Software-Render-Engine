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
    void VertexBuffer::GetAttributes(INT pos, BYTE** output)
    {
#ifdef _SRE_DEBUG_
	    if(pos >= m_vertexNum|| pos < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return;
        }
#endif

        *output = new BYTE[m_attriSize];
        if(nullptr == *output) return;
        memcpy(*output, m_vertexes + pos*m_vertexSize + m_vertexSize - m_attriSize, m_attriSize);

    }

    FLOAT VertexBuffer::GetVertexX(INT pos)
    {
#ifdef _SRE_DEBUG_
	    if(pos >= m_vertexNum || pos < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return 0.0f;
        }
#endif

        BYTE* val = m_vertexes + pos*m_vertexSize;
        return *((FLOAT*)val);
    }

    FLOAT VertexBuffer::GetVertexY(INT pos)
    {
#ifdef _SRE_DEBUG_
	    if(pos >= m_vertexNum || pos < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return 0.0f;
        }
#endif

        BYTE* val = m_vertexes + pos*m_vertexSize + sizeof(FLOAT);
        return *((FLOAT*)val);
    }

    FLOAT VertexBuffer::GetVertexZ(INT pos)
    {
#ifdef _SRE_DEBUG_
	    if(pos >= m_vertexNum || pos < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return 0.0f;
        }
#endif

        BYTE* val = m_vertexes + pos*m_vertexSize + 2*sizeof(FLOAT);
        return *((FLOAT*)val);
    }

    FLOAT VertexBuffer::GetVertexW(INT pos)
    {
#ifdef _SRE_DEBUG_
	    if(pos >= m_vertexNum || pos < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return 0.0f;
        }
#endif
        BYTE* val = m_vertexes + pos*m_vertexSize + 3*sizeof(FLOAT);
        return *((FLOAT*)val);
    }

    VERTEX2 VertexBuffer::GetVertex2(INT pos)
    {
#ifdef _SRE_DEBUG_
	    if(pos >= m_vertexNum || pos < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return VERTEX2();
        }
#endif

        BYTE* v = m_vertexes + pos*m_vertexSize;
        return *(VERTEX2*)v;
    }

    VERTEX3 VertexBuffer::GetVertex3(INT pos)
    {
#ifdef _SRE_DEBUG_
	    if(pos >= m_vertexNum || pos < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return VERTEX3();
        }
#endif

        BYTE* v = m_vertexes + pos*m_vertexSize;
        return *(VERTEX3*)v;
    }

    VERTEX4 VertexBuffer::GetVertex4(INT pos)
    {
#ifdef _SRE_DEBUG_
	    if(pos >= m_vertexNum || pos < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return VERTEX4();
        }
#endif

        BYTE* v = m_vertexes + pos*m_vertexSize;
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

        *out = new VertexBuffer(vertexNumber, vertexSize, dataFormat, vertexes);
        if(nullptr == *out) return RESULT::OUTMEMORY;

        return RESULT::SUCC;
    }
}

