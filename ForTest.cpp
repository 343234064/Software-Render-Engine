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





struct vertex
{
    FLOAT ver[3];
    FLOAT nor[3];
    FLOAT a1;
    FLOAT a2;
};



int main()
{

    vertex vertexes[10];
    for(int i=0; i<10; i++)
    {
        vertexes[i].ver[0] = 0.1+i;
        vertexes[i].ver[1] = 0.2+i;
        vertexes[i].ver[2] = 0.3+i;

        vertexes[i].nor[0] = 0.9+i;
        vertexes[i].nor[1] = 0.8+i;
        vertexes[i].nor[2] = 0.7+i;

        vertexes[i].a1 = 0.11+i;
        vertexes[i].a2 = 0.21+i;
    }

    VertexBuffer * vbuffer;

    RESULT re=CreateVertexBuffer(10, sizeof(vertex), SRE_FORMAT_VERTEX_XYZ | SRE_FORMAT_ATTRIBUTE_NORMAL | SRE_FORMAT_ATTRIBUTE_OTHER2,
                                 (BYTE*)vertexes, &vbuffer);

    if(re == RESULT::SUCC)
        cout<<"succ"<<endl;
    else
        cout<<"fail"<<endl;

    INT index[13]={0,1,2,3,6,-1,9,7,5,4,0,2,3};

    Buffer<INT>* ibuffer;
    BufferDescript bd(13,1,1,SRE_TYPE_INDEXBUFFER,0);
    RESULT re2=CreateBuffer(bd, index, &ibuffer);
    if(re2==RESULT::SUCC)
        cout<<"SUCC"<<endl;
    else
        cout<<"FAIL"<<endl;

    BasicIOBuffer<BasicIOElement*> outputBuffer;
    Observer observer;

    InputAssembler IA(&outputBuffer, &observer);


    IA.SetVertexAndIndexBuffers(vbuffer, ibuffer);
    IA.Run();

    cout<<"main end"<<endl;
}

