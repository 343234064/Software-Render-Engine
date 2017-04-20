//*****************************************************
//
// Software Render Engine
// Version 0.01 by XJL
//
// File: SRE_Mesh.cpp
// Date: 2016/06/29
// Description:
//       Implements of mesh functions and classes which declared
//       in SRE_Mesh.h
//
//
//*****************************************************
#include "SRE_Mesh.h"


namespace SRE {
   //===========================================
	//Public functions
	//
	//
	//===========================================
   RESULT LoadObjMesh(char* filePath,  Mesh* p_outmesh)
   {
       if(nullptr == filePath) return RESULT::INVALIDARG;
       if(nullptr == p_outmesh) return RESULT::INVALIDARG;

       std::ifstream file(filePath, std::ios::in);

       std::string line;
       std::string name="\0";
       INT faceCount=0;
       bool tex=false, nor=false;
       std::vector<VERTEX3> vertexes;
       std::vector<VEC3>      normals;
       std::vector<VEC2>      texcoord;
       std::vector<USINT>     indexes;

       FLOAT x, y, z;
       USINT  index;
       while(getline(file, line))
       {
          if(line.substr(0,2)=="g ")
          {
              name = line.substr(2);
          }
          if(line.substr(0,2)== "v ")
          {
              std::istringstream s(line.substr(2));
              s>>x;s>>y;s>>z;
              vertexes.push_back(VERTEX3(x, y, z));
          }
          else if(line.substr(0,3)== "vt ")
          {
              std::istringstream s(line.substr(3));
              s>>x;s>>y;
              texcoord.push_back(VEC2(x, y));
              tex=true;
          }
          else if(line.substr(0,3)== "vn ")
          {
              std::istringstream s(line.substr(3));
              s>>x;s>>y;s>>z;
              normals.push_back(VEC3(x, y, z));
              nor=true;
          }
          else if(line.substr(0,2)== "f ")
          {
              std::istringstream s(line.substr(2));
              USINT i=0;
              while(i<4)
              {
                  s>>index;
                  indexes.push_back(index);
                  if(tex){
                     s.ignore(line.size(), '/');
                     s>>index;
                     indexes.push_back(index);
                  }
                  if(nor){
                     s.ignore(line.size(), '/');
                     s>>index;
                     indexes.push_back(index);
                  }
                  i++;
              }
              faceCount ++;
           }
       }

       if(vertexes.size()>0)
       {
          BYTE*    vbuffer = nullptr;
          USINT*   ibuffer = nullptr;

          INT ver_num = 4*faceCount;
          INT ind_num = 6*faceCount;
          INT sizever3 = sizeof(VERTEX3);
          INT sizevec3 = sizeof(VEC3);
          INT sizevec2 = sizeof(VEC2);

          INT per_ver_size = sizever3;

          if(nor) per_ver_size += sizevec3;
          if(tex) per_ver_size += sizevec2;

          vbuffer = new BYTE[ver_num*per_ver_size];
          if(nullptr == vbuffer) return RESULT::OUTMEMORY;

          ibuffer = new USINT[ind_num];
          if(nullptr == ibuffer) return RESULT::OUTMEMORY;

          INT i=0, vn=0, in=0, ct=0;
          USINT pv1=0, pv2=0, pv3=0;
          while(i<indexes.size())
          {
              *(VERTEX3*)(vbuffer + vn*per_ver_size) = vertexes[indexes[i]-1];

             if(ct == 3)
             {
                 *(ibuffer+in) = pv3;in++;
                 *(ibuffer+in) = vn;in++;
                 *(ibuffer+in) = pv1;in++;
                 ct = 0;
              }
             else
             {
                *(ibuffer+in) = vn;in++;
                ct++;
             }

             pv1 = pv2;
             pv2 = pv3;
             pv3 = vn;

             if(tex)
                *(VEC2*)(vbuffer + vn*per_ver_size + sizever3) = texcoord[indexes[++i]-1];

             if(nor)
                *(VEC3*)(vbuffer + vn*per_ver_size + sizever3 + sizevec2) = normals[indexes[++i]-1];

             vn++;
             i++;
          }



          RESULT re = CreateVertexBuffer(ver_num, per_ver_size, SRE_FORMAT_VERTEX_XYZ | SRE_FORMAT_ATTRIBUTE_TEXCOORDUV | SRE_FORMAT_ATTRIBUTE_NORMAL,
                                                           vbuffer,  &(p_outmesh->vertexes));
          if(re == RESULT::SUCC)
          {
             if(!p_outmesh->indexes.ResetBuffer(ibuffer, ind_num))
                 re = RESULT::FAIL;
             else
                 p_outmesh->name = name;
          }
          delete[] vbuffer;
          delete[] ibuffer;
          return re;
       }
       else
          return RESULT::FAIL;

   }


