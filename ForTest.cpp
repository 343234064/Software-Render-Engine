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


class testProcessor:public BasicProcessor
{
public:
       testProcessor(BasicIOBuffer<BasicIOElement*> * input=nullptr,
                     BasicIOBuffer<BasicIOElement*> * output=nullptr,
                     BasicObserver * observer=nullptr):
                 BasicProcessor(input, output, observer),
                 value(0)
        {}
       virtual ~testProcessor(){}

protected:
       int value;
protected:
       void HandleElememt(BasicIOElement * element)
       {
           Log_WriteKV("Handle index:", value);
           Element* e=(Element*)element;
           e->val++;
           Log_WriteKV("Element:", e->val);
           if(value == 9) this->Cancel();
       }
       void OnCancel(){cout<<"Cancel!!"<<endl;}
       void OnPause(){cout<<"Pause!!"<<endl;}
       void OnResume(){cout<<"Resume!!"<<endl;}
       void OnRunError(){cout<<"RunError!!"<<endl;}
};



int main()
{
    BasicIOBuffer<BasicIOElement*> inputBuffer;
    BasicIOBuffer<BasicIOElement*> outputBuffer;
    Observer observer;

    for(int i=10;i<20;i++)
    {
        Element * e=new Element(i);
        inputBuffer.push((BasicIOElement*)e);
        cout<<e->val<<endl;
    }
/*
    for(int i=10;i<20;i++)
    {
        BasicIOElement * e;
        inputBuffer.wait_and_pop(e);
        Element * e_=(Element*)e;
        cout<<e_->val<<endl;
    }
*/

    testProcessor processor(&inputBuffer, &outputBuffer, &observer);

    processor.Start();



    cout<<"main end"<<endl;
}

