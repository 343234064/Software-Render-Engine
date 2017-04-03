//*****************************************************
//
// Software Render Engine
// Version 0.01 by XJL
//
// File: Example.cpp
// Date: 2017/04/01
// Description:
//       An example program
//
//*****************************************************
#include <iostream>
#include <sstream>
#include <stdio.h>
#include "SoftRenderEngine.h"

using namespace SRE;
using  std::cout;
using  std::endl;

struct vertex
{
    VEC3     ver;
    VEC2     tex;
    VEC3     nor;
    Color3   color;

};

struct vertex_noc
{
    VERTEX3     ver;
    VEC2     tex;
    VEC3     nor;

};

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

   Mesh                  object;

   BasicObserver     pileLineObserver;

public:
   void HandleMessage(SREVAR message, USINT id)
   {
      if(message == SRE_MESSAGE_ENDSCENE)
         main_device.PresentReady();
   }
};

Global global;
Mesh Triangle;

const USINT frame_width = 800;
const USINT frame_height = 600;

FLOAT frameTime=0;
/********************************************
 *After view transform,
 *the min z value will be 0,
 *the max z value will be the distance between the farthest
 *object and camera
 *
 *After project transform,
 *the z value which in range (znear, zfar) will be transformed into range (0, 1)
 *when z = znear, z-projected = 0, z<znear, z-projected<0
 *when z = zfar,   z-projected = 1, z>zfar,   z-projected>1
/********************************************/
FLOAT znear = 1.0f;
FLOAT zfar = 10.0f;

VEC3 camera_pos = VEC3(0.0f, 1.5f, 2.0f);
VEC3 camera_lookat = VEC3(0.0f, 0.0f, 0.0f);
VEC3 camera_up = VEC3(0.0f, 1.0f, 0.0f);

/********************************
 *camera view's aspect ratio
 *must equals to the window's
 *
 *the window size is 800x600 pixels,
 *so the camera view can be 4.0x3.0 units
 *in world space
/********************************/
VEC2 camera_view =  VEC2(4.0f, 3.0f);

MAT44 view = MatrixViewLookAt(camera_pos , camera_lookat, camera_up);
MAT44 project = MatrixProjectPerspective(camera_view.x, camera_view.y, znear, zfar);
MAT44 viewproject = Multiply(view, project);

FLOAT rotateFactor = 1.0f;
FLOAT rotateAngle = PI/4.0f;



VSOutput* myVS(BYTE* v, VariableBuffer* varbuffer)
{
    VSOutput * out = new VSOutput();
    //vertex* ver = (vertex*)v;
    vertex_noc* ver = (vertex_noc*)v;

    out->vertex = ver->ver;
    out->vertex.w = 1.0f;
    out->normal = ver->nor;
    out->texcoord = ver->tex;

    MAT44 world = MatrixRotationY(rotateAngle*rotateFactor);
    out->vertex = Multiply(out->vertex, world);
    out->vertex = Multiply(out->vertex, viewproject);

    return out;
}


Color4 myPS(PSInput & in)
{
	return Color4(255,0,0,0);
}


bool SceneInit()
{
    char* filePath = ".\\Cube.obj";
    if(RESULT::SUCC != LoadObjMesh(filePath, &global.object))
       return false;

    vertex vertexs[3];
    vertexs[0].ver = VERTEX3(-0.5, 0.5, 0.0);
    vertexs[1].ver = VERTEX3(  0.5, 0.5, 0.0);
    vertexs[2].ver = VERTEX3(  0.5, -0.5, 0.0);
    vertexs[0].color = Color4(255, 0, 0, 0);
    vertexs[1].color = Color4(0, 255, 0, 0);
    vertexs[2].color = Color4(0, 0, 255, 0);

    if(RESULT::SUCC != CreateVertexBuffer(3, sizeof(vertex), SRE_FORMAT_VERTEX_XYZ, vertexs, &(Triangle.vertexes)))
       return false;

    global.vshader.SetCallBackShader(&myVS);
    global.pshader.SetCallBackShader(&myPS);

    global.main_pipeline.SetSamplePixelBlockSize(300);
    global.main_pipeline.SetSampleStep(1);
    global.main_pipeline.SetClipZValue(znear, zfar);

    global.main_pipeline.SetObserver(&global.pileLineObserver);

    return true;
}

void OnRender()
{
     global.main_pipeline.SceneBegin();

     rotateFactor += 0.1f;
     //global.main_pipeline.SetVertexBufferAndIndexBuffer(&Triangle.vertexes, nullptr);
     global.main_pipeline.SetVertexBufferAndIndexBuffer(&global.object.vertexes, &global.object.indexes);

     global.main_pipeline.constbuffer.ZEnable = SRE_TRUE;
     global.main_pipeline.constbuffer.ClipEnable = SRE_TRUE;
     global.main_pipeline.constbuffer.CullEnable = SRE_FALSE;
     global.main_pipeline.constbuffer.primitiveTopology = SRE_PRIMITIVETYPE_TRIANGLELIST;

     global.main_pipeline.ResetZbuffer(zfar);
     global.main_pipeline.SetVertexShader(&global.vshader);
     global.main_pipeline.SetPixelShader(&global.pshader);
     global.main_pipeline.SetRenderTarget(global.main_device.GetFrontFrameBuffer());

     global.main_pipeline.SceneEnd();

}



void OnResize(INT width, INT height)
{
    //main_device.Resize(width, height);;
}


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

   //if(once){
	global.main_device.ClearFrame(255);

 	OnRender();

	global.main_device.Present();
	g_log.Write("Present End");
	once = false;
   //}

}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR sCmdLine, int iShow)
{
    g_log.Write("===================================\n");

    if(global.main_view.Create(hInstance, "SoftwareEngine -ver0.01 -FPS:0", frame_width, frame_height))
    {
	      global.win_adapter.SetHDC(global.main_view.GetHDC());
	      RESULT re = global.main_device.Create(frame_width, frame_height, SRE_FORMAT_PIXEL_R8G8B8, &global.win_adapter);
	      if(re != RESULT::SUCC)
	      {
		      MessageBox(nullptr, "ERROR", "Create Device Failed", MB_OK | MB_ICONERROR);
		      return 0;
         }

         if(global.main_pipeline.Init(frame_width, frame_height, true) != RESULT::SUCC)
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



