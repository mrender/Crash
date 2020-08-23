#include "Application.h"
#include "../Services/Services.h"
#include "../Debug/Log.h"
#include "../Events/UpdateEvent.h"


namespace GameDev2D
{
    Application::Application(const char* aWindowTitle, int aFrameRate, unsigned int aWidth, unsigned int aHeight, bool aFullscreen) : 
        m_GameWindow(nullptr),
        m_GameLoop(nullptr),
        m_IsSuspended(false),
        m_IsRunning(true)
    {
        //Create the Game window
        m_GameWindow = new GameWindow();

        //Create the GameLoop
        m_GameLoop = new GameLoop(this);
        m_GameLoop->SetTargetFrameRate(aFrameRate);

        //Add an event listener for the GameLoop so it is notified of a RESUME_EVENT
        AddEventListener(m_GameLoop, RESUME_EVENT);

        //Set the application and load the default services
        Services::Init(this);

        //Initialize the GameWindow
        m_GameWindow->SetTitle(aWindowTitle);
        m_GameWindow->ResizeTo(aWidth, aHeight);
        m_GameWindow->SetIsFullscreen(aFullscreen);
        m_GameWindow->EnableVerticalSync(false);
    }

    Application::~Application()
    {
        //Call the shutdown callback
        m_ShutdownCallback();

        if (m_GameWindow != nullptr)
        {
            delete m_GameWindow;
            m_GameWindow = nullptr;
        }

        if (m_GameLoop != nullptr)
        {
            delete m_GameLoop;
            m_GameLoop = nullptr;
        }

        //Clean up the game services
        Services::Cleanup();
    }
    
    void Application::Init(std::function<void()> aInitCallback, std::function<void()> aShutdownCallback, std::function<void(double)> aUpdateCallback, std::function<void()> aDrawCallback)
    {
        //Initialize the log file used by the Debug Log class
        Log::Init();

        //Log the platform details (CPU, RAM, GPU and hardware info)
        LogPlatformDetails();

        //Create the Game object, pass in the Camera to the Game
        m_ShutdownCallback = aShutdownCallback;
        m_UpdateCallback = aUpdateCallback;
        m_DrawCallback = aDrawCallback;

        //Call the init callback
        aInitCallback();
    }

    void Application::LogPlatformDetails()
    {
        //Get the platform name, model and operating system version
        string version;
        PlatformVersion(version);

        //Log platform information
        Log::Message(Log::Verbosity_Application, "[Application] Platform: Windows %", version.c_str());
        Log::Message(Log::Verbosity_Application, "[Application] CPU cores: %u", CpuCount());
        Log::Message(Log::Verbosity_Application, "[Application] Memory installed: %.2f-GB", ((float)MemoryInstalled() / 1024.0f / 1024.0f / 1024.0f));
        Log::Message(Log::Verbosity_Application, "[Application] Disk space total: %.2f-GB", ((float)DiskSpaceTotal() / 1024.0f / 1024.0f / 1024.0f));
        Log::Message(Log::Verbosity_Application, "[Application] Disk space used: %.2f-GB", ((float)DiskSpaceUsed() / 1024.0f / 1024.0f / 1024.0f));
        Log::Message(Log::Verbosity_Application, "[Application] Disk space free: %.2f-GB", ((float)DiskSpaceFree() / 1024.0f / 1024.0f / 1024.0f));
    }

    GameWindow* Application::GetWindow() const
    {
        return m_GameWindow;
    }

    GameLoop* Application::GetGameLoop() const
    {
        return m_GameLoop;
    }

    bool Application::Loop()
    {
        if (m_IsSuspended == false)
        {
            m_GameLoop->Step();
        }

        return m_IsRunning;
    }

    void Application::Update(double aDelta)
    {
        //If the game isn't running anymore, return false
        if (m_IsRunning == true)
        {
            //Call the update callback
            m_UpdateCallback(aDelta);

            //Dispatch an Update event
            DispatchEvent(UpdateEvent(aDelta));
        }
    }

    void Application::Draw()
    {
        //If the application isn't suspended, clear the OpenGL view
        if(m_IsSuspended == false)
        {
            Services::GetGraphics()->Clear();
        }

        //Call the draw callback
        m_DrawCallback();
        
        //Dispatch a Draw event
        DispatchEvent(Event(DRAW_EVENT));

        //If the application isn't suspended, flush the opengl buffer
        if(m_IsSuspended == false)
        {
            //Lastly flush the draw buffer
            if(m_GameWindow != nullptr)
            {
                m_GameWindow->SwapDrawBuffer();
            }
        }
    }

    void Application::Resume()
    {
        //Set the isSuspended flag to false
        m_IsSuspended = false;

        //Dispatch a Resume Event
        DispatchEvent(Event(RESUME_EVENT));
    }

