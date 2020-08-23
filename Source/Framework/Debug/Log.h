#pragma once

#include <stdarg.h>


namespace GameDev2D
{
    class Log
    {
    public:
        //Verbosity log levels
        enum Verbosity
        {
            Verbosity_None = 0,
            Verbosity_Debug = 1,
            Verbosity_Application = 2,
            Verbosity_Input_Keyboard = 4,
            Verbosity_Input_Mouse = 8,
            Verbosity_Input_GamePad = 16,
            Verbosity_Input = Verbosity_Input_Keyboard | Verbosity_Input_Mouse | Verbosity_Input_GamePad,
            Verbosity_Graphics = 32,
			Verbosity_Resources = 64,
            Verbosity_Shaders = 128,
            Verbosity_Audio = 256,
            Verbosity_UI = 512,
            Verbosity_Profiling = 1024,
			Verbosity_Memory = 2048,
            Verbosity_All = Verbosity_Debug | Verbosity_Application | Verbosity_Input | Verbosity_Graphics | Verbosity_Resources | Verbosity_Shaders | Verbosity_Audio | Verbosity_UI | Verbosity_Profiling | Verbosity_Memory
        };

        //Calling Init() will ensure that the log file is properly created on the hard drive
        static void Init();

        //Used to Log a message with a variable amount of arguments, the
        //verbosity level for these logs is debug (VerbosityLevel_Debug).
        //If the LOG_TO_FILE to file preproc is enabled then this method
        //will log to a file in addition to the output window.
        static void Message(std::string message, ...);

        //Same as the Message() function above, except you can specify the
        //verbosity of the log messages.
        static void Message(Verbosity verbosity, std::string message, ...);

        //Same as the Message() function above, except you can specify the
        //verbosity of the log messages.
        //static void Message(void* address, const char* label, Verbosity verbosity, const char* message, ...);

        //Used to specically log errors, it has no verbosity level, and can
        //throw an exception.
        static void Error(bool throwException, Verbosity verbosity, std::string message, ...);

    private:
        //Conveniance method used to log messages
        static void Output(bool error, Verbosity verbosity, const char* aOutput, va_list aArgumentsList);
    
        //Member variables
        static bool s_IsInitialized;
    };
}
