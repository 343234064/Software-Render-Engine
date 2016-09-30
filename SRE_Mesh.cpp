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
#include <cstring>
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
    //pVertexes: the vertexes array
    //indexNumber: the number of index
    //pIndexes: the index array
    //primitiveType: the primitive type which to
    //               decide how the index number in
    //               the index array to construct the
    //               mesh, see SRE_GlobalsAndUtils.h
    //pVertexAttributes: the vertex attributes buffer
    //ppOutTriangleMesh: the output mesh
    //
    //It will automatically ignore the single vertex
    //or single edge
	//===========================================
	RESULT CreateTriangleMesh(const INT vertexNumber,
                              const SREVAR vertexFormat,
                              const void * pVertexes,
                              const INT   indexNumber,
                              const INT * pIndexes,
                              const SREVAR primitiveType,
                              const Buffer* pVertexAttributes,
                              TriangleMesh** ppOutTriangleMesh
                              )
    {
#ifdef _SRE_DEBUG_
       if(nullptr == pVertexes || nullptr == pIndexes ||
          nullptr == pVertexAttributes || nullptr == ppOutTriangleMesh)
       {
           _LOG(SRE_ERROR_NULLPOINTER);
           return INVALIDARG;
       }

       if(vertexNumber <= 2 || indexNumber <= 0)
           return FAIL;


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
       bool *validVertexList = nullptr;
       int   validVertexNum = 0;
       int i = 0, e = 0, f = 0;

       validVertexList = new bool[vertexNumber];
       if(nullptr == validVertexList) return OUTMEMORY;
       memset(validVertexList, 0, vertexNumber * sizeof(bool));

       if(primitiveType == SRE_PRIMITIVETYPE_TRIANGLEFAN)
       {
           int count = 0;
           int group = 0;

           /*Calculate the number of edges and groups and valid vertexes*/
           while(i < indexNumber)
           {
               if(pIndexes[i]<0 || pIndexes[i]>=vertexNumber)
               {
                   if(count > 2)
                   {
                      edgeNumber = edgeNumber + 2*count - 3;
                      group++;

                      while(count > 0){
                        validVertexList[pIndexes[i-count]] = true;
                        validVertexNum++;
                        count--;
                      }
                   }
                   count = 0;
               }
               else
                   count++;
               i++;
           }

           /*Calculate the number of faces*/
           faceNumber = edgeNumber - validVertexNum + group;

           /*If we got a non-zero face amount, then continue to generate the edge list
             and face list, else , this is not a triangle mesh*/
           if(faceNumber > 0)
           {
              edgeList = new INT*[edgeNumber];
              if(nullptr == edgeList)
                 return OUTMEMORY;

              faceList = new INT*[faceNumber];
              if(nullptr == faceList)
                 return OUTMEMORY;

              i = 1;
              int k = 0;
              int temp = 0;
              /*Travel down the index list and fill in the edge list and face list*/
              while(i < indexNumber)
              {
                  if(pIndexes[i]>=0 && pIndexes[i]<vertexNumber)
                  {
                      k = i;
                      if(pIndexes[i-1]>=0 && pIndexes[i-1]<vertexNumber)
                      {
                          temp = e;
                          e++;
                          for(k=i+1; k<indexNumber; k++)
                          {
                             if(pIndexes[k]>=0 && pIndexes[k]<vertexNumber)
                             {
                                edgeList[e] = new INT[2];
                                if(nullptr == edgeList[e])
                                   return OUTMEMORY;
                                edgeList[e][0]   = pIndexes[k-1];
                                edgeList[e++][1] = pIndexes[k];

                                edgeList[e] = new INT[2];
                                if(nullptr == edgeList[e])
                                   return OUTMEMORY;
                                edgeList[e][0]   = pIndexes[i-1];
                                edgeList[e++][1] = pIndexes[k];

                                faceList[f] = new INT[3];
                                if(nullptr == faceList[f])
                                   return OUTMEMORY;
                                faceList[f][0] = pIndexes[i-1];
                                faceList[f][1] = pIndexes[k-1];
                                faceList[f++][2] = pIndexes[k];

                             }
                             else
                                break;
                          }
                          if(e > temp)
                          {
                              edgeList[temp] = new INT[2];
                              if(nullptr == edgeList[temp])
                                return OUTMEMORY;
                              edgeList[temp][0] = pIndexes[i-1];
                              edgeList[temp][1] = pIndexes[i];
                          }
                          else
                            e = temp;
                      }
                      i = k + 1;
                  }
                  else
                    i++;
              }
           }
           else
              return FAIL;

       }
       else if(primitiveType == SRE_PRIMITIVETYPE_TRIANGLELIST)
       {
           int count = 0;
           int group = 0;

           /*Calculate the number of edges and groups and valid vertexes*/
           while(i < indexNumber)
           {
               if(pIndexes[i]<0 || pIndexes[i]>=vertexNumber)
               {
                   if(count > 2)
                   {
                      count = count - count % 3;
                      edgeNumber = edgeNumber + count;
                      group++;

                      while(count > 0){
                        validVertexList[pIndexes[i-count]] = true;
                        validVertexNum++;
                        count--;
                      }
                   }
                   count = 0;
               }
               else
                   count++;
               i++;
           }

           /*Calculate the number of faces*/
           faceNumber = edgeNumber - validVertexNum + group;

           /*If we got a non-zero face amount, then continue to generate the edge list
             and face list, else , this is not a triangle mesh*/
           if(faceNumber > 0)
           {
              edgeList = new INT*[edgeNumber];
              if(nullptr == edgeList)
                 return OUTMEMORY;

              faceList = new INT*[faceNumber];
              if(nullptr == faceList)
                 return OUTMEMORY;

              i = 2;

              /*Travel down the index list and fill in the edge list and face list*/
              while(i < indexNumber)
              {
                  if(pIndexes[i]>=0 && pIndexes[i]<vertexNumber)
                  {
                      if((pIndexes[i-1]>=0 && pIndexes[i-1]<vertexNumber)
                         && (pIndexes[i-2]>=0 && pIndexes[i-2]<vertexNumber))
                      {
                          edgeList[e] = new INT[2];
                          if(nullptr == edgeList[e])
                            return OUTMEMORY;
                          edgeList[e][0] = pIndexes[i-2];
                          edgeList[e++][1] = pIndexes[i-1];

                          edgeList[e] = new INT[2];
                          if(nullptr == edgeList[e])
                            return OUTMEMORY;
                          edgeList[e][0] = pIndexes[i-1];
                          edgeList[e++][1] = pIndexes[i];

                          edgeList[e] = new INT[2];
                          if(nullptr == edgeList[e])
                            return OUTMEMORY;
                          edgeList[e][0] = pIndexes[i];
                          edgeList[e++][1] = pIndexes[i-2];

                          faceList[f] = new INT[3];
                          if(nullptr == faceList[f])
                            return OUTMEMORY;
                          faceList[f][0] = pIndexes[i-2];
                          faceList[f][0] = pIndexes[i-1];
                          faceList[f++][0] = pIndexes[i];

                          i+=3;
                      }
                  }
                  else
                     i++;
              }
           }
           else
              return FAIL;

       }
       else if(primitiveType == SRE_PRIMITIVETYPE_TRIANGLESTRIP)
       {
           int count = 0;
           int group = 0;

           /*Calculate the number of edges and groups and valid vertexes*/
           while(i < indexNumber)
           {
               if(pIndexes[i]<0 || pIndexes[i]>=vertexNumber)
               {
                   if(count > 2)
                   {
                      edgeNumber = edgeNumber + 2*count - 3;
                      group++;

                      while(count > 0){
                        validVertexList[pIndexes[i-count]] = true;
                        validVertexNum++;
                        count--;
                      }
                   }
                   count = 0;
               }
               else
                   count++;
               i++;
           }

           /*Calculate the number of faces*/
           faceNumber = edgeNumber - validVertexNum + group;

           /*If we got a non-zero face amount, then continue to generate the edge list
             and face list, else , this is not a triangle mesh*/
           if(faceNumber > 0)
           {
              edgeList = new INT*[edgeNumber];
              if(nullptr == edgeList)
                 return OUTMEMORY;

              faceList = new INT*[faceNumber];
              if(nullptr == faceList)
                 return OUTMEMORY;

              i = 1;
              int k = 0;
              int temp = 0;
              /*Travel down the index list and fill in the edge list and face list*/
              while(i < indexNumber)
              {
                  if(pIndexes[i]>=0 && pIndexes[i]<vertexNumber)
                  {
                      if(pIndexes[i-1]>=0 && pIndexes[i-1]<vertexNumber)
                      {
                         temp = e;
                         e++;
                         for(k=i+1; k<indexNumber; k++)
                         {
                             if(pIndexes[k]>=0 && pIndexes[k]<vertexNumber)
                             {
                                 edgeList[e] = new INT[2];
                                 if(nullptr == edgeList[e])
                                    return OUTMEMORY;
                                 edgeList[e][0] = pIndexes[k-2];
                                 edgeList[e++][0] = pIndexes[k];

                                 edgeList[e] = new INT[2];
                                 if(nullptr == edgeList[e])
                                    return OUTMEMORY;
                                 edgeList[e][0] = pIndexes[k-1];
                                 edgeList[e++][0] = pIndexes[k];

                                 faceList[f] = new INT[3];
                                 if(nullptr == faceList[f])
                                    return OUTMEMORY;
                                 faceList[f][0] = pIndexes[k-2];
                                 faceList[f][0] = pIndexes[k-1];
                                 faceList[f][0] = pIndexes[k];

                             }
                             else
                             {
                                i = k + 1;
                                break;
                             }
                         }
                         if(e > temp)
                         {
                             edgeList[temp] = new INT[2];
                             if(nullptr == edgeList[temp])
                                return OUTMEMORY;
                             edgeList[temp][0] = pIndexes[i-1];
                             edgeList[temp][1] = pIndexes[i];
                         }
                         else
                            e = temp;
                      }
                      else
                         i++;
                  }
                  else
                    i++;
              }
           }
           else
              return FAIL;

       }
       else
       {
           return INVALIDARG;
       }

       /*Generate the vertex list*/
       void * vertexList = (void*)(new FLOAT[validVertexNum*membersOfperVertex]);
       if(nullptr == vertexList)
           return OUTMEMORY;

       /*Copy the user's vertexes to the vertex list*/
       int p = 0;
       void * vertexes = vertexList;
       while(p < vertexNumber)
       {
           if(validVertexList[p])
           {
              memcpy(vertexList++, pVertexes+p, membersOfperVertex*sizeof(FLOAT));
           }
           p++;
       }
       delete[] validVertexList;
       validVertexList = nullptr;

       /*Generate the attributes list*/
       Buffer * attributes = new Buffer();
       if(nullptr == attributes)
           return OUTMEMORY;

       //memcpy(attributes, pVertexAttributes, pVertexAttributes->m_pDescript->BufferSize*);
       /*Copy the user's attributes to the attributes list*/
       //*attributes = *pVertexAttributes;

       /*Generate the triangle mesh*/
       *ppOutTriangleMesh = new TriangleMesh(vertexes,
                                             edgeList,
                                             faceList,
                                             attributes,
                                             vertexFormat,
                                             validVertexNum,
                                             edgeNumber,
                                             faceNumber);
       if(nullptr == *ppOutTriangleMesh)
          return OUTMEMORY;

       return SUCC;
    }











}