    void Application::Suspend()
    {
        //Set the isSuspended flag to true
        m_IsSuspended = true;

        //Dispatch a Suspend Event
        DispatchEvent(Event(SUSPEND_EVENT));
    }

    void Application::Shutdown()
    {
        if (m_IsRunning == true)
        {
            //Set the isRunning flag to false
            m_IsRunning = false;

            //Dispatch a Shutdown Event
            DispatchEvent(Event(SHUTDOWN_EVENT));
        }
    }

    bool Application::IsSuspended()
    {
        return m_IsSuspended;
    }
    
    string Application::GetWorkingDirectory()
    {
        return GetApplicationDirectory();
    }
    
    string Application::GetApplicationDirectory()
    {
        char currentDir[MAX_PATH];
        currentDir, GetModuleFileNameA(NULL, currentDir, MAX_PATH);
        PathRemoveFileSpecA(currentDir);
        return string(currentDir);
    }
    
    string Application::GetPathForResourceInDirectory(const char* aFileName, const char* aFileType, const char* aDirectory)
    {
        //string path = string(GetApplicationDirectory());
        //path += "\\Assets\\";
        string path = "Assets\\";
        path += string(aDirectory);
        path += "\\";
        path += string(aFileName);
        path += ".";
        path += string(aFileType);
        return path;
    }
    
    bool Application::DoesFileExistAtPath(const string& aPath)
    {
        DWORD fileAttributes = GetFileAttributesA(aPath.c_str());
        return (fileAttributes != INVALID_FILE_ATTRIBUTES && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY));
    }

    void Application::PlatformVersion(string& aVersion)
    {
        //Setup and initialize version struct
        OSVERSIONINFOA osVersion;
        ZeroMemory(&osVersion, sizeof(OSVERSIONINFOA));
        osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

#pragma warning(push)
#pragma warning(disable : 4996)

        //Get the OS version
        GetVersionExA(&osVersion);

#pragma warning(pop)

        //String stream used to convert the numeric version
        stringstream version;

        //Convert the version to a string
        if(osVersion.dwMajorVersion == 6 && osVersion.dwMinorVersion == 3)
        {
            version << "8.1";
        }
        else if(osVersion.dwMajorVersion == 6 && osVersion.dwMinorVersion == 2)
        {
            version << "8";
        }
        else if(osVersion.dwMajorVersion == 6 && osVersion.dwMinorVersion == 1)
        {
            version << "7";
        }
        else if(osVersion.dwMajorVersion == 6 && osVersion.dwMinorVersion == 0)
        {
            version << "Vista";
        }
        else if(osVersion.dwMajorVersion == 5 && (osVersion.dwMinorVersion == 1 || osVersion.dwMinorVersion == 2))
        {
            version << "XP";
        }
        else if(osVersion.dwMajorVersion == 5 && osVersion.dwMinorVersion == 0)
        {
            version << "2000";
        }

        //Set the return version
        version << " (Build " << osVersion.dwBuildNumber << ": " << osVersion.szCSDVersion << ")";
        aVersion = string(version.str());
    }
    
    unsigned long long Application::MemoryInstalled()
    {
        unsigned long long physicalMemory = 0;
        GetPhysicallyInstalledSystemMemory(&physicalMemory);
        physicalMemory *= 1024;
        return physicalMemory;
    }
    
    unsigned long long Application::DiskSpaceUsed()
    {
        return DiskSpaceTotal() - DiskSpaceFree();
    }
    
    unsigned long long Application::DiskSpaceFree()
    {
        unsigned __int64 lpFreeBytesAvailableToCaller, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes;
        GetDiskFreeSpaceExA ("C:", (PULARGE_INTEGER)&lpFreeBytesAvailableToCaller, (PULARGE_INTEGER)&lpTotalNumberOfBytes, (PULARGE_INTEGER)&lpTotalNumberOfFreeBytes);
        return lpTotalNumberOfFreeBytes;
    }
    
    unsigned long long Application::DiskSpaceTotal()
    {
        unsigned __int64 lpFreeBytesAvailableToCaller, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes;
        GetDiskFreeSpaceExA ("C:", (PULARGE_INTEGER)&lpFreeBytesAvailableToCaller, (PULARGE_INTEGER)&lpTotalNumberOfBytes, (PULARGE_INTEGER)&lpTotalNumberOfFreeBytes);
        return lpTotalNumberOfBytes;
    }
    
    unsigned int Application::CpuCount()
    {
        SYSTEM_INFO sysinfo;
        GetSystemInfo( &sysinfo );
        unsigned int cpuCount = sysinfo.dwNumberOfProcessors;
        return cpuCount;
    }
}