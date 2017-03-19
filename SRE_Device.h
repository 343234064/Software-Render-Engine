//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_Device.h
// Date: 2017/03/06
// Description:
//       Defines all Device and platform relative classes and functions
//
//
//
//
//*****************************************************
#ifndef SRE_DEVICE_H_INCLUDED
#define SRE_DEVICE_H_INCLUDED

#define _SRE_PLATFORM_WIN_

#include "SRE_GlobalsAndUtils.h"
#include "SRE_Resources.h"

#ifdef _SRE_PLATFORM_WIN_
#include <windows.h>

/************************************
 *if compile in visual studio, add
 *the following code here:
 *
 *   #pragma comment(lib, "winmm.lib")
 *
 *else if compile in codeblocks, add
 *libgdi32.a to the linker:
 *Setting -> Compiler ->Linker Setting-> Add:
 *
 *   CodeBlocks\MinGW\lib\libgdi32.a
 *
 ************************************/
#endif



namespace SRE {
    //=============================
	//Class DeviceAdapter
	//
	//
	//=============================
	class DeviceAdapter
	{
    public:
        virtual ~DeviceAdapter(){}

        virtual void PresentToScreen(BYTE* colorBuffer, INT width, INT height)=0;
	};


    //=============================
	//Class Device
	//
	//
	//=============================
	class Device
	{
    public:
        Device():
            m_frameWidth(0),
            m_frameHeight(0),
            m_bufferFormat(0),
            m_frameBuffers(),
            m_pDeviceAdapter(nullptr),
            m_front()
        {}
        virtual ~Device()
        {
            ReleaseBuffers();
        }

        RESULT Create(USINT frameBufferNum=2,
                      USINT frameWidth=800,
                      USINT frameHeight=600,
                      SREVAR bufferFomat=SRE_FORMAT_PIXEL_R8G8B8,
                      DeviceAdapter* deviceAdapter=nullptr);
        void ClearFrame(Color3 color);
        void ClearFrame(INT grayLevel);
        void Resize(USINT width, USINT height);
        void Present()
        {
            CLList<Color3*>::Iterator pbuffer = m_front;
            m_front = m_front->next;
            m_pDeviceAdapter->PresentToScreen((BYTE*)pbuffer->data,
                                               m_frameWidth,
                                               m_frameHeight);
        }

        void SetDeviceAdapter(DeviceAdapter* adapter)
        {
            m_pDeviceAdapter = adapter;
        }

        USINT GetFrameBufferNum() const
        {
            return m_frameBuffers.Size();
        }

        Device(const Device & other) = delete;
        Device & operator=(const Device & other) = delete;

    protected:
        void ReleaseBuffers()
        {
            if(m_frameBuffers.Size()>0)
            {
               m_front = m_frameBuffers.Begin();
               do
               {
                  delete[] m_front->data;
                  m_front = m_front->next;

                }while(m_front != m_frameBuffers.Begin());
            }
        }


    protected:
        USINT   m_frameWidth;
        USINT   m_frameHeight;
        SREVAR  m_bufferFormat;

        CLList<Color3*>  m_frameBuffers;
        DeviceAdapter*   m_pDeviceAdapter;

        CLList<Color3*>::Iterator
                         m_front;

	};


#ifdef _SRE_PLATFORM_WIN_
	//=============================
	//Class WindowsAdapter
	//=============================
	class WindowsAdapter:public DeviceAdapter
	{
    public:
        WindowsAdapter(HDC hdc=nullptr, USINT frameWidth=800, USINT frameHeight=600):
            m_hdc(hdc),
            m_bitMapInfo()
        {
            memset(&m_bitMapInfo, 0, sizeof(BITMAPINFO));
            m_bitMapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            m_bitMapInfo.bmiHeader.biPlanes = 1;
            m_bitMapInfo.bmiHeader.biBitCount = 24;
            m_bitMapInfo.bmiHeader.biCompression = BI_RGB;
            m_bitMapInfo.bmiHeader.biWidth = frameWidth;
            m_bitMapInfo.bmiHeader.biHeight = -frameHeight;
        }
        ~WindowsAdapter(){}

        void SetHDC(HDC hdc){m_hdc=hdc;}
        void PresentToScreen(BYTE* colorBuffer, INT width, INT height);

        WindowsAdapter(const WindowsAdapter & other) = delete;
        WindowsAdapter & operator=(const WindowsAdapter & other) = delete;

    private:
        HDC        m_hdc;
        BITMAPINFO m_bitMapInfo;

	};

    //=============================
	//Windows Call back functions
	//
	//
	//=============================
    void SetWndCallBackOnFrame(void (*OnFrame)());
    void SetWndCallBackOnKeyDown(void (*OnKeyDown)(UINT key));
    void SetWndCallBackOnLButtonDown(void (*OnLButtonDown)(INT x, INT y));
    void SetWndCallBackOnRButtonDown(void (*OnRButtonDown)(INT x, INT y));
    void SetWndCallBackOnLButtonUp(void (*OnLButtonUp)(INT x, INT y));
    void SetWndCallBackOnRButtonUp(void (*OnRButtonUp)(INT x, INT y));
    void SetWndCallBackOnMouseMove(void (*OnMouseMove)(INT x, INT y));
    void SetWndCallBackOnResize(void (*OnResize)(INT width, INT height));


    //=============================
	//Class Windows view
	//
	//
	//=============================
    class Window_view
    {
    public:
       Window_view():
           m_hwnd(),
           m_hdc(),
           m_title(nullptr),
           m_winWidth(800),
           m_winHeight(600),
           m_created(false),
           m_winMaximize(false)
        {}
       ~Window_view()
       {
           Destroy();
       }

       bool Create(HINSTANCE hInstance,
                   char* winTitle,
                   INT winWidth,
                   INT winHeight
                   );
       bool Create(HINSTANCE hInstance,
                   char* winTitle,
                   INT winWidth,
                   INT winHeight,
                   WNDPROC wndProc
                   );

       void ShowWindow();
       void MsgLoop();
       void Destroy();

       HDC   GetHDC(){return m_hdc;}
       const char * GetTitle(){return m_title;}
       void  SetTitle(const char * title){SetWindowText(m_hwnd, title);}

       Window_view(const Window_view & other) = delete;
       Window_view & operator=(const Window_view & other) = delete;

    private:
       HWND  m_hwnd;
       HDC   m_hdc;

       const char *
             m_title;
       INT   m_winWidth;
       INT   m_winHeight;

       bool  m_created;

    public:
       bool  m_winMaximize;


   };

#endif

}




#endif // SRE_DEVICE_H_INCLUDED
