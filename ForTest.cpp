#include <iostream>
#include <sstream>
#include <stdio.h>
#include "SoftRenderEngine.h"

using namespace SRE;
using  std::cout;
using  std::endl;

static FLOAT frameTime=0;


struct vertex
{
    VEC3     ver;
    VEC3     nor;
    VEC2     tex;
    Color3   color;

};

VSOutput* myVS(BYTE* v, VariableBuffer* varbuffer)
{
    VSOutput * out = new VSOutput();

    vertex* ver = (vertex*)v;
    out->vertex = ver->ver;
    out->vertex.w = 1.0f;

    out->normal = ver->nor;
    out->texcoord = ver->tex;
    out->color = ver->color;

    //MAT44 project=MatrixProjectPerspective(800, 600, 0.0f, 1.0f);
    //out->vertex = Multiply(out->vertex,  project);


    return out;
}

Color4 myPS(PSInput & in)
{
	return in.color;
}


/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
class Global:public ObserverCallBack
{
public:
   Global():
      pileLineObserver(this)
   {}
   ~Global(){}

   Window_view      main_view;
   Device                main_device;
   WindowsAdapter win_adapter;
   SREPipeLine         main_pipeline;
   VertexShader      vshader;
   PixelShader         pshader;

   VertexBuffer        vertexbuffer;
   Buffer<INT>        indexbuffer;

   BasicObserver      pileLineObserver;

public:
   void HandleMessage(SREVAR message, USINT id)
   {
      if(message == SRE_MESSAGE_ENDSCENE)
         main_device.PresentReady();
   }
};
Global global;



bool SceneInit()
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
    }
        vertexes[0].ver = VEC3(-3, 1, 0.5);
        vertexes[1].ver = VEC3( 3, 1, 0.5);
        vertexes[2].ver = VEC3( 0, -3, 0.5);

        vertexes[3].ver = VEC3( 0.5, 1.0, 0.5);
        vertexes[4].ver = VEC3( 0.5, 0.0, 0.5);
        vertexes[5].ver = VEC3(-0.5, 0.5, 0.5);

        vertexes[3].color = Color4(255, 0, 0);
        vertexes[4].color = Color4(0, 255, 0);
        vertexes[5].color = Color4(0, 0, 255);


    RESULT re=CreateVertexBuffer(10, sizeof(vertex), SRE_FORMAT_VERTEX_XYZ | SRE_FORMAT_ATTRIBUTE_NORMAL | SRE_FORMAT_ATTRIBUTE_TEXCOORDUV | SRE_FORMAT_ATTRIBUTE_DIFFUSE,
                                 (BYTE*)vertexes, &global.vertexbuffer);

    if(re != RESULT::SUCC)
       return false;

    //INT index[13]={0,1,2,3,6,-1,9,7,5,4,0,2,3};
    INT index[3]={3,4,5};
    global.indexbuffer.ResetBuffer(index, 3);

    global.vshader.SetCallBackShader(&myVS);
    global.pshader.SetCallBackShader(&myPS);

    global.main_pipeline.SetObserver(&global.pileLineObserver);

    return true;
}

void OnRender()
{
     global.main_pipeline.SceneBegin();

     global.main_pipeline.SetVertexBufferAndIndexBuffer(&global.vertexbuffer, &global.indexbuffer);

     global.main_pipeline.constbuffer.ZEnable = SRE_TRUE;
     global.main_pipeline.constbuffer.primitiveTopology = SRE_PRIMITIVETYPE_TRIANGLELIST;

     global.main_pipeline.ResetZbuffer(1.0);
     global.main_pipeline.SetVertexShader(&global.vshader);
     global.main_pipeline.SetPixelShader(&global.pshader);
     global.main_pipeline.SetSamplePixelBlockSize(192);
     global.main_pipeline.SetSampleStep(1);
     global.main_pipeline.SetRenderTarget(global.main_device.GetFrontFrameBuffer());

     global.main_pipeline.SceneEnd();

}



void OnResize(INT width, INT height)
{
    //main_device.Resize(width, height);;
}

RTCOLOR* colorbuffer;

void OnFrame()
{
    static DWORD lastTime = GetTickCount(), lastFrameTime = lastTime;
    static INT FPS = 0;
    DWORD nowTime = GetTickCount();

    frameTime = (nowTime - lastFrameTime)*0.001f;

    if(nowTime - lastTime > 1000)
    {
        std::string text = "SoftwareEngine -ver0.01 -FPS:";
        std::string sFPS;
        std::stringstream sstream;
        sstream <<FPS;
        sstream >>sFPS;
        global.main_view.SetTitle((text+sFPS).data());

        lastTime = nowTime;
        FPS = 0;
    }
    else
        FPS++;



   static bool once = true;

  // if(once){
	global.main_device.ClearFrame(255);

 	OnRender();

	global.main_device.Present();
	 g_log.Write("device presented");
	//BasicIOBuffer 改为传指针
   //once = false;
   //}
   /*
   static int factor=0, step=1;
   factor++;
   if(factor>255||factor<0) step = -step;

   for(int i=0;i<800*600;i++)
      colorbuffer[i] = Color4(0,factor,255,0);

      BITMAPINFO m_bitMapInfo;
            memset(&m_bitMapInfo, 0, sizeof(BITMAPINFO));
            m_bitMapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            m_bitMapInfo.bmiHeader.biPlanes = 1;
            m_bitMapInfo.bmiHeader.biBitCount = 32;
            m_bitMapInfo.bmiHeader.biCompression = BI_RGB;
            m_bitMapInfo.bmiHeader.biWidth = 800;
            m_bitMapInfo.bmiHeader.biHeight = -600;
     ::StretchDIBits(global.main_view.GetHDC(),
                       0, 0,800, 600,
                       0, 0,800, 600,
                       colorbuffer,
                       &m_bitMapInfo,
                       DIB_RGB_COLORS,
                       SRCCOPY);
     */
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR sCmdLine, int iShow)
{
    USINT width = 800, height = 600;

    //g_log.Write("===================================\n");
    colorbuffer = new RTCOLOR[width*height];

    if(global.main_view.Create(hInstance, "SoftwareEngine -ver0.01 -FPS:0", width, height))
    {
	      global.win_adapter.SetHDC(global.main_view.GetHDC());
	      RESULT re = global.main_device.Create(width, height, SRE_FORMAT_PIXEL_R8G8B8, &global.win_adapter);
	      if(re != RESULT::SUCC)
	      {
		      MessageBox(nullptr, "ERROR", "Create Device Failed", MB_OK | MB_ICONERROR);
		      return 0;
         }

         if(global.main_pipeline.Init(width, height, true) != RESULT::SUCC)
         {
		      MessageBox(nullptr, "ERROR", "Create PipeLine Failed", MB_OK | MB_ICONERROR);
		      return 0;
         }

         if(!SceneInit())
         {
		      MessageBox(nullptr, "ERROR", "Scene Init Failed", MB_OK | MB_ICONERROR);
		      return 0;
         }

        global.main_pipeline.Run();

		  SetWndCallBackOnResize(&OnResize);
        SetWndCallBackOnFrame(&OnFrame);

        global.main_view.ShowWindow();
		  global.main_view.MsgLoop();

    }
    else
    {
        MessageBox(nullptr, "ERROR", "Create Window Failed", MB_OK | MB_ICONERROR);
    }

    global.main_pipeline.Cancel();
    global.main_view.Destroy();

    return 0;
}



