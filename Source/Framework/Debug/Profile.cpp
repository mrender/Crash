#include "Profile.h"
#include "Log.h"
#include <Windows.h>


namespace GameDev2D
{
    Profile::Profile(const std::string& aLabel) :
        m_Label(aLabel),
        m_StartTime(0)
    {

    }

    void Profile::Begin()
    {
#if DEBUG || _DEBUG
        //Log the begining of the profile
        Log::Message("Profile", Log::Verbosity_Profiling, "%s Begin", m_Label.c_str());

        //Get the performance counter frequency
        LARGE_INTEGER largeInt;
        QueryPerformanceFrequency(&largeInt);
        double performanceCounterFrequency = double(largeInt.QuadPart) / 1000.0;

        //Cache the profiling start time
        QueryPerformanceCounter(&largeInt);
        m_StartTime = (unsigned long long)(largeInt.QuadPart / performanceCounterFrequency);
#endif
    }

    double Profile::End()
    {
        //Local variable
        double duration = 0.0;

#if DEBUG || _DEBUG
        //Get the performance counter frequency
        LARGE_INTEGER largeInt;
        QueryPerformanceFrequency(&largeInt);
        double performanceCounterFrequency = double(largeInt.QuadPart) / 1000.0;

        //Cache the profiling start time
        QueryPerformanceCounter(&largeInt);
        unsigned long long profileEnd = (unsigned long long)(largeInt.QuadPart / performanceCounterFrequency);

        //determine the entire duration
        duration = (((double)(profileEnd - m_StartTime)) / 1000.0);

        //Log the profile results
        Log::Message("Profile", Log::Verbosity_Profiling, "%s Ended: %f", m_Label.c_str(), duration);
#endif

        //Return the duration
        return duration;
    }
}