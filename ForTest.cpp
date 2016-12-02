#include <iostream>
#include <stdio.h>
#include <cstring>
#include <memory>
#include <map>
#include <list>
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

class pro:public BasicProcessor
{
public:
    pro(int _v):v(_v){cout<<"pro constructor!"<<endl;}
    ~pro(){cout<<"pro destructor!"<<endl;}
    int v;

    void Run()
    {
        cout<<v<<endl;
        NextStage();
    }

    void NextStage()
    {
        if(this->m_pNextStage != nullptr)
           this->m_pNextStage->Run();
    }
};


int main()
{
    PileLineBuilder builder;
    pro p1(1);
    pro p2(2);
    pro p3(3);
    pro p4(4);

    builder.AddProcessor(0,(BasicProcessor*)&p1);
    builder.AddProcessor(1,(BasicProcessor*)&p2);
    builder.AddProcessor(2,(BasicProcessor*)&p3);
    builder.AddProcessor(0,(BasicProcessor*)&p4);
    BasicProcessor * my = builder.BuildPileLine();
    pro * pp = (pro*)my;
    pp->Run();
    cout<<endl;

    builder.RemoveProcessor(6);
    my = builder.BuildPileLine();
    pp = (pro*)my;
    pp->Run();

}

