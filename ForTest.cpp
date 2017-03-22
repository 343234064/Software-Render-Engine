#include <iostream>
#include <sstream>
#include <stdio.h>
#include "SoftRenderEngine.h"

using namespace SRE;
using  std::cout;
using  std::endl;
using  std::unique_ptr;

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

Color4 myPS(PSInput & in)
{
	return Color4(0,0,255,1);
}

class PileLine: public ObserverCallBack
{
public:
	 PileLine():
	 	IAoutputBuffer(),
	 	VPoutputBuffer(),
	 	PAoutputBuffer(),
	 	VPPoutputBuffer(),
	 	RZoutputBuffer(),
	 	observer(this),
	 	conbuffer(),
	 	varbuffer(),
	 	vbuffer(),
	 	ibuffer(3),
	 	vshader(),
	 	pshader(),
	 	IA(1, &IAoutputBuffer, &observer),
	 	VP(2, &IAoutputBuffer, &VPoutputBuffer, &observer),
	 	PA(3, &VPoutputBuffer, &PAoutputBuffer, &observer),
	 	VPP(4, &PAoutputBuffer, &VPPoutputBuffer, &observer),
	 	RZ(5, &VPPoutputBuffer, &RZoutputBuffer, &observer),
	 	OM(6, &RZoutputBuffer, &observer)
     {}
     ~PileLine()
     {

     }

 	bool Init();
 	void Run();
 	void Cancel();
 	void Render();
 	void HandleMessage(SREVAR message, USINT id);

public:

BasicIOBuffer<BasicIOElement> IAoutputBuffer;
BasicIOBuffer<BasicIOElement> VPoutputBuffer;
BasicIOBuffer<BasicIOElement> PAoutputBuffer;
BasicIOBuffer<BasicIOElement> VPPoutputBuffer;
BasicIOBuffer<BasicIOElement> RZoutputBuffer;

BasicObserver observer;
ConstantBuffer conbuffer;
VariableBuffer varbuffer;
SynMatBuffer<FLOAT> zbuffer;
RenderTexture renderTarget;

VertexBuffer vbuffer;
Buffer<INT> ibuffer;

VertexShader vshader;
PixelShader pshader;

InputAssembler IA;
VertexProcessor VP;
PrimitiveAssembler PA;
VertexPostProcessor VPP;
Rasterizer RZ;
OutputMerger OM;
};
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
Window_view      main_view;
Device                main_device;
WindowsAdapter win_adapter;
PileLine               main_pileline;

void PileLine::Run()
{
    IA.Start();
    VP.Start();
    PA.Start();
    VPP.Start();
    RZ.Start();
    OM.Start();

}

void PileLine::Cancel()
{

	IA.Cancel();
    VP.Cancel();
    PA.Cancel();
    VPP.Cancel();
    RZ.Cancel();
    OM.Cancel();

    OM.Join();
    RZ.Join();
    VPP.Join();
    PA.Join();
    VP.Join();
    IA.Join();
}

void PileLine::HandleMessage(SREVAR message, USINT id)
{
	if(message == SRE_MESSAGE_ENDSCENE)
		main_device.PresentReady();
}


bool PileLine::Init()
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

    RESULT re=CreateVertexBuffer(10, sizeof(vertex), SRE_FORMAT_VERTEX_XYZ | SRE_FORMAT_ATTRIBUTE_NORMAL | SRE_FORMAT_ATTRIBUTE_OTHER2,
                                 (BYTE*)vertexes, &vbuffer);

    if(re != RESULT::SUCC)
       return false;

    //INT index[13]={0,1,2,3,6,-1,9,7,5,4,0,2,3};
    INT index[3]={3,4,5};
    ibuffer.ResetData(index, 3);

    VP.SetVariableBuffer(&varbuffer);

    RZ.AddSubProcessors(4);
    RZ.SetConstantBuffer(&conbuffer);
    RZ.SetZbuffer(&zbuffer);
    RZ.SetRenderTarget(&renderTarget);

	OM.SetConstantBuffer(&conbuffer);
    OM.SetFrameBuffer(main_device.GetFrameBuffer());

    return true;
}

void PileLine::Render()
{
     vshader.SetCallBackShader(&myVS);
     pshader.SetCallBackShader(&myPS);
     pshader.InputFormat = SRE_SHADERINPUTFORMAT_ALL;

     zbuffer.Reset(1.0);
     renderTarget.Clear(255);

     conbuffer.ZEnable = SRE_TRUE;
     conbuffer.CullEnable = SRE_FALSE;

     IA.BeginSceneSetting();

	 IA.SetVertexAndIndexBuffers(&vbuffer, &ibuffer);

	 VP.SetVertexShader(&vshader);
     RZ.SetPixelShader(&pshader);
     RZ.SetRenderTarget(&renderTarget);
     OM.SetOutputToFrameBuffer(true);

     IA.EndSceneSetting();
}



void OnResize(INT width, INT height)
{
    //main_device.Resize(width, height);;
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

    main_pileline.Render();

    main_device.Present();
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR sCmdLine, int iShow)
{
    USINT width = 800, height = 600;

    g_log.Write("===================================\n");

    if(!main_pileline.Init())
	{
		MessageBox(nullptr, "ERROR", "Create PileLine Failed", MB_OK | MB_ICONERROR);
		return 0;
	}

	main_pileline.Run();
    main_pileline.Cancel();
	/*
    if(main_view.Create(hInstance, "SoftwareEngine -ver0.01 -FPS:0", width, height))
    {
        win_adapter.SetHDC(main_view.GetHDC());

        RESULT re = main_device.Create(width, height, SRE_FORMAT_PIXEL_R8G8B8, &win_adapter);
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

    main_pileline.Cancel();
    main_view.Destroy();
    */
    return 0;
}



