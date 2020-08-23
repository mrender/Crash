    
#include "GameWindow.h"
#include "Application.h"
#include "OpenGL/OpenGLExtensions.h"
#include "../GameDev2D_Settings.h"
#include "../Events/ResizeEvent.h"
#include "../Events/FullscreenEvent.h"
#include "../Services/Services.h"
#include "../Services/InputManager/InputManager.h"
#include "resource.h"
#include <Xinput.h>


namespace GameDev2D
{
    GameWindow::GameWindow() :
        m_WindowHandle(nullptr),
        m_DeviceContext(nullptr),
        m_OpenGLContext(nullptr),
        m_Width(0),
        m_Height(0),
        m_IsMouseCursorVisible(true),
        m_IsFullScreen(false),
        m_IsVerticalSyncEnabled(false),
        m_LastGamePadDetection(0.0)
    {
        //Get the current hardware instance
        HINSTANCE currentInstance = GetModuleHandle(NULL);

        //Initialize the window class data and register it
        WNDCLASSEX windowClassExtended;
        windowClassExtended.cbSize          = sizeof(WNDCLASSEX);
        windowClassExtended.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        windowClassExtended.lpfnWndProc	    = &GameWindow::EventHandler;
        windowClassExtended.cbClsExtra		= 0;
        windowClassExtended.cbWndExtra		= 0;
        windowClassExtended.hInstance       = currentInstance;
        windowClassExtended.hIcon			= LoadIcon(currentInstance, MAKEINTRESOURCE(IDI_MYICON));
        windowClassExtended.hCursor		    = LoadCursor(NULL, IDC_ARROW);
        windowClassExtended.hbrBackground	= NULL;
        windowClassExtended.lpszMenuName	= NULL;
        windowClassExtended.lpszClassName	= L"GameDev2D";
        windowClassExtended.hIconSm		    = LoadIcon(currentInstance, MAKEINTRESOURCE(IDI_MYICON));
        RegisterClassEx(&windowClassExtended);

        //Create the window, if the assert is hit, we couldn't create a window
        m_WindowHandle = CreateWindowEx(WS_EX_APPWINDOW, L"GameDev2D", L"", WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, currentInstance, this);
        assert(m_WindowHandle != nullptr);

        //Get the device context, if the assert is hit, then we couldn't get a device context
        m_DeviceContext = GetDC(m_WindowHandle);
        assert(m_DeviceContext != nullptr);

        //The pixel format descriptor tells Windows how we want our OpenGL context to be setup
        static PIXELFORMATDESCRIPTOR pixelFormatDescriptor =
        {
            sizeof(PIXELFORMATDESCRIPTOR),  // Size Of This Pixel Format Descriptor
            1,                              // Version Number
            PFD_DRAW_TO_WINDOW |            // Format Must Support Window
              PFD_SUPPORT_OPENGL |          // Format Must Support OpenGL
              PFD_DOUBLEBUFFER,             // Must Support Double Buffering
            PFD_TYPE_RGBA,                  // Request An RGBA Format
            32,                             // Select Our Color Depth
            0, 0, 0, 0, 0, 0,               // Color Bits Ignored
            0,                              // No Alpha Buffer
            0,                              // Shift Bit Ignored
            0,                              // No Accumulation Buffer
            0, 0, 0, 0,                     // Accumulation Bits Ignored
            32,                             // Bits for Z-Buffer (Depth Buffer)
            0,                              // Stencil bits
            0,                              // No Auxiliary Buffer
            PFD_MAIN_PLANE,                 // Main Drawing Layer
            0,                              // Reserved
            0, 0, 0                         // Layer Masks Ignored
        };

        //Choose the pixel format, based on the pixel format descriptor. If the assert fails it means the supplied pixel format descriptor is invalid on this system
        unsigned int pixelFormat = ChoosePixelFormat(m_DeviceContext, &pixelFormatDescriptor);
        assert(pixelFormat != 0);

        //Set the pixel format, if the assert is hit, it means the pixel format supplied is invalid on this system
        BOOL success = SetPixelFormat(m_DeviceContext, pixelFormat, &pixelFormatDescriptor);
        assert(success == TRUE);

        //Create an opengl context, this context will be version 2.1, and is temporary. 
        //If the assert is hit, it means creating an OpenGL context failed
        m_OpenGLContext = wglCreateContext(m_DeviceContext);
        assert(m_OpenGLContext != nullptr);
    
        //Make the current opengl context current
        wglMakeCurrent(m_DeviceContext, m_OpenGLContext);

        //Show the window and update it
        ShowWindow(m_WindowHandle, SW_SHOWDEFAULT);
        UpdateWindow(m_WindowHandle);

        //Initialize the OpenGL extensions
        InitOpenGLExtensions();

        //Create the OpenGL context
        CreateOpenGLContext(OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION);
    }

