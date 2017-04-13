//*****************************************************
//
// Software Render Engine
// Version 0.01 by XJL
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
   BYTE* VertexBuffer::GetVertexAddr(INT index)
	{
#ifdef _SRE_DEBUG_
	    if(index >= m_vertexNum|| index < 0)
        {
            _ERRORLOG(SRE_ERROR_INVALIDARG);
            return nullptr;
        }
#endif
        return m_vertexes + index*m_vertexSize;
	}

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

        out->m_vertexFormat = dataFormat;
        out->m_vertexSize = vertexSize;
        out->m_vertexNum = vertexNumber;

        return RESULT::SUCC;
    }


   //=============================
	//Texture functions
	//
	//=============================
    void   Texture::Set(BYTE* image, USINT width, USINT height, USINT bytesPerColor)
    {
       if(nullptr == image) return;
       if(width <=0 || height <=0 ) return;
       if(bytesPerColor != 1 && bytesPerColor != 3 && bytesPerColor != 4) return;

       if(nullptr != m_data) delete[] m_data;

       m_data = image;
       m_width = width;
       m_height = height;
       m_bytesPerColor = bytesPerColor;

       return;
    }


   //=============================
	//RenderTexture functions
	//
	//
	//=============================
	RenderTexture::RenderTexture(const RenderTexture & other):
       Texture(other.m_width, other.m_height, other.m_bytesPerColor, nullptr),
       m_mutex(),
       m_cond(),
       m_lock(false),
	    m_pdata(nullptr)
	{
	    m_pdata = new RTCOLOR[m_width*m_height];
	    if(nullptr == m_pdata) return;

	    if(nullptr != other.m_pdata)
	       memcpy(m_pdata, other.m_pdata, m_width*m_height*sizeof(RTCOLOR));

       m_data = (BYTE*)m_pdata;
	}

	RenderTexture & RenderTexture::operator=(const RenderTexture & other)
	{
        if(this == &other) return *this;

        if(m_width != other.m_width || m_height != other.m_height || m_bytesPerColor != other.m_bytesPerColor)
        {
            delete[] m_pdata;
            m_pdata = new RTCOLOR[other.m_width*other.m_height];
	        if(nullptr == m_pdata) return *this;
        }

        memcpy(m_pdata, other.m_pdata, other.m_width*other.m_height*sizeof(RTCOLOR));
        m_width = other.m_width;
        m_height = other.m_height;
        m_bytesPerColor = other.m_bytesPerColor;

        m_data = (BYTE*)m_pdata;
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
        m_bytesPerColor = sizeof(RTCOLOR);

        if(nullptr != m_pdata) delete[] m_pdata;

        m_pdata = new RTCOLOR[width*height];
        if(nullptr == m_pdata) return RESULT::OUTMEMORY;

        m_width = width;
        m_height = height;

        m_data = (BYTE*)m_pdata;

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

        m_data = (BYTE*)m_pdata;

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





   //=============================
	//Class Sampler functions
	//=============================
    Color3 Sampler::getcolor3(Texture* tex, FLOAT u, FLOAT v)
    {
#ifdef _SRE_DEBUG_
         if(nullptr == tex)
         {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return Color3();
         }

         if(nullptr == tex->m_data)
         {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return Color3();
         }
#endif

         USINT px = u*tex->m_width;
         USINT py = v*tex->m_height;

         if(px>=tex->m_width) px = tex->m_width-1;
         else if(px<0) px = 0;
         if(py>=tex->m_height) py = tex->m_height-1;
         else if(py<0) py = 0;

         Color3 color;
         Color3* buffer = (Color3*)(tex->m_data);
         if(FilterMode == SRE_FILTERMODE_NEAREST)
         {
             color = *(buffer + py*tex->m_width + px);
             return color;
         }
         else if(FilterMode == SRE_FILTERMODE_BILINEAR)
         {
             FLOAT _u = u - (INT)u;
             FLOAT _v = v - (INT)v;

             Color3 *add1, *add2, *add3, *add4;

             if(_u<=0.5f)
             {
                if(_v>0.5f)
                {
                    add2 =  buffer + py*tex->m_width + px;
                    add1 =  (py<tex->m_height-1) ? add2 - tex->m_width: add2;
                }
                else
                {
                    add1 =  buffer + py*tex->m_width + px;
                    add2 =  (py<tex->m_height -1) ? add1 - tex->m_width: add1;
                }

                if(px>=1)
                {
                    add3 =  add2 - 1;
                    add4 =  add1 - 1;
                }
                else
                {
                    add3 = add2;
                    add4 = add1;
                }
             }
             else
             {
                if(_v>0.5f)
                {
                    add3 =  buffer + py*tex->m_width + px;
                    add4 =  (py < tex->m_height - 1)? add3 - tex->m_width: add3;
                }
                else
                {
                    add4 =  buffer + py*tex->m_width + px;
                    add3 =  (py < tex->m_height - 1)? add4 - tex->m_width: add4;
                }

                if(px< tex->m_width-1)
                {
                    add2 =  add3 + 1;
                    add1 =  add4 + 1;
                }
                else
                {
                    add2 =  add3 + 1;
                    add1 =  add4 + 1;
                }
             }

             color = (*(add4))*(1.0f-_u)*(1.0f-_v) + (*(add1))*_u*(1.0f-_v) +
                         (*(add3))*(1.0f-_u)*_v  + (*(add2))*_u*_v;

             return color;
         }

         return Color3();
    }

    Color4 Sampler::getcolor4(Texture* tex, FLOAT u, FLOAT v)
    {
#ifdef _SRE_DEBUG_
         if(nullptr == tex)
         {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return Color4();
         }

         if(nullptr == tex->m_data)
         {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return Color4();
         }
#endif

         USINT px = u*tex->m_width;
         USINT py = v*tex->m_height;

         if(px>=tex->m_width) px = tex->m_width-1;
         else if(px<0) px = 0;
         if(py>=tex->m_height) py = tex->m_height-1;
         else if(py<0) py = 0;

         Color4 color;
         Color4* buffer = (Color4*)(tex->m_data);
         if(FilterMode == SRE_FILTERMODE_NEAREST)
         {
             color = *(buffer + py*tex->m_width + px);
             return color;
         }
         else if(FilterMode == SRE_FILTERMODE_BILINEAR)
         {
             FLOAT _u = u - (INT)u;
             FLOAT _v = v - (INT)v;

             Color4 *add1, *add2, *add3, *add4;

             if(_u<=0.5f)
             {
                if(_v>0.5f)
                {
                    add2 =  buffer + py*tex->m_width + px;
                    add1 =  (py<tex->m_height-1) ? add2 - tex->m_width: add2;
                }
                else
                {
                    add1 =  buffer + py*tex->m_width + px;
                    add2 =  (py<tex->m_height -1) ? add1 - tex->m_width: add1;
                }

                if(px>=1)
                {
                    add3 =  add2 - 1;
                    add4 =  add1 - 1;
                }
                else
                {
                    add3 = add2;
                    add4 = add1;
                }
             }
             else
             {
                if(_v>0.5f)
                {
                    add3 =  buffer + py*tex->m_width + px;
                    add4 =  (py < tex->m_height - 1)? add3 - tex->m_width: add3;
                }
                else
                {
                    add4 =  buffer + py*tex->m_width + px;
                    add3 =  (py < tex->m_height - 1)? add4 - tex->m_width: add4;
                }

                if(px< tex->m_width-1)
                {
                    add2 =  add3 + 1;
                    add1 =  add4 + 1;
                }
                else
                {
                    add2 =  add3 + 1;
                    add1 =  add4 + 1;
                }
             }

             color = (*(add4))*(1.0f-_u)*(1.0f-_v) + (*(add1))*_u*(1.0f-_v) +
                         (*(add3))*(1.0f-_u)*_v  + (*(add2))*_u*_v;

             return color;
         }

         return Color4();
    }


}

