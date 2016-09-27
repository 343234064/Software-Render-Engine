//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_Mesh.cpp
// Date: 2016/06/29
// Description:
//       Implements of mesh functions and classes which declared
//       in SRE_Mesh.h
//
//
//*****************************************************


#include <string>
#include "SRE_Mesh.h"


namespace SREngine {



    //===========================================
	//Public functions
	//
	//
	//===========================================
    //===========================================
	//CreateTriangleMesh
	//
	//
	//vertexNumber: the number of vertex in the mesh
	//vertexSize: the size of each vertex, in byte
	//vertexFormat: the format of vertex,
	//             see SRE_GlobalsAndUtils.h
    //faceNumber: the number of face in the mesh
    //pVertices: the vertices array
    //indexNumber: the number of index
    //pIndex: the index array
    //primitiveType: the primitive type which to
    //               decide how the index number in
    //               the index array to construct the
    //               mesh, see SRE_GlobalsAndUtils.h
    //pVertexAttributes: the vertex attributes buffer
    //ppOutTriangleMesh: the output mesh
	//===========================================
	RESULT CreateTriangleMesh(const INT vertexNumber,
                              const SREVAR vertexFormat,
                              const void * pVertices,
                              const INT   indexNumer,
                              const INT * pIndex,
                              const SREVAR primitiveType,
                              const Buffer* pVertexAttributes,
                              TriangleMesh** ppOutTriangleMesh
                              )
    {
#ifdef _SRE_DEBUG_
       if(nullptr == pVertices || nullptr == pIndex ||
          nullptr == pVertexAttributes || nullptr == ppOutTriangleMesh)
       {
           _LOG(SRE_ERROR_NULLPOINTER);
           return INVALIDARG;
       }

       if(vertexNumber<=0 || indexNumber<=0)
           return INVALIDARG;


#endif // _SRE_DEBUG_

       /*Make sure how many members in each vertex*/
       INT membersOfperVertex = 0;
       if(vertexFormat == SRE_FORMAT_VERTEX_XY)
           membersOfperVertex = 2;
       else if(vertexFormat == SRE_FORMAT_VERTEX_XYZ)
           membersOfperVertex = 3;
       else if(vertexFormat == SRE_FORMAT_VERTEX_XYZW)
           membersOfperVertex = 4;
       else
           return INVALIDARG;

       /*Generate the vertex list*/
       void * vertexList = (void*)(new FLOAT[vertexNumber*membersOfperVertex]);
       if(nullptr == vertexList)
           return OUTMEMORY;

       /*Copy the user's vertice to the vertex list*/
       memcpy(vertexList, pVertices, vertexNumber*membersOfperVertex*sizeof(FLOAT));


       /*Generate the index list*/
       INT * indexList = new INT[indexNumer];
       if(nullptr == indexList)
       {
           delete[] vertexList;
           return OUTMEMORY;
       }


       /*Copy the user's indexes to the index list*/
       memcpy(indexList, pIndex, indexNumer*sizeof(INT));


       /*Generate the attributes list*/
       Buffer * attributes = new Buffer();
       if(nullptr == attributes)
       {
           delete[] vertexList;
           delete[] indexList;
           return OUTMEMORY;
       }

       /*Copy the user's attributes to the attributes list*/
       *attributes = *pVertexAttributes;


       /*Generate the edge list*/
       INT ** edgeList = nullptr;
       int edgeNumber = 0;



       *ppOutTriangleMesh = new TriangleMesh();
       if(nullptr == *ppOutTriangleMesh)
           return OUTMEMORY;

       return SUCC;
    }











}
