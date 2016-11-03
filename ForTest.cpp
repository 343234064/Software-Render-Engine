#include <iostream>
#include <stdio.h>
#include <cstring>

#include "SoftRenderEngine.h"

using namespace SREngine;
using  std::cout;
using  std::endl;

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
int main()
{
  int vnum = 2;
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

 if(result != SUCC)
    cout<<"error"<<endl;
  else{

  TriangleMesh* tri = ptriangle;
  cout<<"Edge Number:"<<tri->m_edgeNumber<<endl;
  for(int i=0;i<tri->m_edgeNumber; i++)
      cout<<tri->m_pEdgeList[i][0]<<tri->m_pEdgeList[i][1]<<" ";
  cout<<endl;

  cout<<"Face Number:"<<tri->m_faceNumber<<endl;
  for(int i=0;i<tri->m_faceNumber; i++)
      cout<<tri->m_pFaceList[i][0]<<tri->m_pFaceList[i][1]<<tri->m_pFaceList[i][2]<<" ";
  cout<<endl;

  cout<<"Vertex Number:"<<tri->m_vertexNumber<<endl;
  v2* attrilist = (v2*)tri->m_pAttributes;
  for(int i=0;i<tri->m_vertexNumber;i++)
      cout<<tri->m_pVertexList[i].x<<","<<tri->m_pVertexList[i].y<<","<<tri->m_pVertexList[i].z<<","<<tri->m_pVertexList[i].w<<" "
          <<attrilist[i].nx<<","<<attrilist[i].ny<<","<<attrilist[i].nz<<" "
          <<attrilist[i].a<<","<<attrilist[i].r<<","<<attrilist[i].g<<","<<attrilist[i].b<<endl;

   }
 return 0;
}
