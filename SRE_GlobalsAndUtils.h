//*****************************************************
//
// Software Render Engine
// Version 0.01 by XJL
//
// File: SRE_GlobalsAndUtils.h
// Date: 2016/5/04
// Description:
//       Defines global variables and other basic classes
//       and functions
//
//
//*****************************************************
#ifndef _SRE_GLOBALSANDUTILS_
#define _SRE_GLOBALSANDUTILS_

//==============================
//For debug use
//==============================
#define _SRE_DEBUG_
//==============================
#ifdef _SRE_DEBUG_
#include "SRE_DebugLog.h"
#include <iostream>
using std::cout;
using std::endl;
#endif

//=============================
//8 bit per channel
//=============================
//=============================
//Little endian ) B, G, R, A
//=============================
#define _BGRA_OREDER_
//=============================
//BIG endian R, G, B, A
//=============================
//#define _RGBA_OREDER_


#define _SRE_PLATFORM_WIN_
#ifdef _SRE_PLATFORM_WIN_
#include <windows.h>
/************************************
 *if compile in visual studio, add
 *the following code here:
 *
 *   #pragma comment(lib, "winmm.lib")
 *
 *else if compile in codeblocks, add
 *libgdi32.a to the linker:
 *Setting -> Compiler ->Linker Setting-> Add:
 *
 *   CodeBlocks\MinGW\lib\libgdi32.a
 *
 ************************************/
#endif

#include <math.h>
#include <string.h>
#include <map>
#include <list>
#include <queue>
#include <thread>
#include <mutex>
#include <exception>
#include <memory>
#include <stdlib.h>
#include <condition_variable>
#include <fstream>
#include <sstream>


namespace SRE {
    //==============================
    //structure type
    //
    //
    //==============================
    template<class T>
    struct array_deleter
    {
        void operator()(T* & x) const { delete[] x; }
    };

    struct empty_exception:std::exception
	{
	    const char* what() const throw();
	};


    //==============================
    //Classes definitions
    //
    //==============================
    class BaseContainer;
    class BaseTask;

	 class Vector;
	 class Vector2;
	 class Vector3;
	 class Vector4;
	 class Matrix;
	 class Matrix3x3;
	 class Matrix4x4;
    class Quaternion;

    class BaseMesh;
    class Mesh;
    class TMesh;
	 class BaseMeshManager;
	 class TMeshManager;

    class Color3;
    class Color4;

    template <typename T>
	 class Buffer;
    class VertexBuffer;
    class Texture;
    class RenderTexture;
    class Sampler;

    template<typename T>
    class BasicIOBuffer;
    template<typename T>
    class BasicIOBufferEx;

    class BasicIOElement;
    class BasicObserver;
    class CallBackFunctions;
    class BasicProcessor;
    class BasePipeLine;

    class InputAssembler;
    class VertexProcessor;
    class VertexPostProcessor;
    class PrimitiveAssembler;
    class Rasterizer;
    class PixelProcessor;
    class SREPipeLine;
    //class OutputMerger;

    class VariableBuffer;
    class ConstantBuffer;

    class VSOutput;
    class PSInput;
    class VertexShader;
    class PixelShader;
    class Technique;
    class RenderPass;

    class Device;
    class DeviceAdapter;

    //==============================
    //Type definitions
    //
    //==============================
    typedef float                       FLOAT;
    typedef int                           INT;
    typedef short int                 SINT;
    typedef unsigned short int USINT;
    typedef unsigned int           SREVAR;
    typedef unsigned char        BYTE;

    typedef Vector     VEC;
	 typedef Vector2   VEC2;
	 typedef Vector3   VEC3;
	 typedef Vector4   VEC4;
	 typedef Vector  * PVEC;
	 typedef Vector2 * PVEC2;
	 typedef Vector3 * PVEC3;
	 typedef Vector4 * PVEC4;
	 typedef const Vector  * CPVEC;
	 typedef const Vector2 * CPVEC2;
	 typedef const Vector3 * CPVEC3;
	 typedef const Vector4 * CPVEC4;

