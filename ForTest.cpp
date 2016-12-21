#include <iostream>
#include <stdio.h>
#include <cstring>
#include <memory>
#include <map>
#include <list>
#include "SoftRenderEngine.h"

using namespace SRE;
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



int cc = 0;

class aa
{
public:
    aa(){v=-1;cout<<"aa constructor!"<<endl;}
    virtual ~aa(){cout<<this<<":aa destructor!"<<endl;}
    int v;

};

class av:public aa
{
public:
    av(){cout<<"av constructor!"<<endl;}
    virtual ~av(){cout<<"av destructor!"<<endl;}

};



int main()
{
    //test if map will destructs the data when calling operator=
    aa a1,a2;
    a2.v = 2;
    a1.v = 1;
    cout<<&a1<<endl;
    cout<<&a2<<endl;
    a2 = a1;

    cout<<"end"<<endl;
}

