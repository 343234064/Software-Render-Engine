//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_Device.cpp
// Date: 2017/03/11
// Description:
//       Implements of device functions and classes which declared
//       in SRE_Device.h
//
//
//*****************************************************
#include "SRE_Device.h"

namespace SRE {
    //=============================
	//Class Device
	//
	//
	//=============================
    RESULT Device::Create(USINT frameWidth,
                                      USINT frameHeight,
                                      SREVAR bufferFormat,
                                      DeviceAdapter* deviceAdapter)
    {
        if(frameWidth<=0) return RESULT::INVALIDARG;
        if(frameHeight<=0) return RESULT::INVALIDARG;
        if(deviceAdapter==nullptr) return RESULT::INVALIDARG;

        //bufferFormat

        RESULT re;
        re = m_frameBuffers[0].Create(frameWidth, frameHeight);
		if(RESULT::SUCC != re)
			return re;

		re = m_frameBuffers[1].Create(frameWidth, frameHeight);
		if(RESULT::SUCC != re)
			return re;

        m_front = 0;
        m_pDeviceAdapter = deviceAdapter;
        m_bufferFormat = bufferFormat;

        return RESULT::SUCC;
    }


    void Device::ClearFrame(DECOLOR color)
    {
        m_frameBuffers[m_front].Clear(color);
    }


    void Device::ClearFrame(INT grayLevel)
    {
        m_frameBuffers[m_front].Clear(grayLevel);
    }


    ////////////////////////////not finish
    RESULT Device::Resize(USINT width, USINT height)
    {
        RESULT re = m_frameBuffers[0].Resize(width, height);

        if(re != RESULT::SUCC)
        {
        	return re;
        }
        else
			return m_frameBuffers[1].Resize(width, height);

    }


	void  Device::Present()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cond.wait(lock, [this]{return m_present;});

		m_present = false;
		m_front = !m_front;

		m_pDeviceAdapter->PresentToScreen((BYTE*)m_frameBuffers[!m_front].Get(),
											                       m_frameBuffers[!m_front].GetWidth(),
											                       m_frameBuffers[!m_front].GetHeight());

	}


	void  Device::PresentReady()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_present = true;

		m_cond.notify_one();
	}