   RESULT LoadObjMesh2(char* filePath,  Mesh* p_outmesh)
   {
       if(nullptr == filePath) return RESULT::INVALIDARG;
       if(nullptr == p_outmesh) return RESULT::INVALIDARG;

       std::ifstream file(filePath, std::ios::in);

       std::string line;
       std::string name="\0";
       INT faceCount=0;
       bool tex=false, nor=false;
       std::vector<VERTEX3> vertexes;
       std::vector<VEC3>      normals;
       std::vector<VEC2>      texcoord;
       std::vector<USINT>     indexes;

       FLOAT x, y, z;
       USINT  index;
       while(getline(file, line))
       {
          if(line.substr(0,2)=="g ")
          {
              name = line.substr(2);
          }
          if(line.substr(0,2)== "v ")
          {
              std::istringstream s(line.substr(2));
              s>>x;s>>y;s>>z;
              vertexes.push_back(VERTEX3(x, y, z));
          }
          else if(line.substr(0,3)== "vt ")
          {
              std::istringstream s(line.substr(3));
              s>>x;s>>y;
              texcoord.push_back(VEC2(x, y));
              tex=true;
          }
          else if(line.substr(0,3)== "vn ")
          {
              std::istringstream s(line.substr(3));
              s>>x;s>>y;s>>z;
              normals.push_back(VEC3(x, y, z));
              nor=true;
          }
          else if(line.substr(0,2)== "f ")
          {
              std::istringstream s(line.substr(2));
              USINT i=0;
              while(i<3)
              {
                  s>>index;
                  indexes.push_back(index);
                  if(tex){
                     s.ignore(line.size(), '/');
                     s>>index;
                     indexes.push_back(index);
                  }
                  if(nor){
                     s.ignore(line.size(), '/');
                     s>>index;
                     indexes.push_back(index);
                  }
                  i++;
              }
              faceCount ++;
           }
       }

       if(vertexes.size()>0)
       {
          BYTE*    vbuffer = nullptr;
          USINT*   ibuffer = nullptr;

          INT ver_num = 3*faceCount;
          INT ind_num = 3*faceCount;
          INT sizever3 = sizeof(VERTEX3);
          INT sizevec3 = sizeof(VEC3);
          INT sizevec2 = sizeof(VEC2);

          INT per_ver_size = sizever3;

          if(nor) per_ver_size += sizevec3;
          if(tex) per_ver_size += sizevec2;

          vbuffer = new BYTE[ver_num*per_ver_size];
          if(nullptr == vbuffer) return RESULT::OUTMEMORY;

          ibuffer = new USINT[ind_num];
          if(nullptr == ibuffer) return RESULT::OUTMEMORY;

          INT i=0, vn=0;
          while(i<indexes.size())
          {
              *(VERTEX3*)(vbuffer + vn*per_ver_size) = vertexes[indexes[i]-1];

             if(tex)
                *(VEC2*)(vbuffer + vn*per_ver_size + sizever3) = texcoord[indexes[++i]-1];

             if(nor)
                *(VEC3*)(vbuffer + vn*per_ver_size + sizever3 + sizevec2) = normals[indexes[++i]-1];

             vn++;
             i++;
          }



          RESULT re = CreateVertexBuffer(ver_num, per_ver_size, SRE_FORMAT_VERTEX_XYZ | SRE_FORMAT_ATTRIBUTE_TEXCOORDUV | SRE_FORMAT_ATTRIBUTE_NORMAL,
                                                           vbuffer,  &(p_outmesh->vertexes));
          if(re == RESULT::SUCC)
          {
             p_outmesh->name = name;
          }
          delete[] vbuffer;
          delete[] ibuffer;
          return re;
       }
       else
          return RESULT::FAIL;

   }





