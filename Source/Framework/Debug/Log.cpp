#include "Log.h"
#include <GameDev2D.h>
#include <stdio.h>
#include <string>
#include <Windows.h>


namespace GameDev2D
{
    bool Log::s_IsInitialized = false;

    void Log::Init()
    {
#if LOG_TO_FILE
        if (s_IsInitialized == false)
        {
            //Get the Path for the Working Directory and append the Log filename to it
            string path = string(Services::GetApplication()->GetWorkingDirectory());
            path += LOG_FILE;

            //Open the log file, wiping any existing data in the file
            FILE* logFile;
            logFile = fopen(path.c_str(), "w");

            //Make sure the file opened, if it did close the file
            if (logFile != NULL)
            {
                s_IsInitialized = true;
                fprintf(logFile, "GameDev2D initiated\n\n");
                fclose(logFile);
            }
        }
#endif
    }

    void Log::Message(std::string aMessage, ...)
    //void Log::Message(const char* aMessage, ...)
    {
#if DEBUG || _DEBUG
        va_list arguments;
        va_start(arguments, aMessage);
        Log::Output(false, Verbosity_Debug, aMessage.c_str(), arguments);
        va_end(arguments);
#endif
    }


    void Log::Message(Verbosity aVerbosity, std::string aMessage, ...)
    {
#if DEBUG || _DEBUG
        va_list arguments;
        va_start(arguments, aMessage);
        Log::Output(false, aVerbosity, aMessage.c_str(), arguments);
        va_end(arguments);
#endif
    }

    void Log::Error(bool aThrowException, Verbosity aVerbosity, std::string aMessage, ...)
    {
#if DEBUG || _DEBUG
        va_list arguments;
        va_start(arguments, aMessage);
        Log::Output(true, aVerbosity, aMessage.c_str(), arguments);
        va_end(arguments);


        
#if THROW_EXCEPTION_ON_ERROR
        //Should an exception be thrown?
        if (aThrowException == true)
        {
            char outputBuffer[4096];
            vsnprintf(outputBuffer, 4096, aMessage.c_str(), arguments);

            throw std::runtime_error(outputBuffer);
        }

        //End the variable argument list
        va_end(arguments);
#endif
#endif
    }

    void Log::Output(bool aError, Verbosity aVerbosity, const char* aOutput, va_list aArgumentsList)
    {
#if DEBUG || _DEBUG
        //Is this an error?
        if (aError == true)
        {
            ////Output to the window's 'Output' window
            OutputDebugStringA("[ERROR]");
            char outputBuffer[1024];
            vsnprintf(outputBuffer, 1024, aOutput, aArgumentsList);
            OutputDebugStringA(outputBuffer);
            OutputDebugStringA("\n");
        }
        else
        {
            if ((aVerbosity & (LOG_VERBOSITY_MASK)) != 0)
            {
                //Output to the window's 'Output' window
                char outputBuffer[1024];
                vsnprintf(outputBuffer, 1024, aOutput, aArgumentsList);
                OutputDebugStringA(outputBuffer);
                OutputDebugStringA("\n");
            }


#if LOG_TO_FILE
            if (s_IsInitialized == true)
            {
                //Get the path for the log file
                string path = string(Services::GetApplication()->GetWorkingDirectory());
                path += LOG_FILE;

                //Open the file at the path, if the assert is hit it means the log file couldn't be opened
                FILE* logFile;
                logFile = fopen(path.c_str(), "a+");
                assert(logFile != nullptr);

                //Write the log to the file
                if (logFile != NULL)
                {
                    vfprintf(logFile, aOutput, aArgumentsList);
                    fprintf(logFile, "\n");
                    fclose(logFile);
                }
            }
#endif
        }
#endif
    }
}