#ifdef _SRE_PLATFORM_WIN_
	//=============================
	//Class WindowsAdapter
	//=============================
	void WindowsAdapter::PresentToScreen(BYTE* colorBuffer, INT width, INT height)
	{
       m_bitMapInfo.bmiHeader.biWidth = width;
       m_bitMapInfo.bmiHeader.biHeight = -height;
       ::StretchDIBits(m_hdc,
                       0, 0,width, height,
                       0, 0,width, height,
                       colorBuffer,
                       &m_bitMapInfo,
                       DIB_RGB_COLORS,
                       SRCCOPY);
	}

    //=============================
	//Class Window Message Callback
	//
	//
	//=============================
    class WndProcCallBack
    {
    public:
       WndProcCallBack():
           OnFrame(nullptr),
           OnKeyDown(nullptr),
           OnLButtonDown(nullptr),
           OnRButtonDown(nullptr),
           OnLButtonUp(nullptr),
           OnRButtonUp(nullptr),
           OnMouseMove(nullptr),
           OnResize(nullptr)
       {}
       void (*OnFrame)();
       void (*OnKeyDown)(UINT key);
       void (*OnLButtonDown)(INT x, INT y);
       void (*OnRButtonDown)(INT x, INT y);
       void (*OnLButtonUp)(INT x, INT y);
       void (*OnRButtonUp)(INT x, INT y);
       void (*OnMouseMove)(INT x, INT y);
       void (*OnResize)(INT width, INT height);
       //void (*OnMouseWheel)(short delta);
    };
    WndProcCallBack g_wndCallBack;

    void SetWndCallBackOnFrame(void(*OnFrame)())
    {
        g_wndCallBack.OnFrame = OnFrame;
    }

    void SetWndCallBackOnKeyDown(void (*OnKeyDown)(UINT key))
    {
        g_wndCallBack.OnKeyDown = OnKeyDown;
    }

    void SetWndCallBackOnLButtonDown(void (*OnLButtonDown)(INT x, INT y))
    {
        g_wndCallBack.OnLButtonDown = OnLButtonDown;
    }

    void SetWndCallBackOnRButtonDown(void (*OnRButtonDown)(INT x, INT y))
    {
        g_wndCallBack.OnRButtonDown = OnRButtonDown;
    }

    void SetWndCallBackOnLButtonUp(void (*OnLButtonUp)(INT x, INT y))
    {
        g_wndCallBack.OnLButtonUp = OnLButtonUp;
    }

    void SetWndCallBackOnRButtonUp(void (*OnRButtonUp)(INT x, INT y))
    {
        g_wndCallBack.OnRButtonUp = OnRButtonUp;
    }

    void SetWndCallBackOnMouseMove(void (*OnMouseMove)(INT x, INT y))
    {
        g_wndCallBack.OnMouseMove = OnMouseMove;
    }

    void SetWndCallBackOnResize(void (*OnResize)(INT width, INT height))
    {
        g_wndCallBack.OnResize = OnResize;
    }

    LRESULT CALLBACK SR_WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
    {
        switch(uiMsg)
        {
            case WM_CLOSE:
                PostQuitMessage(0);
                break;

            case WM_KEYDOWN:
                if(nullptr != g_wndCallBack.OnKeyDown)
                   g_wndCallBack.OnKeyDown((UINT)wParam);
                break;

            case WM_LBUTTONDOWN:
                if(nullptr != g_wndCallBack.OnLButtonDown)
                   g_wndCallBack.OnLButtonDown(LOWORD(lParam), HIWORD(lParam));
                break;

            case WM_LBUTTONUP:
                if(nullptr != g_wndCallBack.OnLButtonUp)
                   g_wndCallBack.OnLButtonUp(LOWORD(lParam), HIWORD(lParam));
                break;

            case WM_MOUSEMOVE:
                if(nullptr != g_wndCallBack.OnMouseMove)
                   g_wndCallBack.OnMouseMove(LOWORD(lParam), HIWORD(lParam));
                break;
            /*
            case WM_MOUSWHEEL=0x020A:
                break;
            */
            case WM_PAINT:
            {
                PAINTSTRUCT ps;
                BeginPaint(hWnd, &ps);
                if(nullptr != g_wndCallBack.OnFrame)
                   g_wndCallBack.OnFrame();
                EndPaint(hWnd, &ps);
                InvalidateRect(hWnd, nullptr, FALSE);
                break;
            }

            case WM_RBUTTONDOWN:
                if(nullptr != g_wndCallBack.OnRButtonDown)
                   g_wndCallBack.OnRButtonDown(LOWORD(lParam), HIWORD(lParam));
                break;

            case WM_RBUTTONUP:
                if(nullptr != g_wndCallBack.OnRButtonUp)
                   g_wndCallBack.OnRButtonUp(LOWORD(lParam), HIWORD(lParam));
                break;

            case WM_SIZE:
                if(nullptr != g_wndCallBack.OnResize)
                   g_wndCallBack.OnResize(LOWORD(lParam), HIWORD(lParam));
                break;

            default:
                return DefWindowProc(hWnd, uiMsg, wParam, lParam);
        }

        return 0;
    }

    //=============================
	//Class Windows view functions
	//
	//
	//=============================
    bool Window_view::Create(HINSTANCE hInstance,
                             char* winTitle,
                             INT winWidth,
                             INT winHeight)
    {
        WNDCLASSEX wndClassEx;
        memset(&wndClassEx, 0, sizeof(WNDCLASSEX));

        wndClassEx.cbSize = sizeof(WNDCLASSEX);
        wndClassEx.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
        wndClassEx.lpfnWndProc = &SR_WndProc;
        wndClassEx.hInstance = hInstance;
        wndClassEx.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        wndClassEx.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
        wndClassEx.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wndClassEx.lpszClassName = "SRE_WINDOW";

        if(RegisterClassEx(&wndClassEx) == 0)
        {
#ifdef _SRE_DEBUG_
            _ERRORMSG("Register Windows Class failed!");
#endif
            return false;
        }

        m_hwnd = CreateWindowEx(WS_EX_APPWINDOW,
                                wndClassEx.lpszClassName,
                                winTitle,
                                WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                                0, 0,
                                winWidth, winHeight,
                                nullptr,
                                nullptr,
                                hInstance,
                                nullptr);
        if(nullptr == m_hwnd)
        {
#ifdef _SRE_DEBUG_
            _ERRORMSG("Create Windows failed!");
#endif
            return false;
        }

        m_hdc = ::GetDC(m_hwnd);
        if(nullptr == m_hdc)
        {
#ifdef _SRE_DEBUG_
            _ERRORMSG("Get DC failed!");
#endif
            return false;
        }

        m_winWidth = winWidth;
        m_winHeight = winHeight;
        m_title = winTitle;
        m_created = true;

        return true;
    }

    bool Window_view::Create(HINSTANCE hInstance,
                             char* winTitle,
                             INT winWidth,
                             INT winHeight,
                             WNDPROC wndProc)
    {
        WNDCLASSEX wndClassEx;
        memset(&wndClassEx, 0, sizeof(WNDCLASSEX));

        wndClassEx.cbSize = sizeof(WNDCLASSEX);
        wndClassEx.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
        wndClassEx.lpfnWndProc = wndProc;
        wndClassEx.hInstance = hInstance;
        wndClassEx.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        wndClassEx.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
        wndClassEx.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wndClassEx.lpszClassName = "SRE_WINDOW";

        if(RegisterClassEx(&wndClassEx) == 0)
        {
#ifdef _SRE_DEBUG_
            _ERRORMSG("Register Windows Class failed!");
#endif
            return false;
        }

        m_hwnd = CreateWindowEx(WS_EX_APPWINDOW,
                                wndClassEx.lpszClassName,
                                winTitle,
                                WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                                0, 0,
                                winWidth, winHeight,
                                nullptr,
                                nullptr,
                                hInstance,
                                nullptr);
        if(nullptr == m_hwnd)
        {
#ifdef _SRE_DEBUG_
            _ERRORMSG("Create Windows failed!");
#endif
            return false;
        }

        m_hdc = ::GetDC(m_hwnd);
        if(nullptr == m_hdc)
        {
#ifdef _SRE_DEBUG_
            _ERRORMSG("Get DC failed!");
#endif
            return false;
        }

        m_winWidth = winWidth;
        m_winHeight = winHeight;
        m_created = true;

        return true;
    }


    void Window_view::ShowWindow()
    {
        RECT desktopSize, clientSize, winSize;
        GetWindowRect(GetDesktopWindow(), &desktopSize);
        GetClientRect(m_hwnd, &clientSize);
        GetWindowRect(m_hwnd, &winSize);

        winSize.right  += m_winWidth  - clientSize.right;
        winSize.bottom += m_winHeight - clientSize.bottom;

        winSize.right  -= winSize.left;
        winSize.bottom -= winSize.top;

        winSize.left = (desktopSize.right  - winSize.right)/2;
        winSize.top  = (desktopSize.bottom - winSize.bottom)/2;

        ::MoveWindow(m_hwnd, winSize.left, winSize.top, winSize.right, winSize.bottom, FALSE);
        ::ShowWindow(m_hwnd, m_winMaximize ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);
    }


    void Window_view::MsgLoop()
    {
        MSG msg;
        while(GetMessage(&msg, nullptr, 0, 0)>0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }


    void Window_view::Destroy()
    {
        if(m_created)
        {
            DestroyWindow(m_hwnd);
            m_created = false;
        }
    }

#endif
}
