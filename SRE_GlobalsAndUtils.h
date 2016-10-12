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



namespace SREngine {
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
	class IMeshManager;
	class TriangleMeshManager;
    class TriangleMesh;

    //==============================
    //Type definitions
    //
    //==============================
    typedef float         FLOAT;
    typedef int             INT;
    typedef unsigned int SREVAR;

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

	typedef Vector3   COLOR3;
	typedef Vector4   COLOR4;
    typedef Vector3 * PCOLOR3;
	typedef Vector4 * PCOLOR4;
	typedef const Vector3 * CPCOLOR3;
	typedef const Vector4 * CPCOLOR4;

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


    //==============================
    //Global variables
    //
    //==============================
    const FLOAT EPSILON = 0.000001;
    const FLOAT PI      = 3.1415926;

    const int INDEX_END_FLAG = -1;

    enum RESULT { SUCC=1, FAIL=0, INVALIDARG=-1, OUTMEMORY=-2 };

    /*each vertex has x and y member, ,this format will be treated as
    a screen position of the vertex, that's say, a vertex which uses this
    format will be directly delivered to screen transform part. FLOAT FLOAT*/
    const SREVAR SRE_FORMAT_VERTEX_XY=0x00000065;
    /*each vertex has x ,y and z member. FLOAT FLOAT FLOAT*/
    const SREVAR SRE_FORMAT_VERTEX_XYZ=0x00000066;
    /*each vertex has x ,y ,z and w member, this format will be treated as
    a transformed and clipped vertex. FLOAT FLOAT FLOAT FLOAT*/
    const SREVAR SRE_FORMAT_VERTEX_XYZW=0x00000067;

    const SREVAR SRE_FORMAT_ATTRIBUTE_TEXCOORD_1=0x00000068;
    const SREVAR SRE_FORMAT_ATTRIBUTE_TEXCOORD_2=0x00000069;
    const SREVAR SRE_FORMAT_ATTRIBUTE_TEXCOORD_3=0x0000006A;
    const SREVAR SRE_FORMAT_ATTRIBUTE_NORMAL=0x0000006B;
    const SREVAR SRE_FORMAT_ATTRIBUTE_DIFFUSE=0x0000006C;
    const SREVAR SRE_FORMAT_ATTRIBUTE_SPECULAR=0x0000006D;
    const SREVAR SRE_FORMAT_ATTRIBUTE_BINORMAL=0x0000006E;
    const SREVAR SRE_FORMAT_ATTRIBUTE_TANGENT=0x0000006F;

    const SREVAR SRE_FORMAT_PIXEL_R8G8B8A8=0x00000070;
    const SREVAR SRE_FORMAT_PIXEL_R8G8B8=0x00000071;

    const SREVAR SRE_BUFFERUSAGE_ATTRIBUTEBUFFER=0x00000072;
    const SREVAR SRE_BUFFERUSAGE_RENDERBUFFER=0x00000073;



    /*
      Primitive types
      POINTLIST: a list of points
      LINELIST: a list of lines, write a index -1 means the end of a line
      TRIANGLEFAN: a list of triangle fans, write a index -1 means the end of a fan
      TRIANGLESTRIP: a list of continuous triangles, which shared an edge with the adjacent one,
                     write a index -1 means the end of a strip.
      TRIANGLELIST: a list of triangles
    */
    const SREVAR SRE_PRIMITIVETYPE_POINTLIST=0x00000074;
    const SREVAR SRE_PRIMITIVETYPE_LINELIST=0x00000075;
    const SREVAR SRE_PRIMITIVETYPE_TRIANGLEFAN=0x00000076;
    const SREVAR SRE_PRIMITIVETYPE_TRIANGLESTRIP=0x00000077;
    const SREVAR SRE_PRIMITIVETYPE_TRIANGLELIST=0x00000078;


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
