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

#include <iostream>
using std::cout;
using std::endl;
namespace SREngine {
    //==============================
    //Deleter
    //
    //Use for smart pointer
    //==============================
    template<class T>
    struct array_deleter
    {
        void operator()(T* &x) const { delete[] x; }
    };

    //==============================
    //Classes definitions
    //
    //==============================
    class IContainer;

	class Vector;
	class Vector2;
	class Vector3;
	class Vector4;
	class Matrix;
	class Matrix3x3;
	class Matrix4x4;
    class Quaternion;

    class BufferDescript;
	class Buffer;

    class IMesh;
    class TriangleMesh;
	class IMeshManager;
	class TriangleMeshManager;

    class Color3;
    class Color4;

    //==============================
    //Type definitions
    //
    //==============================
    typedef float         FLOAT;
    typedef int             INT;
    typedef unsigned int  SREVAR;
    typedef unsigned char BYTE;
    //考虑添加short int来作为索引数据类型
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


    const SREVAR SRE_FORMAT_PIXEL_R8G8B8A8=0x00000100;
    const SREVAR SRE_FORMAT_PIXEL_R8G8B8=0x00000101;

    const SREVAR SRE_BUFFERTYPE_RENDERBUFFER=0x00000200;



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
    //Class IContainer
    //
    //Basic class
    //==============================
    class IContainer
    {
    public:
        IContainer(){}
        virtual ~IContainer(){}
        virtual void Clear() = 0;

    };






}


#endif
