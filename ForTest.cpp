#include <iostream>
#include <sstream>
#include <stdio.h>
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

    friend std::ostream& operator<<(std::ostream& out, const vertex& s)
    {
        out<<s.a1;
        return out;
    }
};

VSOutput* myVS(BYTE* v, VariableBuffer* varbuffer)
{
    VSOutput * out = new VSOutput();

    vertex* ver = (vertex*)v;
    out->vertex = ver->ver;
    out->vertex.w = 1.0f;
    out->normal = ver->nor;
    out->texcoord.x = ver->a1;
    out->texcoord.y = ver->a2;

    return out;
}


/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
Window_view    main_view;
Device         main_device;
WindowsAdapter win_adapter;

void OnResize(INT width, INT height)
{
    main_device.Resize(width, height);;
}

void OnFrame()
{
    static DWORD lastTime = GetTickCount();
    static INT FPS = 0;
    DWORD nowTime = GetTickCount();

    if(nowTime - lastTime > 1000)
    {
        std::string text = "SoftwareEngine -ver0.01 -FPS:";
        std::string sFPS;
        std::stringstream sstream;
        sstream <<FPS;
        sstream >>sFPS;
        main_view.SetTitle((text+sFPS).data());

        lastTime = nowTime;
        FPS = 0;
    }
    else
        FPS++;

    main_device.ClearFrame(0);

    //

    main_device.Present();
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR sCmdLine, int iShow)
{
    USINT width = 800, height = 600;

    if(main_view.Create(hInstance, "SoftwareEngine -ver0.01 -FPS:0", width, height))
    {
        win_adapter.SetHDC(main_view.GetHDC());
        RESULT re = main_device.Create(3, width, height, &win_adapter);
        if(re == RESULT::SUCC)
        {
           SetWndCallBackOnResize(&OnResize);
           SetWndCallBackOnFrame(&OnFrame);

           main_view.ShowWindow();
           main_view.MsgLoop();
        }
    }
    else
    {
        MessageBox(nullptr, "ERROR", "Create Window Failed", MB_OK | MB_ICONERROR);
    }

    main_view.Destroy();

    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

/*
int main()
{
    CLList<vertex> mylist;
    vertex vertexes[2];
    for(int i=0; i<2; i++)
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
    cout<<mylist.Empty()<<endl;
    for(int i=0; i<2; i++)
       mylist.Add_back(vertexes[i]);

    cout<<mylist.Empty()<<endl;
    cout<<"size:"<<mylist.Size()<<endl;

    CLList<vertex>::Iterator it=mylist.Begin();
    do
    {
        vertex* va = &(it->data);
        vertex* vb = &(it->next->data);
        it=it->next;

        cout<<va->a1<<endl;
        cout<<vb->a1<<endl;


    }while(it!=mylist.Begin());

    mylist.Clear();
    cout<<"size:"<<mylist.Size()<<endl;

    CLList<int> mylist2;
    cout<<sizeof(mylist)<<endl;
    cout<<sizeof(mylist2)<<endl;

}
*/

/*
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
        vertexes[0].ver = VEC3(-1.5, 1.5, 0.5);
        vertexes[1].ver = VEC3( 1.5, 1.5, 0.5);
        vertexes[2].ver = VEC3( 0.5, 2.5, 1.0);

        vertexes[3].ver = VEC3( 0.5, 1.0, 0.5);
        vertexes[4].ver = VEC3( 0.5, 0.0, 0.5);
        vertexes[5].ver = VEC3(-0.5, 0.5, 0.5);

        vertexes[6].ver = VEC3(-2.0, 0.0, 0.5);
        vertexes[7].ver = VEC3( 1.0,-1.0, 0.5);
        vertexes[8].ver = VEC3( 2.5, 0.5, 0.5);



    VertexBuffer * vbuffer;

    RESULT re=CreateVertexBuffer(10, sizeof(vertex), SRE_FORMAT_VERTEX_XYZ | SRE_FORMAT_ATTRIBUTE_NORMAL | SRE_FORMAT_ATTRIBUTE_OTHER2,
                                 (BYTE*)vertexes, &vbuffer);

    if(re == RESULT::SUCC)
        cout<<"succ"<<endl;
    else
        cout<<"fail"<<endl;

    //INT index[13]={0,1,2,3,6,-1,9,7,5,4,0,2,3};
    INT index[3]={3,4,5};

    Buffer<INT> ibuffer(3, index);

    BasicIOBuffer<BasicIOElement*> IAoutputBuffer;
    BasicIOBuffer<BasicIOElement*> VPoutputBuffer;
    BasicIOBuffer<BasicIOElement*> PAoutputBuffer;
    BasicIOBuffer<BasicIOElement*> VPPoutputBuffer;
    BasicIOBuffer<BasicIOElement*> RZoutputBuffer;

    Observer observer;
    ConstantBuffer conbuffer;
    VariableBuffer varbuffer;
    conbuffer.primitiveTopology = SRE_PRIMITIVETYPE_TRIANGLELIST;

    InputAssembler IA(&IAoutputBuffer, &observer, &conbuffer);
    IA.SetVertexAndIndexBuffers(vbuffer, &ibuffer);

    CallBackVShader* callback = &myVS;
    VertexShader vshader(callback);
    VertexProcessor VP(&IAoutputBuffer, &VPoutputBuffer, &observer, &vshader, &varbuffer);

    PrimitiveAssembler PA(&VPoutputBuffer, &PAoutputBuffer, &observer);

    VertexPostProcessor VPP(&PAoutputBuffer, &VPPoutputBuffer, &observer);

    Rasterizer RZ(&VPPoutputBuffer, &RZoutputBuffer, &observer, &conbuffer);
    RZ.AddSubProcessor(4);


    g_log.Write("==============================================");
    IA.Start();
    VP.Start();
    PA.Start();
    VPP.Start();
    RZ.Start();

    IA.Join();
    VP.Join();
    PA.Join();
    VPP.Join();
    RZ.Join();
    RZ.CancelSubProcessor();
    cout<<"main end"<<endl;
}
*/
