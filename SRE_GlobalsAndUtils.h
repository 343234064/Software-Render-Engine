//*****************************************************
//
// Software Render Engine
// Version 0.01
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

#include <memory>
using std::unique_ptr;
using std::shared_ptr;


namespace SREngine {
    //==============================
    //Deleter
    //
    //Use for smart pointer
    //==============================
    template<class T>
    struct array_deleter
    {
        void operator()(T* & x) const { delete[] x; }
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

    class BufferDescript;

    template <typename T>
	class Buffer;

    class BaseMesh;
    class TriangleMesh;
	class BaseMeshManager;
	class TriangleMeshManager;

    class Color3;
    class Color4;


    class BasicInput;
    class BasicOutput;
    class BasicProcessor;
    class BasePileLineBuilder;

    class PileLineBuilder;
    class InputAssembler;
    class VertexPostProcesser;
    class Rasterizer;
    class OutputMerger;


    class RenderStates;
    class VariableBuffer;
    class RunTimeData;
    class Technique;
    class RenderPass;

    class VSInput;
    class VertexShader;
    class PixelShader;
    //==============================
    //Type definitions
    //
    //==============================
    typedef float         FLOAT;
    typedef int             INT;
    typedef unsigned int  SREVAR;
    typedef unsigned char BYTE;

    typedef Vector    VEC;
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

    typedef unique_ptr<VERTEX4, array_deleter<VERTEX4>> unique_vertex4_array;
    typedef unique_ptr<INT, array_deleter<INT>>         unique_int_array;
    typedef unique_ptr<BYTE, array_deleter<BYTE>>       unique_byte_array;
    typedef unique_ptr<unique_int_array, array_deleter<unique_int_array>> unique_int_matrix;

    typedef BasicOutput* (CallBackVShader)(const BasicInput &);
    typedef BasicOutput* (CallBackPShader)(const BasicInput &);


    //==============================
    //Global variables
    //
    //==============================
    const FLOAT EPSILON = 0.000001;
    const FLOAT PI      = 3.1415926;

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
    const SREVAR SRE_FORMAT_VERTEX_XYZ=0x00000003;
    /*
      Each vertex has x ,y ,z and w member, this format will be treated as
    a transformed vertex.
      It must be a 4 floating point data
    */
    const SREVAR SRE_FORMAT_VERTEX_XYZW=0x00000007;

    /*
      Vertex normal, 3 floating point data
    */
    const SREVAR SRE_FORMAT_ATTRIBUTE_NORMAL=0x00000008;
    /*
      Vertex diffuse color, in ARGB order,3 floating point data
    */
    const SREVAR SRE_FORMAT_ATTRIBUTE_DIFFUSE=0x00000010;
    /*
      Vertex specular color, in ARGB order,3 floating point data
    */
    const SREVAR SRE_FORMAT_ATTRIBUTE_SPECULAR=0x00000020;
    /*
      Vertex bi-normal, 3 floating point data
    */
    const SREVAR SRE_FORMAT_ATTRIBUTE_BINORMAL=0x00000040;
    /*
      Vertex tangent, 3 floating point data
    */
    const SREVAR SRE_FORMAT_ATTRIBUTE_TANGENT=0x00000080;
    /*
      Vertex texture coordinate set , in UV order
    */
    const SREVAR SRE_FORMAT_ATTRIBUTE_TEXCOORD1=0x00000100;
    const SREVAR SRE_FORMAT_ATTRIBUTE_TEXCOORD2=0x00000200;
    const SREVAR SRE_FORMAT_ATTRIBUTE_TEXCOORD3=0x00000400;
    const SREVAR SRE_FORMAT_ATTRIBUTE_TEXCOORD4=0x00000800;
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

    const SREVAR SRE_BUFFERTYPE_RENDERBUFFER=0x10000060;

    const SREVAR SRE_MATRIXTYPE_WORLD=0x10000070;
    const SREVAR SRE_MATRIXTYPE_VIEW=0x10000071;
    const SREVAR SRE_MATRIXTYPE_PROJECT=0x10000072;
    const SREVAR SRE_MATRIXTYPE_WORLDVIEW=0x10000073;
    const SREVAR SRE_MATRIXTYPE_VIEWPROJECT=0x10000074;
    const SREVAR SRE_MATRIXTYPE_WORLDVIEWPROJECT=0x10000075;












    /*
      Primitive types
      POINTLIST: a list of points
      LINELIST: a list of lines, write a index -1 means the end of a line
      TRIANGLEFAN: a list of triangle fans, write a index -1 means the end of a fan
      TRIANGLESTRIP: a list of continuous triangles, which shared an edge with the adjacent one,
                     write a index -1 means the end of a strip.
      TRIANGLELIST: a list of triangles
    */
    const SREVAR SRE_PRIMITIVETYPE_POINTLIST=0x00000300;
    const SREVAR SRE_PRIMITIVETYPE_LINELIST=0x00000301;
    const SREVAR SRE_PRIMITIVETYPE_TRIANGLEFAN=0x00000302;
    const SREVAR SRE_PRIMITIVETYPE_TRIANGLESTRIP=0x00000303;
    const SREVAR SRE_PRIMITIVETYPE_TRIANGLELIST=0x00000304;


    //SREVAR SRE_DRAWPIMITIVETYPE_POINT;
    //SREVAR SRE_DRAWPIMITIVETYPE_LINE;
    //SREVAR SRE_DRAWPIMITIVETYPE_FACE;



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
        BaseMesh():name("\0"){}
        BaseMesh(std::string _name):name(_name){}
        virtual ~BaseMesh(){}

        void         SetName(std::string _name){name=_name;}
        std::string  getName(){return name;}

    protected:
        std::string  name;

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


        virtual void * GetVertex(INT vertexIndex)=0;
        virtual void * GetVertexFromFace(INT faceIndex, INT index){return nullptr;}
        virtual void * GetVertexFromEdge(INT edgeIndex, INT index){return nullptr;}
        virtual void * GetFaceFromEdge(INT edgeIndex, INT index){return nullptr;}
        virtual void * GetEdgeFromVertex(INT vertexIndex, INT index){return nullptr;}
        virtual void * GetAttribute(INT vertexIndex){return nullptr;}
        virtual INT    GetVertexNumber()=0;
        virtual INT    GetEdgeNumber(){return 0;}
        virtual INT    GetFaceNumber(){return 0;}
        virtual void   ReleaseMesh()=0;


    };

}


#endif