	 typedef Vector2 VERTEX2;
    typedef Vector3 VERTEX3;
	 typedef Vector4 VERTEX4;
    typedef Vector2 * PVERTEX2;
    typedef Vector3 * PVERTEX3;
	 typedef Vector4 * PVERTEX4;
	 typedef const Vector2 * CPVERTEX2;
	 typedef const Vector3 * CPVERTEX3;
	 typedef const Vector4 * CPVERTEX4;

	 typedef Matrix3x3 MAT33;
	 typedef Matrix4x4 MAT44;
	 typedef Matrix3x3 * PMAT33;
	 typedef Matrix4x4 * PMAT44;
	 typedef const Matrix3x3 * CPMAT33;
	 typedef const Matrix4x4 * CPMAT44;

	 typedef Quaternion    QUAT;
    typedef Quaternion * PQUAT;
    typedef const Quaternion * CPQUAT;

    typedef std::unique_ptr<VERTEX4, array_deleter<VERTEX4>>                        unique_vertex4_array;
    typedef std::unique_ptr<INT, array_deleter<INT>>                                        unique_int_array;
    typedef std::unique_ptr<BYTE, array_deleter<BYTE>>                                   unique_byte_array;
    typedef std::unique_ptr<unique_int_array, array_deleter<unique_int_array>> unique_int_matrix;

    typedef VSOutput* (CallBackVShader)(BYTE*, VariableBuffer*);
    typedef Color4      (CallBackPShader)(PSInput&);

    typedef TMeshManager TMManager;

    typedef BasicIOBuffer<BasicIOElement> PIOBUFFER;

    typedef Color4 DECOLOR;
    typedef Color4 RTCOLOR;
    typedef Buffer<FLOAT> ZBUFFER;
    typedef Buffer<USINT>   IBUFFER;
    typedef VertexBuffer      VBUFFER;
    typedef Color4  VCOLOR;

    //==============================
    //Global variables
    //
    //==============================
    const FLOAT EPSILON = 0.000001;
    const FLOAT PI          = 3.1415926;

    const int INDEX_END_FLAG = -1;

    enum class RESULT { SUCC, FAIL, INVALIDARG, OUTMEMORY };


    //===========================================================
    //Vertex format
    //The program will read user-defined vertex data in this order:
    //position > normal > diffuse > specular > bi-normal >
    //tangent > texcoord(1-4)
    //===========================================================
    /*
      Each vertex has x and y member, ,this format will be treated as
    a screen position of the vertex, that's say, a vertex which uses this
    format will be directly delivered to screen transform part.
      It must includes 2 floating point data
    */
    const SREVAR SRE_FORMAT_VERTEX_XY=0x00000001;
    /*
      Each vertex has x ,y and z member., this format will be treated as
    a non-transformed vertex.
      It must be a 3 floating point data
    */
    const SREVAR SRE_FORMAT_VERTEX_XYZ=0x00000002;
    /*
      Each vertex has x ,y ,z and w member, this format will be treated as
    a transformed vertex.
      It must be a 4 floating point data
    */
    const SREVAR SRE_FORMAT_VERTEX_XYZW=0x00000004;
   /*
      Vertex texture coordinate set , in UV order
    */
    const SREVAR SRE_FORMAT_ATTRIBUTE_TEXCOORDUV=0x00000008;
    const SREVAR SRE_FORMAT_ATTRIBUTE_TEXCOORDUVW=0x0000010;
    /*
      Vertex normal, 3 floating point data
    */
    const SREVAR SRE_FORMAT_ATTRIBUTE_NORMAL=0x00000020;

