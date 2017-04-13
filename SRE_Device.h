//*****************************************************
//
// Software Render Engine
// Version 0.01 by XJL
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

#include "SRE_GlobalsAndUtils.h"
#include "SRE_Resources.h"



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
            m_msgbuffer(),
            m_framebuffers(),
            m_front(nullptr),
            m_pDeviceAdapter(nullptr)
        {}

        virtual ~Device()
        {
             RemoveFrameBuffers();
        }

        RESULT Create(USINT frameWidth=800,
                              USINT frameHeight=600,
                              USINT framebufferNum=3,
                              DeviceAdapter* deviceAdapter=nullptr);
        RESULT  Resize(USINT width, USINT height);
		  void     ClearFrame(DECOLOR color);
        void     ClearFrame(INT grayLevel);
        void     Present();
        void     PresentReady();
        void     PresentError();
        RenderTexture*  GetFrontFrameBuffer();
        RenderTexture*  GetNextFrameBuffer();
        void     RemoveFrameBuffers();

        inline void   SetDeviceAdapter(DeviceAdapter* adapter);
        inline USINT  GetWidth() const;
        inline USINT  GetHeight() const;
        inline USINT  GetFrameBufferNum() const;

        Device(const Device & other) = delete;
        Device & operator=(const Device & other) = delete;

	 private:
		  BasicIOBufferEx<SREVAR>            m_msgbuffer;

    protected:
        CLList<RenderTexture*>              m_framebuffers;
        CLList<RenderTexture*>::Iterator m_front;
        DeviceAdapter*                            m_pDeviceAdapter;

	};


	void Device::SetDeviceAdapter(DeviceAdapter* adapter)
	{
		m_pDeviceAdapter = adapter;
	}

    USINT Device::GetWidth() const
    {
    	return m_front->data->GetWidth();
    }

    USINT Device::GetHeight() const
    {
    	return m_front->data->GetHeight();
    }

    USINT Device::GetFrameBufferNum() const
    {
       return m_framebuffers.Size();
    }


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
            m_bitMapInfo.bmiHeader.biBitCount = 32;
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
        HDC            m_hdc;
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
       HDC    m_hdc;

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
