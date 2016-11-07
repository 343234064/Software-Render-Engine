#include <iostream>
#include <stdio.h>
#include <cstring>
#include <memory>

#include "SoftRenderEngine.h"

using namespace SREngine;
using  std::cout;
using  std::endl;
using  std::unique_ptr;

struct vv
{
    float x,y,z;
    float nx,ny,nz;
    float a,r,g,b;
};

struct v2
{
    float nx,ny,nz;
    float a,r,g,b;
};
  /*
int main()
{
  int vnum = 8;
  vv* vlist = new vv[vnum];

  int value = 1;
  for(int i = 0; i<vnum; i++)
  {
  vlist[i].x = value++;
  vlist[i].y = value++;
  vlist[i].z = value++;
  vlist[i].nx = value++;
  vlist[i].ny = value++;
  vlist[i].nz = value++;
  vlist[i].a = value++;
  vlist[i].r = value++;
  vlist[i].g = value++;
  vlist[i].b = value++;
  }

  int inum = 9;
  int index[] = {2,3,4,5,6,7,0,1,2};
  TriangleMesh* ptriangle;

  RESULT result =
  CreateTriangleMesh(vnum, SRE_FORMAT_VERTEX_XYZ | SRE_FORMAT_ATTRIBUTE_NORMAL | SRE_FORMAT_ATTRIBUTE_DIFFUSE,
                     sizeof(vv), (void*)vlist, inum, index, SRE_PRIMITIVETYPE_TRIANGLELIST, &ptriangle);

 if(result != RESULT::SUCC)
    cout<<"error"<<endl;
  else{

  TriangleMesh* tri = ptriangle;
  cout<<"Edge Number:"<<tri->m_edgeNumber<<endl;
  for(int i=0;i<tri->m_edgeNumber; i++)
      cout<<tri->m_pEdgeList.get()[i].get()[0]<<tri->m_pEdgeList.get()[i].get()[1]<<" ";
  cout<<endl;

  cout<<"Face Number:"<<tri->m_faceNumber<<endl;
  for(int i=0;i<tri->m_faceNumber; i++)
      cout<<tri->m_pFaceList.get()[i].get()[0]<<tri->m_pFaceList.get()[i].get()[1]<<tri->m_pFaceList.get()[i].get()[2]<<" ";
  cout<<endl;

  cout<<"Vertex Number:"<<tri->m_vertexNumber<<endl;
  v2* attrilist = (v2*)tri->m_pAttributes.get();
  for(int i=0;i<tri->m_vertexNumber;i++)
      cout<<tri->m_pVertexList.get()[i].x<<","<<tri->m_pVertexList.get()[i].y<<","<<tri->m_pVertexList.get()[i].z<<","<<tri->m_pVertexList.get()[i].w<<" "
          <<attrilist[i].nx<<","<<attrilist[i].ny<<","<<attrilist[i].nz<<" "
          <<attrilist[i].a<<","<<attrilist[i].r<<","<<attrilist[i].g<<","<<attrilist[i].b<<endl;

   }

   cout<<"constructor test=============================="<<endl;
   TriangleMesh another = *ptriangle;
   TriangleMesh another2(another);

   another.m_pVertexList.get()[0].x = 100;
   another2.m_pVertexList.get()[0].x = 200;

    for(int i=0;i<another.m_vertexNumber;i++)
      cout<<another.m_pVertexList.get()[i].x<<","<<another.m_pVertexList.get()[i].y
          <<","<<another.m_pVertexList.get()[i].z<<","<<another.m_pVertexList.get()[i].w<<endl;
    cout<<endl;
    for(int i=0;i<another2.m_vertexNumber;i++)
      cout<<another2.m_pVertexList.get()[i].x<<","<<another2.m_pVertexList.get()[i].y
          <<","<<another2.m_pVertexList.get()[i].z<<","<<another2.m_pVertexList.get()[i].w<<endl;
    cout<<endl;
    for(int i=0;i<ptriangle->m_vertexNumber;i++)
      cout<<ptriangle->m_pVertexList.get()[i].x<<","<<ptriangle->m_pVertexList.get()[i].y
          <<","<<ptriangle->m_pVertexList.get()[i].z<<","<<ptriangle->m_pVertexList.get()[i].w<<endl;
    ptriangle->~TriangleMesh();

 return 0;
} */




class aa
{
public:
    aa(){v=-1;cout<<"aa constructor!"<<endl;}
    ~aa(){cout<<v<<":aa destructor!"<<endl;}
    int v;

};


typedef unique_ptr<aa, array_deleter<aa>> unique_array;
typedef unique_ptr<unique_array, array_deleter<unique_array>> pArray;

int main()
{
    int row = 10;
    /*
    pArray array(new unique_array[row]);
    for(int i=0; i<row; i++)
    {
        array.get()[i].reset(new aa[2]);
        array.get()[i].get()[0].v = i;
        array.get()[i].get()[1].v = i+1;
        cout<<array.get()[i].get()[0].v<<" "<<array.get()[i].get()[1].v<<endl;
    }
    cout<<"end init"<<endl;
    array.reset(nullptr);
    */
    aa* a=new aa();
    a->v = 10;

    delete a;

}

