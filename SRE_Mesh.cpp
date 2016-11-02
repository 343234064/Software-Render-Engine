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

#include <iostream>
using  std::cout;
using  std::endl;

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
	//vertexStructSize: the size of the struct of vertex
    //faceNumber: the number of face in the mesh
    //pVertexes: the vertexes array
    //indexNumber: the number of index
    //pIndexes: the index array
    //primitiveType: the primitive type which to
    //               decide how the index number in
    //               the index array to construct the
    //               mesh, see SRE_GlobalsAndUtils.h
    //ppOutTriangleMesh: the output mesh
    //
    //It will automatically ignore the single vertex
    //or single edge
	//===========================================
	RESULT CreateTriangleMesh(const INT vertexNumber,
                              const SREVAR vertexFormat,
                              const INT vertexStructSize,
                              const void * pVertexes,
                              const INT   indexNumber,
                              const INT * pIndexes,
                              const SREVAR primitiveType,
                              TriangleMesh** ppOutTriangleMesh
                              )
    {
#ifdef _SRE_DEBUG_
       if(nullptr == pVertexes || nullptr == pIndexes ||
          nullptr == ppOutTriangleMesh)
       {
           _LOG(SRE_ERROR_NULLPOINTER);
           return INVALIDARG;
       }

       if(vertexNumber <= 2 || indexNumber <= 0)
       {
           _LOG(SRE_ERROR_FAIL);
           return FAIL;
       }

       if(vertexStructSize <= 0 && vertexStructSize%sizeof(FLOAT) != 0)
       {
           _LOG(SRE_ERROR_INVALIDARG);
           return INVALIDARG;
       }

#endif // _SRE_DEBUG_

       /*Generate the face list and edge list*/
       INT ** edgeList = nullptr;
       INT ** faceList = nullptr;
       INT faceNumber = 0;
       INT edgeNumber = 0;
       bool *validVertexList = nullptr;
       int   validVertexNum = 0;
       int i = 0, e = 0, f = 0;

       int vmembers = 0;
       if((vertexFormat & SRE_FORMAT_VERTEX_XYZW) == SRE_FORMAT_VERTEX_XYZW)
          vmembers = 4;
       else if((vertexFormat & SRE_FORMAT_VERTEX_XYZ) == SRE_FORMAT_VERTEX_XYZ)
          vmembers = 3;
       else if((vertexFormat & SRE_FORMAT_VERTEX_XY) == SRE_FORMAT_VERTEX_XY)
          vmembers = 2;
       else
          return INVALIDARG;

       validVertexList = new bool[vertexNumber];
       if(nullptr == validVertexList) return OUTMEMORY;
       memset(validVertexList, 0, vertexNumber * sizeof(bool));

       if(primitiveType == SRE_PRIMITIVETYPE_TRIANGLEFAN)
       {
           int count = 0;
           int group = 0;
           int startPos = -1;
           /*Calculate the number of edges and groups and valid vertexes*/
           while(i <= indexNumber)
           {
               if(i==indexNumber || pIndexes[i]<0 || pIndexes[i]>=vertexNumber )
               {
                   if(count > 2)
                   {
                      edgeNumber = edgeNumber + 2*count - 3;
                      group++;

                      if(startPos == -1)
                         startPos = i - count + 1;

                      while(count > 0){
                        validVertexNum++;
                        validVertexList[pIndexes[i-count]] = true;
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

           validVertexNum = 0;
           for(int n=0; n<vertexNumber; n++)
              if(validVertexList[n]) validVertexNum++;

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

              i = startPos;
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
                          if(e > temp + 1)
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
           else{
              delete[] validVertexList;
              return FAIL;
           }

       }
       else if(primitiveType == SRE_PRIMITIVETYPE_TRIANGLELIST)
       {
           int count = 0;
           int group = 0;
           int startPos = -1;
           /*Calculate the number of edges and groups and valid vertexes*/
           while(i <= indexNumber)
           {
               if(i==indexNumber || pIndexes[i]<0 || pIndexes[i]>=vertexNumber)
               {
                   if(count > 2)
                   {
                      count = count - count % 3;
                      edgeNumber = edgeNumber + count;
                      group++;

                      if(startPos == -1)
                         startPos = i - count + 2;

                      while(count > 0){
                        if(!validVertexList[pIndexes[i-count]])
                        {
                            validVertexNum++;
                            validVertexList[pIndexes[i-count]] = true;
                        }
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

              i = startPos;

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
                          faceList[f][1] = pIndexes[i-1];
                          faceList[f++][2] = pIndexes[i];

                          i+=3;
                      }
                  }
                  else
                     i++;
              }
           }
           else{
              delete[] validVertexList;
              return FAIL;
           }

       }
       else if(primitiveType == SRE_PRIMITIVETYPE_TRIANGLESTRIP)
       {
           int count = 0;
           int group = 0;
           int startPos = -1;
           /*Calculate the number of edges and groups and valid vertexes*/
           while(i <= indexNumber)
           {
               if(i==indexNumber || pIndexes[i]<0 || pIndexes[i]>=vertexNumber)
               {
                   if(count > 2)
                   {
                      edgeNumber = edgeNumber + 2*count - 3;
                      group++;

                      if(startPos == -1)
                         startPos = i - count + 1;

                      while(count > 0){
                        validVertexNum++;
                        validVertexList[pIndexes[i-count]] = true;
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

           validVertexNum = 0;
           for(int n=0; n<vertexNumber; n++)
              if(validVertexList[n]) validVertexNum++;

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

              i = startPos;
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
                                 edgeList[e][0] = pIndexes[k-2];
                                 edgeList[e++][1] = pIndexes[k];

                                 edgeList[e] = new INT[2];
                                 if(nullptr == edgeList[e])
                                    return OUTMEMORY;
                                 edgeList[e][0] = pIndexes[k-1];
                                 edgeList[e++][1] = pIndexes[k];

                                 faceList[f] = new INT[3];
                                 if(nullptr == faceList[f])
                                    return OUTMEMORY;
                                 faceList[f][0] = pIndexes[k-2];
                                 faceList[f][1] = pIndexes[k-1];
                                 faceList[f++][2] = pIndexes[k];

                             }
                             else
                                break;
                         }
                         if(e > temp + 1)
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
           else{
              delete[] validVertexList;
              return FAIL;
           }
       }
       else
       {
           delete[] validVertexList;
           return INVALIDARG;
       }

///////////////////////////
for(int i=0; i<faceNumber;i++)
    cout<<faceList[i][0]<<faceList[i][1]<<faceList[i][2]<<" ";
cout<<endl;
for(int i=0; i<edgeNumber;i++)
    cout<<edgeList[i][0]<<edgeList[i][1]<<" ";
cout<<endl;
for(int i=0; i<vertexNumber; i++)
   if(validVertexList[i])
      cout<<i<<" ";
cout<<endl;
cout<<endl;

//////////////////////////

       int sfloat = sizeof(FLOAT);
       int sVertex4 = sizeof(VERTEX4);

       BYTE * vertexData = (BYTE*)pVertexes;
       int perAttrSize = vertexStructSize-sfloat*vmembers;
       int perVerSize = vertexStructSize - perAttrSize;

       /*Generate the vertex list and attributes list*/
       VERTEX4 * vertexList = new VERTEX4[validVertexNum];
       if(nullptr == vertexList)
           return OUTMEMORY;
       BYTE * attributesList = new BYTE[validVertexNum*perAttrSize];
       if(nullptr == attributesList)
           return OUTMEMORY;

       /*Copy user's vertexes and attributes to the vertex list and attributes list*/
       int p = 0, q = 0;
       while(p < vertexNumber)
       {
           if(validVertexList[p])
           {
               memcpy(vertexList+q, vertexData+p*vertexStructSize, perVerSize);
               memcpy(attributesList+q*perAttrSize, vertexData+p*vertexStructSize+perVerSize, perAttrSize);
               q++;
           }
           p++;
       }
       delete[] validVertexList;
       validVertexList = nullptr;


       /*Generate the triangle mesh*/
       TriangleMesh* triangle
                          = new TriangleMesh(vertexList,
                                             edgeList,
                                             faceList,
                                             attributesList,
                                             vertexFormat,
                                             validVertexNum,
                                             edgeNumber,
                                             faceNumber,
                                             perAttrSize);
       if(nullptr == triangle)
          return OUTMEMORY;
       *ppOutTriangleMesh = triangle;

       return SUCC;
    }




    //===========================================
	//Class TriangleMesh functions
	//
	//
	//===========================================
	TriangleMesh::TriangleMesh(const TriangleMesh & other):
	        IMesh(other.name),
            m_pVertexList(nullptr),
            m_pEdgeList(nullptr),
            m_pFaceList(nullptr),
            m_pAttributes(nullptr),
            m_vertexFormat(other.m_vertexFormat),
            m_vertexNumber(other.m_vertexNumber),
            m_edgeNumber(other.m_edgeNumber),
            m_faceNumber(other.m_faceNumber),
            m_perAttrSize(other.m_perAttrSize)
	{

        m_pVertexList = new VERTEX4[m_vertexNumber];
        memcpy(m_pVertexList, other.m_pVertexList, sizeof(VERTEX4)*m_vertexNumber);

        m_pAttributes = new BYTE[m_vertexNumber*m_perAttrSize];
        memcpy(m_pAttributes, other.m_pAttributes, m_vertexNumber*m_perAttrSize);

        int i=0;
        m_pFaceList = new INT*[m_faceNumber];
        while(i<m_faceNumber)
        {
            m_pFaceList[i] = new INT[3];
            m_pFaceList[i][0] = other.m_pFaceList[i][0];
            m_pFaceList[i][1] = other.m_pFaceList[i][1];
            m_pFaceList[i][2] = other.m_pFaceList[i][2];
            i++;
        }

        i=0;
        m_pEdgeList = new INT*[m_edgeNumber];
        while(i<m_edgeNumber)
        {
            m_pEdgeList[i] = new INT[2];
            m_pEdgeList[i][0] = other.m_pEdgeList[i][0];
            m_pEdgeList[i][1] = other.m_pEdgeList[i][1];
            i++;
        }

	}

    TriangleMesh & TriangleMesh::operator=(const TriangleMesh & other)
    {
        if(this == &other)
          return *this;

        if(nullptr != m_pVertexList)
          delete[] m_pVertexList;
        if(nullptr != m_pAttributes)
          delete[] m_pAttributes;
        if(nullptr != m_pFaceList)
        {
          int i=0;
          while(i++<m_faceNumber)
              delete[] m_pFaceList[i];
          delete[] m_pFaceList;
        }
        if(nullptr != m_pEdgeList)
        {
          int i=0;
          while(i++<m_edgeNumber)
              delete[] m_pEdgeList[i];
          delete[] m_pEdgeList;
        }

        this->name = other.name;
        this->m_vertexFormat = other.m_vertexFormat;
        this->m_vertexNumber = other.m_vertexNumber;
        this->m_edgeNumber = other.m_edgeNumber;
        this->m_faceNumber = other.m_faceNumber;
        this->m_perAttrSize = other.m_perAttrSize;

        m_pVertexList = new VERTEX4[m_vertexNumber];
        memcpy(m_pVertexList, other.m_pVertexList, sizeof(VERTEX4)*m_vertexNumber);

        m_pAttributes = new BYTE[m_vertexNumber*m_perAttrSize];
        memcpy(m_pAttributes, other.m_pAttributes, m_vertexNumber*m_perAttrSize);

        int i=0;
        m_pFaceList = new INT*[m_faceNumber];
        while(i<m_faceNumber)
        {
            m_pFaceList[i] = new INT[3];
            m_pFaceList[i][0] = other.m_pFaceList[i][0];
            m_pFaceList[i][1] = other.m_pFaceList[i][1];
            m_pFaceList[i][2] = other.m_pFaceList[i][2];
            i++;
        }

        i=0;
        m_pEdgeList = new INT*[m_edgeNumber];
        while(i<m_edgeNumber)
        {
            m_pEdgeList[i] = new INT[2];
            m_pEdgeList[i][0] = other.m_pEdgeList[i][0];
            m_pEdgeList[i][1] = other.m_pEdgeList[i][1];
            i++;
        }

        return *this;
    }




}