	/***************Unused**************************/
    //===========================================
	//CreateTMesh
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
    //ppOutTMesh: the output mesh
    //
    //It will automatically ignore the single vertex
    //or single edge
	//===========================================
	RESULT CreateTMesh(const INT vertexNumber,
                              const SREVAR vertexFormat,
                              const INT vertexStructSize,
                              const void * pVertexes,
                              const INT   indexNumber,
                              const INT * pIndexes,
                              const SREVAR primitiveType,
                              TMesh** ppOutTMesh
                              )
    {
#ifdef _SRE_DEBUG_
       if(nullptr == pVertexes || nullptr == pIndexes ||
          nullptr == ppOutTMesh)
       {
           _ERRORLOG(SRE_ERROR_NULLPOINTER);
           return RESULT::INVALIDARG;
       }

       if(vertexNumber <= 2 || indexNumber <= 0)
       {
           _ERRORLOG(SRE_ERROR_FAIL);
           return RESULT::FAIL;
       }

       if(vertexStructSize <= 0 && vertexStructSize%sizeof(FLOAT) != 0)
       {
           _ERRORLOG(SRE_ERROR_INVALIDARG);
           return RESULT::INVALIDARG;
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
          return RESULT::INVALIDARG;

       validVertexList = new bool[vertexNumber];
       if(nullptr == validVertexList) return RESULT::OUTMEMORY;
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
                 return RESULT::OUTMEMORY;

              faceList = new INT*[faceNumber];
              if(nullptr == faceList)
                 return RESULT::OUTMEMORY;

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
                                   return RESULT::OUTMEMORY;
                                edgeList[e][0]   = pIndexes[k-1];
                                edgeList[e++][1] = pIndexes[k];

                                edgeList[e] = new INT[2];
                                if(nullptr == edgeList[e])
                                   return RESULT::OUTMEMORY;
                                edgeList[e][0]   = pIndexes[i-1];
                                edgeList[e++][1] = pIndexes[k];

                                faceList[f] = new INT[3];
                                if(nullptr == faceList[f])
                                   return RESULT::OUTMEMORY;
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
                                return RESULT::OUTMEMORY;
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
              return RESULT::FAIL;
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
           faceNumber = edgeNumber / 3;

           /*If we got a non-zero face amount, then continue to generate the edge list
             and face list, else , this is not a triangle mesh*/
           if(faceNumber > 0)
           {
              edgeList = new INT*[edgeNumber];
              if(nullptr == edgeList)
                 return RESULT::OUTMEMORY;

              faceList = new INT*[faceNumber];
              if(nullptr == faceList)
                 return RESULT::OUTMEMORY;

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
                            return RESULT::OUTMEMORY;
                          edgeList[e][0] = pIndexes[i-2];
                          edgeList[e++][1] = pIndexes[i-1];

                          edgeList[e] = new INT[2];
                          if(nullptr == edgeList[e])
                            return RESULT::OUTMEMORY;
                          edgeList[e][0] = pIndexes[i-1];
                          edgeList[e++][1] = pIndexes[i];

                          edgeList[e] = new INT[2];
                          if(nullptr == edgeList[e])
                            return RESULT::OUTMEMORY;
                          edgeList[e][0] = pIndexes[i];
                          edgeList[e++][1] = pIndexes[i-2];

                          faceList[f] = new INT[3];
                          if(nullptr == faceList[f])
                            return RESULT::OUTMEMORY;
                          faceList[f][0] = pIndexes[i-2];
                          faceList[f][1] = pIndexes[i-1];
                          faceList[f++][2] = pIndexes[i];

                          i+=3;
                      }
                      else
                        i++;
                  }
                  else
                     i++;
              }
           }
           else{
              delete[] validVertexList;
              return RESULT::FAIL;
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
                 return RESULT::OUTMEMORY;

              faceList = new INT*[faceNumber];
              if(nullptr == faceList)
                 return RESULT::OUTMEMORY;

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
                                    return RESULT::OUTMEMORY;
                                 edgeList[e][0] = pIndexes[k-2];
                                 edgeList[e++][1] = pIndexes[k];

                                 edgeList[e] = new INT[2];
                                 if(nullptr == edgeList[e])
                                    return RESULT::OUTMEMORY;
                                 edgeList[e][0] = pIndexes[k-1];
                                 edgeList[e++][1] = pIndexes[k];

                                 faceList[f] = new INT[3];
                                 if(nullptr == faceList[f])
                                    return RESULT::OUTMEMORY;
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
                                return RESULT::OUTMEMORY;
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
              return RESULT::FAIL;
           }
       }
       else
       {
           delete[] validVertexList;
           return RESULT::INVALIDARG;
       }


       int sfloat = sizeof(FLOAT);
       BYTE * vertexData = (BYTE*)pVertexes;
       int perAttrSize = vertexStructSize-sfloat*vmembers;
       int perVerSize = vertexStructSize - perAttrSize;

       /*Generate the vertex list and attributes list*/
       VERTEX4 * vertexList = new VERTEX4[validVertexNum];
       if(nullptr == vertexList)
           return RESULT::OUTMEMORY;
       BYTE * attributesList = new BYTE[validVertexNum*perAttrSize];
       if(nullptr == attributesList)
           return RESULT::OUTMEMORY;

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
       TMesh* triangle
                          = new TMesh(vertexList,
                                             edgeList,
                                             faceList,
                                             attributesList,
                                             vertexFormat,
                                             validVertexNum,
                                             edgeNumber,
                                             faceNumber,
                                             perAttrSize);
       if(nullptr == triangle)
          return RESULT::OUTMEMORY;
       *ppOutTMesh = triangle;

       return RESULT::SUCC;
    }



    //===========================================
	//Class TMesh functions
	//
	//
	//===========================================
	//Release
	void TMesh::Release()
	{
        m_pVertexList.reset(nullptr);
        m_pFaceList.reset(nullptr);
        m_pEdgeList.reset(nullptr);
        m_pAttributes.reset(nullptr);

        m_vertexNumber = 0;
        m_faceNumber = 0;
        m_edgeNumber = 0;
        m_perAttrSize = 0;
	}

	//copy constructor
	TMesh::TMesh(const TMesh & other):
	        BaseMesh(other.name),
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
      try
      {
        if(m_vertexNumber>0)
        {
           m_pVertexList.reset(new VERTEX4[m_vertexNumber]);
           VERTEX4* source_1 = other.m_pVertexList.get();
           VERTEX4* dest_1 = m_pVertexList.get();
           std::copy(source_1, source_1 + m_vertexNumber, dest_1);

           m_pAttributes.reset(new BYTE[m_vertexNumber*m_perAttrSize]);
           BYTE* source_2 = other.m_pAttributes.get();
           BYTE* dest_2 = m_pAttributes.get();
           std::copy(source_2, source_2 + m_vertexNumber*m_perAttrSize, dest_2);
        }

        int i=0;
        m_pEdgeList.reset(new unique_int_array[m_edgeNumber]);
        while(i<m_edgeNumber)
        {
            m_pEdgeList.get()[i].reset(new int[2]);
            m_pEdgeList.get()[i].get()[0] = other.m_pEdgeList.get()[i].get()[0];
            m_pEdgeList.get()[i].get()[1] = other. m_pEdgeList.get()[i].get()[1];
            i++;
        }

        i=0;
        m_pFaceList.reset(new unique_int_array[m_faceNumber]);
        while(i<m_faceNumber)
        {
            m_pFaceList.get()[i].reset(new int[3]);
            m_pFaceList.get()[i].get()[0] = other.m_pFaceList.get()[i].get()[0];
            m_pFaceList.get()[i].get()[1] = other.m_pFaceList.get()[i].get()[1];
            m_pFaceList.get()[i].get()[2] = other.m_pFaceList.get()[i].get()[2];
            i++;
        }
      }
      catch(...)
      {
          Release();
          throw;
      }
	}

    //assignment operator
    TMesh & TMesh::operator=(const TMesh & other)
    {
        if(this == &other)
          return *this;

        this->name = other.name;
        this->m_vertexFormat = other.m_vertexFormat;
        this->m_vertexNumber = other.m_vertexNumber;
        this->m_edgeNumber = other.m_edgeNumber;
        this->m_faceNumber = other.m_faceNumber;
        this->m_perAttrSize = other.m_perAttrSize;

        m_pVertexList.reset(nullptr);
        m_pFaceList.reset(nullptr);
        m_pEdgeList.reset(nullptr);
        m_pAttributes.reset(nullptr);

        try
        {

        if(m_vertexNumber>0)
        {
           m_pVertexList.reset(new VERTEX4[m_vertexNumber]);
           VERTEX4* source_1 = other.m_pVertexList.get();
           VERTEX4* dest_1 = m_pVertexList.get();
           std::copy(source_1, source_1 + m_vertexNumber, dest_1);

           m_pAttributes.reset(new BYTE[m_vertexNumber*m_perAttrSize]);
           BYTE* source_2 = other.m_pAttributes.get();
           BYTE* dest_2 = m_pAttributes.get();
           std::copy(source_2, source_2 + m_vertexNumber*m_perAttrSize, dest_2);
        }

        int i=0;
        m_pEdgeList.reset(new unique_int_array[m_edgeNumber]);
        while(i<m_edgeNumber)
        {
            m_pEdgeList.get()[i].reset(new int[2]);
            m_pEdgeList.get()[i].get()[0] = other.m_pEdgeList.get()[i].get()[0];
            m_pEdgeList.get()[i].get()[1] = other. m_pEdgeList.get()[i].get()[1];
            i++;
        }

        i=0;
        m_pFaceList.reset(new unique_int_array[m_faceNumber]);
        while(i<m_faceNumber)
        {
            m_pFaceList.get()[i].reset(new int[3]);
            m_pFaceList.get()[i].get()[0] = other.m_pFaceList.get()[i].get()[0];
            m_pFaceList.get()[i].get()[1] = other.m_pFaceList.get()[i].get()[1];
            m_pFaceList.get()[i].get()[2] = other.m_pFaceList.get()[i].get()[2];
            i++;
        }

        }
        catch(...)
        {
            Release();
            throw;
        }

        return *this;
    }

    //move constructor
    TMesh::TMesh(TMesh && other):
            BaseMesh(std::move(other.name)),
            m_pVertexList(std::move(other.m_pVertexList)),
            m_pEdgeList(std::move(other.m_pEdgeList)),
            m_pFaceList(std::move(other.m_pFaceList)),
            m_pAttributes(std::move(other.m_pAttributes)),
            m_vertexFormat(other.m_vertexFormat),
            m_vertexNumber(other.m_vertexNumber),
            m_edgeNumber(other.m_edgeNumber),
            m_faceNumber(other.m_faceNumber),
            m_perAttrSize(other.m_perAttrSize)
    {
        other.m_pVertexList = nullptr;
        other.m_pEdgeList = nullptr;
        other.m_pFaceList = nullptr;
        other.m_pAttributes = nullptr;
    }

    //move assignment
    TMesh & TMesh::operator=(TMesh && other)
    {
        if(this != &other)
        {
           m_pVertexList = nullptr;
           m_pFaceList = nullptr;
           m_pEdgeList = nullptr;
           m_pAttributes = nullptr;

           name = std::move(other.name);
           m_vertexFormat = other.m_vertexFormat;
           m_vertexNumber = other.m_vertexNumber;
           m_edgeNumber = other.m_edgeNumber;
           m_faceNumber = other.m_faceNumber;
           m_perAttrSize = other.m_perAttrSize;
           m_pVertexList = std::move(other.m_pVertexList);
           m_pFaceList = std::move(other.m_pFaceList);
           m_pEdgeList = std::move(other.m_pEdgeList);
           m_pAttributes = std::move(other.m_pAttributes);

           other.m_pVertexList = nullptr;
           other.m_pEdgeList = nullptr;
           other.m_pFaceList = nullptr;
           other.m_pAttributes = nullptr;
        }

        return *this;
    }



    //===========================================
	//Class TMeshManager functions
	//
	//
	//===========================================
	INT TMeshManager::GetVertexNumber(TMesh * mesh)
	{
#ifdef _SRE_DEBUG_
        if(nullptr == mesh) return 0;
#endif
	    return mesh->m_vertexNumber;
	}

	INT TMeshManager::GetEdgeNumber(TMesh * mesh)
	{
#ifdef _SRE_DEBUG_
        if(nullptr == mesh) return 0;
#endif
	    return mesh->m_edgeNumber;
	}

	INT TMeshManager::GetFaceNumber(TMesh * mesh)
	{
#ifdef _SRE_DEBUG_
        if(nullptr == mesh) return 0;
#endif
	    return mesh->m_faceNumber;
	}

    void * TMeshManager::GetVertex(INT vertexIndex, TMesh * mesh)
    {
#ifdef _SRE_DEBUG_
        if(nullptr == mesh) return nullptr;
        if(vertexIndex < 0 || vertexIndex >= mesh->m_vertexNumber) return nullptr;
#endif
        return (void*)(&(mesh->m_pVertexList.get()[vertexIndex]));
    }

    void * TMeshManager::GetVertexFromEdge(INT edgeIndex, INT vertexIndex, TMesh * mesh)
    {
#ifdef _SRE_DEBUG_
        if(nullptr == mesh) return nullptr;
        if(edgeIndex < 0 || edgeIndex >= mesh->m_edgeNumber) return nullptr;
        if(vertexIndex < 0 || vertexIndex > 2) return nullptr;
#endif

        return (void*)(&(mesh->m_pEdgeList.get()[edgeIndex].get()[vertexIndex]));
    }


    void * TMeshManager::GetVertexFromFace(INT faceIndex, INT vertexIndex, TMesh * mesh)
    {
#ifdef _SRE_DEBUG_
        if(nullptr == mesh) return nullptr;
        if(faceIndex < 0 || faceIndex >= mesh->m_faceNumber) return nullptr;
        if(vertexIndex < 0 || vertexIndex > 3) return nullptr;
#endif

        return (void*)(&(mesh->m_pFaceList.get()[faceIndex].get()[vertexIndex]));
    }

    void * TMeshManager::GetAttribute(INT vertexIndex, TMesh * mesh)
    {
#ifdef _SRE_DEBUG_
        if(nullptr == mesh) return nullptr;
        if(vertexIndex < 0 || vertexIndex >= mesh->m_vertexNumber) return nullptr;
#endif
        BYTE* attribute = mesh->m_pAttributes.get();
        return  (void*)(attribute + vertexIndex * mesh->m_perAttrSize);
    }
    /***************Unused**************************/
}
