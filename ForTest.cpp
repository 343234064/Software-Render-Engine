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
    VEC3  ver;
    VEC3  nor;
    FLOAT a1;
    FLOAT a2;
};

VSOutput* myVS(BYTE* v, VariableBuffer* varbuffer)
{
    VSOutput * out = new VSOutput();
    return out;
}



int main()
{

    vertex vertexes[10];
    for(int i=0; i<10; i++)
    {
        vertexes[i].ver.x = 0.1+i;
        vertexes[i].ver.y = 0.2+i;
        vertexes[i].ver.z = 0.3+i;

        vertexes[i].nor.x = 0.9+i;
        vertexes[i].nor.y = 0.8+i;
        vertexes[i].nor.z = 0.7+i;

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

    //INT index[13]={0,1,2,3,6,-1,9,7,5,4,0,2,3};
    INT index[7]={0,1,2,3,4,5,6};

    Buffer<INT>* ibuffer;
    BufferDescript bd(7,1,1,SRE_TYPE_INDEXBUFFER,0);
    RESULT re2=CreateBuffer(bd, index, &ibuffer);
    if(re2==RESULT::SUCC)
        cout<<"SUCC"<<endl;
    else
        cout<<"FAIL"<<endl;

    BasicIOBuffer<BasicIOElement*> IAoutputBuffer;
    BasicIOBuffer<BasicIOElement*> VPoutputBuffer;
    Observer observer;
    ConstantBuffer conbuffer;
    VariableBuffer varbuffer;
    conbuffer.primitiveTopology = SRE_PRIMITIVETYPE_TRIANGLEFAN;

    InputAssembler IA(&IAoutputBuffer, &observer, &conbuffer);
    IA.SetVertexAndIndexBuffers(vbuffer, ibuffer);

    CallBackVShader* callback = &myVS;
    VertexShader vshader(callback);
    VertexProcessor VP(&IAoutputBuffer, &VPoutputBuffer, &observer, &vshader, &varbuffer);

    g_log.Write("==============================================");
    IA.Start();
    VP.Start();

    IA.Release();
    VP.Release();
    cout<<"main end"<<endl;
}

