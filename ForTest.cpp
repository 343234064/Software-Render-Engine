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


int main()
{
/*
    BasicIOBuffer<BasicIOElement*> inputBuffer;
    BasicIOBuffer<BasicIOElement*> outputBuffer;
    BasicIOBuffer<BasicIOElement*> outputBuffer2;
    Observer observer;

    int v=100;
    for(int i=1;i<10;i++)
    {
        Element * e=new Element(i*v);
        inputBuffer.push((BasicIOElement*)e);
        cout<<e->val<<endl;
    }

    testProcessor processor(&inputBuffer, &outputBuffer, &observer);
    testProcessor2 processor2(&outputBuffer, &outputBuffer2, &observer);
    processor.Start();
    processor2.Start();
*/

   Buffer<int>* buffer;
   BufferDescript bd(2,1,1);
   CreateBuffer(bd, &buffer);


    cout<<"main end"<<endl;

}

