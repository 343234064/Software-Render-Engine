#pragma once
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
    //Type definitions
    //
    //==============================
    typedef float         FLOAT;
    typedef int             INT;
    typedef unsigned int SREVAR;



    //==============================
    //Global variables
    //
    //==============================
    enum RESULT { SUCC=1, FAIL=0, INVALIDARG=-1, OUTMEMORY=-2 };

    /*each vertex has x and y member, ,this format will be treated as
    a screen position of the vertex, that's say, a vertex which uses this
    format will be directly delivered to screen transform part. FLOAT FLOAT*/
    SREVAR SRE_FORMAT_VERTEX_XY=0x00000065;
    /*each vertex has x ,y and z member. FLOAT FLOAT FLOAT*/
    SREVAR SRE_FORMAT_VERTEX_XYZ=0x00000066;
    /*each vertex has x ,y ,z and w member, this format will be treated as
    a transformed and clipped vertex. FLOAT FLOAT FLOAT FLOAT*/
    SREVAR SRE_FORMAT_VERTEX_XYZW=0x00000067;

    SREVAR SRE_FORMAT_ATTRIBUTE_TEXCOORD_1=0x00000068;
    SREVAR SRE_FORMAT_ATTRIBUTE_TEXCOORD_2=0x00000069;
    SREVAR SRE_FORMAT_ATTRIBUTE_TEXCOORD_3=0x0000006A;
    SREVAR SRE_FORMAT_ATTRIBUTE_NORMAL=0x0000006B;
    SREVAR SRE_FORMAT_ATTRIBUTE_DIFFUSE=0x0000006C;
    SREVAR SRE_FORMAT_ATTRIBUTE_SPECULAR=0x0000006D;
    SREVAR SRE_FORMAT_ATTRIBUTE_BINORMAL=0x0000006E;
    SREVAR SRE_FORMAT_ATTRIBUTE_TANGENT=0x0000006F;

    SREVAR SRE_FORMAT_PIXEL_R8G8B8A8=0x00000070;
    SREVAR SRE_FORMAT_PIXEL_R8G8B8=0x00000071;

    SREVAR SRE_BUFFERUSAGE_VERTEXBUFFER=0x00000072;
    SREVAR SRE_BUFFERUSAGE_ATTRIBUTEBUFFER=0x00000073;


    /*
      Primitive types
      POINTLIST: a list of points
      LINELIST: a list of lines, write a -1 means the end of a line
      TRIANGLEFAN: a list of triangle fans, write a -1 means the end of a fan
      TRIANGLESTRIP: a list of continuous triangles, which shared an edge with the adjacent one,
                     write a -1 means the end of a strip.
      TRIANGLELIST: a list of triangles
    */
    SREVAR SRE_PRIMITIVETYPE_POINTLIST=0x00000074;
    SREVAR SRE_PRIMITIVETYPE_LINELIST=0x00000075;
    SREVAR SRE_PRIMITIVETYPE_TRIANGLEFAN=0x00000076;
    SREVAR SRE_PRIMITIVETYPE_TRIANGLESTRIP=0x00000077;
    SREVAR SRE_PRIMITIVETYPE_TRIANGLELIST=0x00000078;






    //==============================
    //Utility classes definitions
    //
    //==============================
    class IContainer;







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


    };






}


#endif
