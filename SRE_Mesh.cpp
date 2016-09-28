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
    //pIndexes: the index array
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
                              const INT   indexNumber,
                              const INT * pIndexes,
                              const SREVAR primitiveType,
                              const Buffer* pVertexAttributes,
                              TriangleMesh** ppOutTriangleMesh
                              )
    {
#ifdef _SRE_DEBUG_
       if(nullptr == pVertices || nullptr == pIndexes ||
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

       /*Generate the face list and edge list*/
       INT ** edgeList = nullptr;
       INT ** faceList = nullptr;
       int faceNumber = 0;
       int edgeNumber = 0;
       int i = 0, j = 0;
       if(primitiveType == SRE_PRIMITIVETYPE_POINTLIST)
       {
           /*Don not need to generate the edge list and face list*/;
       }
       else if(primitiveType == SRE_PRIMITIVETYPE_LINELIST)
       {
           /*Calculate the number of edges*/
           while(i <= indexNumber-1)
           {
               if(pIndexes[i] != INDEX_END_FLAG && pIndexes[i+1] != INDEX_END_FLAG)
               {
                   edgeNumber++;
               }
               i++;
           }

           /*If we got a non-zero edge amount, then travel down the index list
             and fill in the edge list*/
           if(edgeNumber > 0)
           {
              edgeList = new INT*[edgeNumber];
              if(nullptr == edgeList)
                 return OUTMEMORY;

              i = 0;
              while(i <= indexNumber-1)
              {
                 if(pIndexes[i] != INDEX_END_FLAG && pIndexes[i+1] != INDEX_END_FLAG)
                 {
                    edgeList[j] = new INT[2];
                    if(nullptr == edgeList[j])
                    {
                       return OUTMEMORY;
                    }
                    edgeList[j][0] = pIndexes[i];
                    edgeList[j][1] = pIndexes[i+1];
                    j++;
                 }
                 i++;
              }
          }
       }
       else if(primitiveType == SRE_PRIMITIVETYPE_TRIANGLEFAN)
       {
           int count = 0;
           /*Calculate the number of edges*/
           while(i < indexNumber)
           {
               if(pIndexes[i++] == INDEX_END_FLAG)
               {
                   if(count > 1)
                     edgeNumber = edgeNumber + 2*count - 3;
                   count = 0;
               }
               else
               {
                   count++;
               }
           }

           /*If we got a non-zero edge amount, then travel down the index list
             and fill in the edge list*/
           if(edgeNumber > 0)
           {
              int group = 0;

              edgeList = new INT*[edgeNumber];
              if(nullptr == edgeList)
                 return OUTMEMORY;

              i = 0;
              int k = 0;
              while(i < indexNumber)
              {
                  if(pIndexes[i] != INDEX_END_FLAG)
                  {
                      k = i + 2;
                      if(pIndexes[i+1] != INDEX_END_FLAG)
                      {
                          group++;

                          edgeList[j] = new INT[2];
                          if(nullptr == edgeList[j])
                             return OUTMEMORY;
                          edgeList[j][0]   = pIndexes[i];
                          edgeList[j++][1] = pIndexes[i+1];

                          for(; k<indexNumber; k++)
                          {
                             if(pIndexes[k] != INDEX_END_FLAG)
                             {
                                edgeList[j] = new INT[2];
                                if(nullptr == edgeList[j])
                                   return OUTMEMORY;
                                edgeList[j][0]   = pIndexes[k-1];
                                edgeList[j++][1] = pIndexes[k];

                                edgeList[j] = new INT[2];
                                if(nullptr == edgeList[j])
                                   return OUTMEMORY;
                                edgeList[j][0]   = pIndexes[i];
                                edgeList[j++][1] = pIndexes[k];
                             }
                             else{
                                i = k + 1;
                                break;
                             }
                          }
                      }
                      else
                        i = k;
                  }
                  else
                    i++;
              }

              /*Calculate the number of faces*/
              faceNumber =

           }

       }
       else if(primitiveType == SRE_PRIMITIVETYPE_TRIANGLELIST)
       {

       }
       else if(primitiveType == SRE_PRIMITIVETYPE_TRIANGLESTRIP)
       {

       }
       else
       {
           return INVALIDARG;
       }

       /*Generate the vertex list*/
       void * vertexList = (void*)(new FLOAT[vertexNumber*membersOfperVertex]);
       if(nullptr == vertexList)
           return OUTMEMORY;

       /*Copy the user's vertice to the vertex list*/
       memcpy(vertexList, pVertices, vertexNumber*membersOfperVertex*sizeof(FLOAT));

       /*Generate the attributes list*/
       Buffer * attributes = new Buffer();
       if(nullptr == attributes)
       {
           return OUTMEMORY;
       }

       /*Copy the user's attributes to the attributes list*/
       //*attributes = *pVertexAttributes;


       *ppOutTriangleMesh = new TriangleMesh();
       if(nullptr == *ppOutTriangleMesh)
       {
           return OUTMEMORY;
       }


       return SUCC;
    }











}
