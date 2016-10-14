#include <iostream>
#include <stdio.h>
#include <cstring>

#include "SoftRenderEngine.h"

using namespace SREngine;
using  std::cout;
using  std::endl;

struct vv
{
    float x,y,z,w;
};

int main()
{

  vv* vl = new vv[2];
  vl[0].x = 1.0;
  vl[0].y = 2.0;
  vl[0].z = 3.0;
  vl[0].w = 4.0;

  vl[1].x = 5.0;
  vl[1].y = 6.0;
  vl[1].z = 7.0;
  vl[1].w = 8.0;

  VERTEX4* vl2 = new VERTEX4[2];
  memcpy(vl2, vl, sizeof(vv)*2);

  cout<<vl2[0].x<<" "<<vl2[0].y<<" "<<vl2[0].z<<" "<<vl2[0].w<<endl;
  cout<<vl2[1].x<<" "<<vl2[1].y<<" "<<vl2[1].z<<" "<<vl2[1].w<<endl;

  delete[] vl;
  delete[] vl2;

 return 0;
}