    GameWindow::~GameWindow()
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(m_OpenGLContext);
    }

    unsigned int GameWindow::GetWidth()
    {
        return m_Width;
    }

    unsigned int GameWindow::GetHeight()
    {
        return m_Height;
    }

    void GameWindow::SetTitle(const std::string& aTitle)
    {
        SetWindowTextA(m_WindowHandle, aTitle.c_str());
    }

    bool GameWindow::IsMouseCursorVisible()
    {
        return m_IsMouseCursorVisible;
    }

    void GameWindow::ShowMouseCursor()
    {
        ShowCursor(true);
        m_IsMouseCursorVisible = true;
    }

    void GameWindow::HideMouseCursor()
    {
        ShowCursor(false);
        m_IsMouseCursorVisible = false;
    }

    void GameWindow::SetMouseCursorPosition(int aX, int aY)
    {
        POINT point;
        point.x = aX;
        point.y = aY;
        ClientToScreen(m_WindowHandle, &point);
        SetCursorPos(point.x, point.y);
    }

    void GameWindow::ResizeTo(unsigned int aWidth, unsigned int aHeight)
    {
        VideoModeInfo videoModeInfo;
        videoModeInfo.width = aWidth;
        videoModeInfo.height = aHeight;
        videoModeInfo.fullscreen = m_IsFullScreen;
        ApplyVideoModeChanges(&videoModeInfo);
    }

    void GameWindow::SetIsFullscreen(bool aIsFullscreen)
    {
        VideoModeInfo videoModeInfo;
        videoModeInfo.width = m_Width;
        videoModeInfo.height = m_Height;
        videoModeInfo.fullscreen = aIsFullscreen;
        ApplyVideoModeChanges(&videoModeInfo);
    }

    bool GameWindow::IsFullscreen()
    {
        return m_IsFullScreen;
    }

    void GameWindow::EnableVerticalSync(bool aIsEnabled)
    {
        //Set the vertical sync flag
        m_IsVerticalSyncEnabled = aIsEnabled;

        //Ensure the v-sync function actually exists
        if (wglSwapIntervalEXT != nullptr)
        {
            if (m_IsVerticalSyncEnabled)
            {
                wglSwapIntervalEXT(1);
            }
            else
            {
                wglSwapIntervalEXT(0);
            }
        }
        else
        {
            m_IsVerticalSyncEnabled = false;
        }
    }

    bool GameWindow::IsVerticalSyncEnabled()
    {
        return m_IsVerticalSyncEnabled;
    }

    int GameWindow::PresentNativeDialogBox(const char* aTitle, const char* aMessage, NativeDialogType aType)
    {
        //Determine the type of windows dialog box to display
        unsigned int type = 0;
        switch (aType)
        {
        case GameDev2D::NativeDialogOk:
            type = MB_OK;
            break;
        case GameDev2D::NativeDialogOkCancel:
            type = MB_OKCANCEL;
            break;
        case GameDev2D::NativeDialogYesNo:
            type = MB_YESNO;
            break;
        default:
            break;
        }

        //Get the result of the dialog box
        int result = MessageBoxA(m_WindowHandle, aTitle, aMessage, type);

        //Return the result
        return result;
    }

    void GameWindow::DetectGamePads()
    {
        //Cache the last controller detection time stamp
        m_LastGamePadDetection = GameLoop::GetTime();

        //Local variables used in the for loop below
        XINPUT_STATE state;
        bool detected = false;

        //Cycle through available controller ports
        for (unsigned int port = 0; port < GamePad::PortCount; port++)
        {
            //Zero out the memory of the state struct
            ZeroMemory(&state, sizeof(XINPUT_STATE));

            //Can we get the state of a controller on this port
            if (XInputGetState(port, &state) == ERROR_SUCCESS)
            {
                GamePad* gamePad = GameDev2D::Services::GetInputManager()->GetGamePad(static_cast<GamePad::Port>(port));
                if (gamePad->IsConnected() == true)
                {
                    continue;
                }
                else
                {
                    gamePad->Connect(state);
                    detected = true;
                }
            }
        }
    }

    void GameWindow::CheckForGamePadInput()
    {
        //If three seconds have passed, check to see if new controllers have been plugged in
        if (GameLoop::GetTime() > m_LastGamePadDetection + GAMEPAD_DETECTION_FREQUENCY)
        {
            DetectGamePads();
        }

        //Local variable used in the for loop below
        XINPUT_STATE state;

        //Cycle through all the GamePad ports
        for (unsigned int port = 0; port < GamePad::PortCount; port++)
        {
            //Get the GamePad for the specific port
            GamePad* gamePad = GameDev2D::Services::GetInputManager()->GetGamePad(static_cast<GamePad::Port>(port));

            //Is the GamePad connected?
            if (gamePad->IsConnected() == true)
            {
                //Zero the memory of the state struct
                ZeroMemory(&state, sizeof(XINPUT_STATE));

                //Can we get the controller state for this port, if we get an error, it 
                //means the GamePad was disconnected
                if (XInputGetState(port, &state) == ERROR_SUCCESS)
                {
                    gamePad->RefreshState(state);
                }
                else
                {
                    gamePad->Disconnect();
                }
            }
        }
    }

    void GameWindow::ApplyVideoModeChanges(VideoModeInfo* aVideoModeInfo)
    {
        //Will we need to dispatch a fullscreen event
        bool dispatchFullscreenEvent = m_IsFullScreen != aVideoModeInfo->fullscreen;

        //Is fullscreen enabled?
        if (aVideoModeInfo->fullscreen == true)
        {
            //Set the fullscreen flag to true
            m_IsFullScreen = true;

            //Update the window style for fullscreen display
            SetWindowLongPtr(m_WindowHandle, GWL_STYLE, WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);

            //Set the fullscreen display settings
            DEVMODE displaySettings;
            EnumDisplaySettings(NULL, 0, &displaySettings);
            displaySettings.dmPelsWidth = aVideoModeInfo->width;
            displaySettings.dmPelsHeight = aVideoModeInfo->height;
            displaySettings.dmBitsPerPel = 32;
            displaySettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

            //Enable fullscreen mode, if the assert below is hit, the resolution passed in is NOT supported in fullscreen mode
            bool success = ChangeDisplaySettings(&displaySettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL;
            assert(success == true);

            //Move the window into position
            MoveWindow(m_WindowHandle, 0, 0, aVideoModeInfo->width, aVideoModeInfo->height, TRUE);
        }
        else
        {
            //Exit fullscreen mode
            bool success = ChangeDisplaySettings(0, 0) == DISP_CHANGE_SUCCESSFUL;
            assert(success == true);

            //Set the fullscreen flag to false
            m_IsFullScreen = false;

            //Set the window rect
            RECT rect;
            rect.left = 0;
            rect.top = 0;
            rect.right = aVideoModeInfo->width;
            rect.bottom = aVideoModeInfo->height;

            //Calculate the position of the window, centered on screen
            int monitorResolutionX = GetSystemMetrics(SM_CXSCREEN);
            int monitorResolutionY = GetSystemMetrics(SM_CYSCREEN);
            int x = (monitorResolutionX - aVideoModeInfo->width) / 2;
            int y = (monitorResolutionY - aVideoModeInfo->height) / 2;

            //Restore the window's 'windowed' style, then set the window's rect and position on screen
            SetWindowLongPtr(m_WindowHandle, GWL_STYLE, WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE);
            AdjustWindowRect(&rect, WS_CAPTION | WS_POPUPWINDOW, FALSE);
            MoveWindow(m_WindowHandle, x, y, rect.right - rect.left, rect.bottom - rect.top, TRUE);
        }

        //Dispatch a fullscreen event
        if (dispatchFullscreenEvent == true)
        {
            Services::GetApplication()->DispatchEvent(FullscreenEvent(m_IsFullScreen));
        }
    }

    void GameWindow::CreateOpenGLContext(unsigned int aMajorVersion, unsigned int aMinorVersion)
    {
        //Set attributes with the OpenGL version we want to target
        int attributes[] = 
        {
		    WGL_CONTEXT_MAJOR_VERSION_ARB, (int)aMajorVersion, // Set the MAJOR version
		    WGL_CONTEXT_MINOR_VERSION_ARB, (int)aMinorVersion, // Set the MINOR version
		    0
	    };

        //If this assert is hit, it means your computer's video card doesn't support the version of OpenGL we want to target
        assert(wglCreateContextAttribsARB != nullptr);

        //Create a new opengl context with the attributes above
        HGLRC openGLContext = wglCreateContextAttribsARB(m_DeviceContext, NULL, attributes);
		
        //Remove the temporary context from being active and delete the temporary OpenGL 2.1 context
        wglMakeCurrent(NULL, NULL); 
		wglDeleteContext(m_OpenGLContext);

        //Set the OpenGL context and make it current
        m_OpenGLContext = openGLContext;
		wglMakeCurrent(m_DeviceContext, m_OpenGLContext);
    }

    void GameWindow::SwapDrawBuffer()
    {
        SwapBuffers(m_DeviceContext);
    }

    void GameWindow::HandleResize(unsigned int aWidth, unsigned int aHeight)
    {
        //Safety check that the width and height have changed
        if (aWidth != m_Width || aHeight != m_Height)
        {
            //Set the width and height
            m_Width = aWidth;
            m_Height = aHeight;

            //Dispatch a Resize event
            Services::GetApplication()->DispatchEvent(ResizeEvent(Vector2((float)m_Width, (float)m_Height)));
        }
    }

    LRESULT GameWindow::EventHandler(HWND aHandle, UINT aMessage, WPARAM aWParam, LPARAM aLParam)
    {
	    bool useDefault = false;

        //Safety check that the application isn't null
        if (Services::GetApplication() == nullptr)
        {
		    return DefWindowProc(aHandle, aMessage, aWParam, aLParam);
        }

        //Handle the windows message
	    switch (aMessage)
	    {
	        case WM_SIZE:
            {
                if (Services::GetApplication() != nullptr && Services::GetApplication()->GetWindow() != nullptr)
                {
                    Services::GetApplication()->GetWindow()->HandleResize(LOWORD(aLParam), HIWORD(aLParam));
		        }
            }
	        break;

            case WM_SETFOCUS:
            {
                Services::GetApplication()->Resume();
            }
            break;

            case WM_KILLFOCUS:
            {
                Services::GetApplication()->Suspend();
            }
            break;

            case WM_MOVING:
            {
                if (Services::GetApplication()->IsSuspended() == false)
                {
                    Services::GetApplication()->Suspend();
                }
            }
            break;

            case WM_EXITSIZEMOVE:
            {
                Services::GetApplication()->Resume();
            }
            break;

            case WM_SYSCOMMAND:
            {
                //Is the screensaver trying to start? Or is a monitor trying to enter Powersave?
                //Prevent From Happening by returning zero
                switch(aWParam)
                {
                    case SC_SCREENSAVE:             
                    case SC_MONITORPOWER:
                        return 0;
                    default:
                        return DefWindowProc(aHandle, aMessage, aWParam, aLParam);
                }
            }
            break;

	        case WM_MOUSEMOVE:
            {
                int height = Services::GetApplication()->GetWindow()->GetHeight();
                int positionX = GET_X_LPARAM(aLParam);
				int positionY = height - GET_Y_LPARAM(aLParam);
                Services::GetInputManager()->HandleMouseMoved((float)positionX, (float)positionY, 0, 0);
            }
	        break;

	        case WM_MOUSEWHEEL:
            {
                Services::GetInputManager()->HandleScrollWheel(GET_WHEEL_DELTA_WPARAM(aWParam));
            }
	        break;

	        case WM_LBUTTONDOWN:
            {
                int height = Services::GetApplication()->GetWindow()->GetHeight();
                int positionX = GET_X_LPARAM(aLParam);
				int positionY = height - GET_Y_LPARAM(aLParam);
                Services::GetInputManager()->HandleLeftMouseDown((float)positionX, (float)positionY);
            }
	        break;

	        case WM_LBUTTONUP:
            {
                int height = Services::GetApplication()->GetWindow()->GetHeight();
                int positionX = GET_X_LPARAM(aLParam);
				int positionY = height - GET_Y_LPARAM(aLParam);
                Services::GetInputManager()->HandleLeftMouseUp((float)positionX, (float)positionY);
            }
	        break;

	        case WM_RBUTTONDOWN:
            {
                int height = Services::GetApplication()->GetWindow()->GetHeight();
                int positionX = GET_X_LPARAM(aLParam);
				int positionY = height - GET_Y_LPARAM(aLParam);
                Services::GetInputManager()->HandleRightMouseDown((float)positionX, (float)positionY);
            }
	        break;

	        case WM_RBUTTONUP:
            {
                int height = Services::GetApplication()->GetWindow()->GetHeight();
                int positionX = GET_X_LPARAM(aLParam);
				int positionY = height - GET_Y_LPARAM(aLParam);
                Services::GetInputManager()->HandleRightMouseUp((float)positionX, (float)positionY);
            }
	        break;

	        case WM_MBUTTONDOWN:
            {
                int height = Services::GetApplication()->GetWindow()->GetHeight();
                int positionX = GET_X_LPARAM(aLParam);
				int positionY = height - GET_Y_LPARAM(aLParam);
                Services::GetInputManager()->HandleCenterMouseDown((float)positionX, (float)positionY);
            }
	        break;

	        case WM_MBUTTONUP:
            {
                int height = Services::GetApplication()->GetWindow()->GetHeight();
                int positionX = GET_X_LPARAM(aLParam);
				int positionY = height - GET_Y_LPARAM(aLParam);
                Services::GetInputManager()->HandleCenterMouseUp((float)positionX, (float)positionY);
            }
	        break;

	        case WM_KEYDOWN:
	        case WM_SYSKEYDOWN:
            {
                Services::GetInputManager()->HandleKeyDown(MapKey(aWParam, aLParam));
            }
	        break;

	        case WM_KEYUP:
	        case WM_SYSKEYUP:
            {
                unsigned int key = MapKey(aWParam, aLParam);
                if (static_cast<Keyboard::Key>(key) == Keyboard::Escape && Services::GetApplication()->GetWindow()->IsFullscreen() == true)
                {
                    Services::GetApplication()->GetWindow()->SetIsFullscreen(false);
                }
                else
                {
                    Services::GetInputManager()->HandleKeyUp(key);
                }
            }
	        break;

	        case WM_CLOSE:
            {
                Services::GetApplication()->Shutdown();
                useDefault = true;
            }
	        break;

	        case WM_DESTROY:
            {
		        PostQuitMessage(0);
            }
		    break;

	        default:
		        useDefault = true;
		        break;
	    }
	
	    if (useDefault)
        {
		    return DefWindowProc(aHandle, aMessage, aWParam, aLParam);
        }
	    else
        {
		    return 0;
        }
    }

    unsigned int GameWindow::MapKey(WPARAM aWParam, LPARAM aLParam)
    {
        switch (aWParam) 
        {
				case VK_CONTROL:
					if(aLParam & (1<<24))
                    {
						aWParam = VK_RCONTROL;
                    }
					else
                    {
						aWParam = VK_LCONTROL;
                    }
					break;
				case VK_SHIFT:
					//Use MapVirtualKey to determine whether it's LSHIFT or RSHIFT by scancode.
					UINT scancode = (aLParam & 0x00ff0000) >> 16;
					aWParam = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
					break;
	    }
	    return (unsigned int)aWParam;
    }
}