    /*
      Vertex diffuse color, in ARGB order,3 floating point data
    */
    const SREVAR SRE_FORMAT_ATTRIBUTE_DIFFUSE=0x00000040;
    /*
      Vertex specular color, in ARGB order,3 floating point data
    */
    const SREVAR SRE_FORMAT_ATTRIBUTE_SPECULAR=0x00000080;
    /*
      Vertex bi-normal, 3 floating point data
    */
    const SREVAR SRE_FORMAT_ATTRIBUTE_BINORMAL=0x00000100;
    /*
      Vertex tangent, 3 floating point data
    */
    const SREVAR SRE_FORMAT_ATTRIBUTE_TANGENT=0x00000200;
    /*
      Some other vertex data
    */
    const SREVAR SRE_FORMAT_ATTRIBUTE_OTHER1=0x00000400;
    const SREVAR SRE_FORMAT_ATTRIBUTE_OTHER2=0x00000800;
    const SREVAR SRE_FORMAT_ATTRIBUTE_OTHER3=0x00001000;
    const SREVAR SRE_FORMAT_ATTRIBUTE_OTHER4=0x00002000;
    //===========================================================
    //===========================================================


    const SREVAR SRE_TRUE=0x10000000;
    const SREVAR SRE_FALSE=0x10000001;

    const SREVAR SRE_RENDERSTATE_FILLMODE=0x10000002;
    const SREVAR SRE_RENDERSTATE_CULLMODE=0x10000003;
    const SREVAR SRE_RENDERSTATE_ZENABLE=0x10000004;

    const SREVAR SRE_FILLMODE_POINT=0x10000020;
    const SREVAR SRE_FILLMODE_FRAME=0x10000021;
    const SREVAR SRE_FILLMODE_SOLID=0x10000022;

    const SREVAR SRE_CULLMODE_CW=0x10000030;
    const SREVAR SRE_CULLMODE_CCW=0x10000031;

    const SREVAR SRE_FORMAT_PIXEL_R8G8B8A8=0x10000040;
    const SREVAR SRE_FORMAT_PIXEL_R8G8B8=0x10000041;

    const SREVAR SRE_SHADERINPUTFORMAT_ALL=0x10000050;
    const SREVAR SRE_SHADERINPUTFORMAT_TEXCOORD=0x10000051;
    const SREVAR SRE_SHADERINPUTFORMAT_VERTEX=0x10000052;

    const SREVAR SRE_MATRIXTYPE_WORLD=0x10000070;
    const SREVAR SRE_MATRIXTYPE_VIEW=0x10000071;
    const SREVAR SRE_MATRIXTYPE_PROJECT=0x10000072;
    const SREVAR SRE_MATRIXTYPE_WORLDVIEW=0x10000073;
    const SREVAR SRE_MATRIXTYPE_VIEWPROJECT=0x10000074;
    const SREVAR SRE_MATRIXTYPE_WORLDVIEWPROJECT=0x10000075;

    const SREVAR SRE_MESSAGE_RUNERROR=0x10000090;
    const SREVAR SRE_MESSAGE_ENDDRAW=0x10000091;
    const SREVAR SRE_MESSAGE_ENDSCENE=0x10000092;

    const SREVAR SRE_WRAPMODE_WRAP=0x100000a0;
    const SREVAR SRE_WRAPMODE_MIRROR=0x100000a1;
    const SREVAR SRE_WRAPMODE_CLAMP=0x100000a3;
    const SREVAR SRE_WRAPMODE_BORDER=0x100000a4;

    const SREVAR SRE_FILTERMODE_NEAREST=0x100000b0;
    const SREVAR SRE_FILTERMODE_BILINEAR=0x100000b1;


    /*
      Primitive types
      POINTLIST: a list of points.
      LINELIST: a list of lines.
      LINESTRIP: a list of line strip, write a index -1 means the end of a strip.
      TRIANGLEFAN: a list of triangle fans, write a index -1 means the end of a fan.
      TRIANGLESTRIP: a list of continuous triangles, which shared an edge with the adjacent one,
                     write a index -1 means the end of a strip.
      TRIANGLELIST: a list of triangles
    */
    //onst SREVAR SRE_PRIMITIVETYPE_POINTLIST=0x00000300;
    //const SREVAR SRE_PRIMITIVETYPE_LINELIST=0x00000301;
    //const SREVAR SRE_PRIMITIVETYPE_LINESTRIP=0x00000302;
    const SREVAR SRE_PRIMITIVETYPE_TRIANGLEFAN=0x00000303;
    const SREVAR SRE_PRIMITIVETYPE_TRIANGLESTRIP=0x00000304;
    const SREVAR SRE_PRIMITIVETYPE_TRIANGLELIST=0x00000305;

