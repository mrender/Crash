#ifndef APPLICATION_H
#define APPLICATION_H

#include "GameLoop.h"
#include "GameWindow.h"
#include "../Events/EventDispatcher.h"
#include <functional>
#include <string>


namespace GameDev2D
{
    //This class provides an Windows specific functionality and the implementation for the game loop, 
    //input, file io, window creation, sizing, dialog boxes and resume, suspend and shutdown event handling.
    //This class dispatches the following events:
    // SHUTDOWN_EVENT
    // SUSPEND_EVENT
    // RESUME_EVENT
    // RESIZE_EVENT
    // FULLSCREEN_EVENT
    class Application : public EventDispatcher, public GameLoopCallback
    {
    public:
        //The constructor takes in the Window the application is in, the window title and the target frame rate.
        Application(const char* windowTitle, int frameRate, unsigned int width, unsigned int height, bool fullscreen);
        ~Application();
        
        //
        void Init(std::function<void()> initCallback, std::function<void()> shutdownCallback, std::function<void(double)> updateCallback, std::function<void()> drawCallback);

        //
        GameWindow* GetWindow() const;

        //
        GameLoop* GetGameLoop() const;

        //The Loop() methodis responsible for calling the Update() and Draw() methods
        //if the application isn't in a suspended state
        bool Loop();

        //Methods used to Update and Draw the application, called from the GameLoop
        void Update(double delta);
        void Draw();

        //The Resume() and Suspend() methods are called when the application gains or loses focus,
        //they will each, respectively, dispatch Resume and Suspend events. Can be overridden.
        void Resume();
        void Suspend();

        //The Shutdown() method is called when the Platform receives a notification that it will be
        //terminated. The Shutdown() method will dispatch a shutdown event.
        void Shutdown();

        //
        bool IsSuspended();

        //Returns a path to the directory where files can be written to
        std::string GetWorkingDirectory();

        //Returns a path to the directory that the application is in
        std::string GetApplicationDirectory();
        
        //Returns a path for a resource in a specific directory
        std::string GetPathForResourceInDirectory(const char* fileName, const char* fileType, const char* directory);
        
        //Returns wether the file exists at the path
        bool DoesFileExistAtPath(const std::string& path);
        
        //Returns platform version
        void PlatformVersion(std::string& version);
     
        //Returns how much memory (RAM) is installed on the machine
        unsigned long long MemoryInstalled();
        
        //Hard drive specific methods
        unsigned long long DiskSpaceUsed();
        unsigned long long DiskSpaceFree();
        unsigned long long DiskSpaceTotal();

        //Returns the number of available CPUs on the machine
        unsigned int CpuCount();
    
    protected:        
        //Initializes the log files, if there are existing log files, they are wiped.
        void InitLogFiles();

        //Logs platform specific details about OS, CPU, RAM, HDD, GPU
        void LogPlatformDetails();

    private:
        //Member variables
        std::function<void()> m_ShutdownCallback;
        std::function<void(double)> m_UpdateCallback;
        std::function<void()> m_DrawCallback;
        GameWindow* m_GameWindow;
        GameLoop* m_GameLoop;
        bool m_IsSuspended;
        bool m_IsRunning;
    };
}

#endif