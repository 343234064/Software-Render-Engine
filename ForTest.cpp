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

class Element:public BasicIOElement
{
public:
    Element(int v=0):
        val(v)
    {}
    ~Element(){}
public:
    int val;

};


class Observer:public BasicObserver
{
public:
    Observer(){}
    ~Observer(){}

protected:
    void HandleMessage(SREVAR message)
    {
        cout<<"Observer Handler Message!!"<<endl;
    }
};


class testProcessor:public BasicProcessor, public CallBackFunctions
{
public:
       testProcessor(BasicIOBuffer<BasicIOElement*> * input=nullptr,
                     BasicIOBuffer<BasicIOElement*> * output=nullptr,
                     BasicObserver * observer=nullptr):
                 BasicProcessor(input, output, observer, (CallBackFunctions*)this),
                 value(1)
        {}
       ~testProcessor(){cout<<"test processor 1 destructor"<<endl;}

protected:
          void HandleElement(BasicIOElement * element)
          {
           if(value == 9) Cancel();
           value++;
           g_log.Write("----Processor:1");
           Element* e=(Element*)element;
           g_log.WriteKV("1 Element:", e->val);
           g_log.Write("---------------");

          }
          void OnCancel(){cout<<"Cancel!!"<<endl;}
          void OnPause(){cout<<"Pause!!"<<endl;}
          void OnResume(){cout<<"Resume!!"<<endl;}
          void OnRunError(){cout<<"RunError!!"<<endl;}
          void OnStart(){}


protected:
       int value;

};


class testProcessor2:public BasicProcessor, public CallBackFunctions
{
public:
       testProcessor2(BasicIOBuffer<BasicIOElement*> * input=nullptr,
                     BasicIOBuffer<BasicIOElement*> * output=nullptr,
                     BasicObserver * observer=nullptr):
                 BasicProcessor(input, output, observer, (CallBackFunctions*)this),
                 value(1)
        {}
       ~testProcessor2(){cout<<"test processor 2 destructor"<<endl;}

protected:
          void HandleElement(BasicIOElement * element)
          {
           if(value == 9) Cancel();
           value++;
           g_log.Write("===Processor:2");
           Element* e=(Element*)element;
           e->val++;
           g_log.WriteKV("2 Element:", e->val);
           g_log.Write("===========");
          }
          void OnCancel(){cout<<"Cancel!!"<<endl;}
          void OnPause(){cout<<"Pause!!"<<endl;}
          void OnResume(){cout<<"Resume!!"<<endl;}
          void OnRunError(){cout<<"RunError!!"<<endl;}
          void OnStart(){}

protected:
       int value;

};


struct vertex
{
    FLOAT ver[3];
    FLOAT nor[3];
    FLOAT a1;
    FLOAT a2;
};



int main()
{
/*
    vertex vertexes[10];
    for(int i=0; i<10; i++)
    {
        vertexes[i].ver[0] = 1.1+i;
        vertexes[i].ver[1] = 1.2+i;
        vertexes[i].ver[2] = 1.3+i;

        vertexes[i].nor[0] = 2.1+i;
        vertexes[i].nor[1] = 2.2+i;
        vertexes[i].nor[2] = 2.3+i;

        vertexes[i].a1 = 3.1+i;
        vertexes[i].a2 = 3.2+i;
    }

    VertexBuffer * vbuffer;

    RESULT re=CreateVertexBuffer(10, sizeof(vertex), SRE_FORMAT_VERTEX_XYZ | SRE_FORMAT_ATTRIBUTE_NORMAL | SRE_FORMAT_ATTRIBUTE_OTHER2,
                                 (BYTE*)vertexes, &vbuffer);

    if(re == RESULT::SUCC)
        cout<<"succ"<<endl;
    else
        cout<<"fail"<<endl;

    BYTE* attri;
    vbuffer->GetAttributes(2, &attri);

    FLOAT* a1=(FLOAT*)(attri);
    FLOAT* a2=(FLOAT*)(attri+sizeof(FLOAT));
    FLOAT* a3=(FLOAT*)(attri+2*sizeof(FLOAT));
    cout<<*a1<<" "<<*a2<<" "<<*a3<<endl;

    cout<<vbuffer->GetVertexX(2)<<endl;
    cout<<vbuffer->GetVertexY(2)<<endl;
    cout<<vbuffer->GetVertexZ(2)<<endl;

    VERTEX3 ver3=vbuffer->GetVertex3(3);
    cout<<ver3.x<<" "<<ver3.y<<" "<<ver3.z<<endl;
*/

    INT index[10];
    for(int i=0;i<10;i++)
        index[i]=i;

    Buffer<INT>* ibuffer;
    BufferDescript bd(10,1,1,SRE_TYPE_INDEXBUFFER,0);
    RESULT re2=CreateBuffer(bd, index, &ibuffer);
    if(re2==RESULT::SUCC)
        cout<<"SUCC"<<endl;
    else
        cout<<"FAIL"<<endl;



    cout<<"main end"<<endl;

}

