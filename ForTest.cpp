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
  vlist[i].r = value++;
  vlist[i].g = value++;
  vlist[i].b = value++;
  }

  int inum = 9;
  int index[] = {0,1,2,-1,3,4,5,6,7};
  TriangleMesh* triangle;

  //RESULT result =
  CreateTriangleMesh(vnum, SRE_FORMAT_VERTEX_XYZ | SRE_FORMAT_ATTRIBUTE_NORMAL | SRE_FORMAT_ATTRIBUTE_DIFFUSE,
                     sizeof(vv), (void*)vlist, inum, index, SRE_PRIMITIVETYPE_TRIANGLEFAN, &triangle);

 // if(result != SUCC)
 //   cout<<"error"<<endl;


 return 0;
}
