#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <string>
#include <Windows.h>


namespace GameDev2D
{
    //Forward declaration
    class GamePad;

    //The video mode struct keeps track of the width and height, and if they need to be changed
    struct VideoModeInfo
    {
        unsigned int width;
        unsigned int height;
        bool fullscreen;
    };

    //Supported dialog types
    enum NativeDialogType
    {
        NativeDialogOk = 0,
        NativeDialogOkCancel,
        NativeDialogYesNo
    };

    //The GameWindow handle window creation, event handling and OpenGL context creation
    class GameWindow
    {
    public:
        GameWindow();
        ~GameWindow();

        //Returns the Window's current width and height
        unsigned int GetWidth();
        unsigned int GetHeight();

        //Set's the title of the window
        void SetTitle(const std::string& title);

        //Returns wether the mouse cursor is visible
        bool IsMouseCursorVisible();

        //Methods to show and hide the mouse cursor
        void ShowMouseCursor();
        void HideMouseCursor();

        //Sets the current position of the mouse cursor
        void SetMouseCursorPosition(int x, int y);

        //Resizes the Window    
        void ResizeTo(unsigned int width, unsigned int height);

        //Fullscreen methods
        void SetIsFullscreen(bool isFullscreen);
        bool IsFullscreen();

        //Vertical sync methods
        void EnableVerticalSync(bool isEnabled);
        bool IsVerticalSyncEnabled();

        //Checks the GamePads for new input
        void CheckForGamePadInput();

        //Presents a platform specific native dialog box
        int PresentNativeDialogBox(const char* title, const char* message, NativeDialogType type);

    private:
        //Allow the Application class to access non-public members
        friend class Application;

        //Controller methods, windows specific
        void DetectGamePads();

        //Apply video mode changes
        void ApplyVideoModeChanges(VideoModeInfo* aVideoModeInfo);

        //Create an OpenGL context for the requested version
        void CreateOpenGLContext(unsigned int majorVersion, unsigned int minorVersion);

        //Swaps the draw buffer
        void SwapDrawBuffer();

        //Handle the window resize, notify the proper services
        void HandleResize(unsigned int width, unsigned int height);

        //Windows event handler static method
        static LRESULT CALLBACK EventHandler(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

        //Keyboard input static methods
        static unsigned int MapKey(WPARAM wParam, LPARAM lParam);

        //Member variables
        HWND m_WindowHandle;
        HDC m_DeviceContext;
        HGLRC m_OpenGLContext;
        unsigned int m_Width;
        unsigned int m_Height;
        bool m_IsMouseCursorVisible;
        bool m_IsFullScreen;
        bool m_IsVerticalSyncEnabled;

        //Used to keep track of when the last controller detection was, on windows we 
        //have to continually poll to see if new controllers have been plugged in
        double m_LastGamePadDetection;
    };
}

#endif