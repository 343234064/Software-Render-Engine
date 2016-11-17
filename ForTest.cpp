#include <iostream>
#include <stdio.h>
#include <cstring>
#include <memory>
#include <map>
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

struct attribute
{
    float nx,ny,nz;
    float a,r,g,b;
};





class aa
{
public:
    aa(){v=-1;cout<<"aa constructor!"<<endl;}
    virtual ~aa(){cout<<v<<":aa destructor!"<<endl;}
    int v;

};

class av:public aa
{
public:
    av(){cout<<"av constructor!"<<endl;}
    virtual ~av(){cout<<"av destructor!"<<endl;}

};


typedef unique_ptr<aa, array_deleter<aa>> unique_array;
typedef unique_ptr<unique_array, array_deleter<unique_array>> pArray;

int main()
{

    av obj1;
    aa * pObj1 = (aa*)&obj1;
    obj1.v = 1123;
    aa* geted = pObj1;
    delete pObj1;

    obj1.v = 23;

    cout<<"deleted"<<endl;

    int value = 2;
    int* pv = &value;
    delete pv;
    value = 3;
    cout<<value<<endl;
    //¶Ñ£¬Õ»µÄÇø±ð£¡£¡

    return 0;

}