    //const SREVAR SRE_PRIMITIVE_POINT=0x00000400;
    //const SREVAR SRE_PRIMITIVE_LINE=0x00000401;
    //const SREVAR SRE_PRIMITIVE_TRIANGLE=0x00000402;




	//==============================
    //function declarations
    //
    //==============================
	void OutputAsImage(BYTE* colorbuffer, INT width, INT height, INT BytesPerColor);
	DECOLOR* RenderTargetToDeviceBuffer(DECOLOR* dest, const RTCOLOR* renderTarget, INT size);



    //==============================
    //Class BaseContainer
    //
    //Base class
    //==============================
    class BaseContainer
    {
    public:
        BaseContainer(){}
        virtual ~BaseContainer(){}
        //virtual void Clear() = 0;

    };


    //==============================
    //Class BaseTask
    //
    //Base class
    //==============================
    class BaseTask
    {
    public:
        BaseTask(){}
        virtual ~BaseTask(){}

        virtual void Run()=0;
    };




    //=============================
	//Class BaseMesh
	//
	//Base class
	//=============================
    class BaseMesh
    {
    public:
        BaseMesh(std::string _name="\0"):name(_name){}
        virtual ~BaseMesh(){}

    public:
        std::string name;

    };


    //=============================
	//Class IMeshManager
	//
	//Mesh class visit layer
	//Basic class
	//=============================
    class BaseMeshManager
    {
    public:
        BaseMeshManager(){}
        virtual ~BaseMeshManager(){}


    };


    //=============================
	//Class Basic I/O Element
	//
	//Just a base class,
	//User defines what need to be input/output
	//=============================
	class BasicIOElement
	{
    public:
        BasicIOElement(){}
        virtual ~BasicIOElement(){}

	};



    //=============================
	//Class CircleLinkList
	//
	//=============================
    template<typename T>
    class CLList
    {
    public:
        class LNode
        {
        public:
           LNode(T& d, LNode* n=nullptr):
             data(d), next(n)
           {}
           LNode():
             data(), next(nullptr)
           {}
           T      data;
           LNode* next;

        };
        typedef LNode* Iterator;

        CLList():
           m_head(new LNode()),
           m_tail(m_head),
           m_curr(m_head),
           m_size(0)
        {
            m_tail->next = m_head;
            m_head->next = m_tail;
        }

        ~CLList()
        {
           Clear();
           delete m_head;
        }

        void Clear()
        {
            if(m_size>0)
            {
                LNode* nextNode;
                m_curr = m_head->next;
                while(m_size>0)
                {
                    nextNode = m_curr->next;
                    delete m_curr;
                    m_curr = nextNode;
                    m_size--;
                }

                m_tail = m_head;
                m_curr = m_head;
                m_tail->next = m_head;
                m_head->next = m_tail;
            }
        }

        void Add_back(T data)
        {
            LNode* node = new LNode(data, m_head->next);
            m_tail->next = node;
            m_tail = node;
            m_size++;
        }

        Iterator Begin()
        {
            return m_curr->next;
        }

        T*   GetCurrentP()
        {
            return &(m_curr->data);
        }

        T*   GetNextP()
        {
            return &(m_curr->next->data);
        }

        T    GetCurrent()
        {
            return m_curr->data;
        }

        T    GetNext()
        {
            return m_curr->next->data;
        }

        void ResetCursor()
        {
            m_curr = m_head->next;
        }

        void Next()
        {
            m_curr = m_curr->next;
        }

        bool isEnd() const
        {
            return m_curr == m_head->next ? true : false;
        }

        bool Empty() const
        {
            return m_size<=0;
        }

        INT  Size() const
        {
            return m_size;
        }

        CLList& operator++()
        {
            m_curr = m_curr->next;
            return *this;
        }

        CLList(const CLList & other) = delete;
        CLList & operator=(const CLList & other) = delete;

    private:
        LNode* m_head;
        LNode* m_tail;
        LNode* m_curr;
        INT    m_size;

    };
}


#endif
