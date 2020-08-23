#include "GameLoop.h"
#include "../Debug/Log.h"
#include <GameDev2D.h>
#include <Windows.h>
#include <assert.h>


namespace GameDev2D
{
    GameLoop::GameLoop(GameLoopCallback* aCallback) :
        m_Callback(aCallback),
        m_DeltaTime(0.0),
        m_ElapsedTime(0.0),
        m_CurrentTime(0.0),
        m_PreviousTime(0.0),
        m_PreviousFpsUpdate(0.0),
        m_FpsUpdateFrequency(1.0),
        m_TargetFrameTime(0.0),
        m_Fps(0),
        m_Frames(0),
#ifdef LIMIT_FPS
        m_LimitFramerate(LIMIT_FPS)
#else
        m_LimitFramerate(true)
#endif
    {
        //The callback pointer can't be null
        assert(m_Callback != nullptr);

        //Initialize the variables that need a starting time
        m_PreviousFpsUpdate = m_CurrentTime = GetTime();
    }

    void GameLoop::HandleEvent(Event* aEvent)
    {
        //Ensure the event isn't null
        if (aEvent != nullptr)
        {
            //Handle a resume event, to ensure we don't get a big delta time spike
            if (aEvent->GetEventCode() == RESUME_EVENT)
            {
                m_CurrentTime = GetTime();
            }
        }
    }

    void GameLoop::Step()
    {
        //Can we actually step?
        if (CanStep() == true)
        {
            //Set the 'previous' time
            m_PreviousTime = m_CurrentTime;

            //Get the current time from system
            m_CurrentTime = GetTime();

            //Calculate the delta time
            m_DeltaTime = m_CurrentTime - m_PreviousTime;

            //This will trash the very first frame, since we've loaded a 
            //lot of stuff, it will avoid a large initial delta time
            bool doCallback = m_ElapsedTime > 0.0;

            //Increment the elapsed time
            m_ElapsedTime += m_DeltaTime;

            //Should we update and draw our frame?
            if (doCallback == true)
            {
                //Call the update callback
                m_Callback->Update(m_DeltaTime);

                //Call the draw callback
                m_Callback->Draw();

                //Increment the frame count
                m_Frames++;

                //Should we update the FPS value?
                double timeSinceLast = m_CurrentTime - m_PreviousFpsUpdate;
                if (timeSinceLast > m_FpsUpdateFrequency)
                {
                    m_Fps = static_cast<unsigned int>(std::roundf((float)m_Frames / (float)timeSinceLast));
                    m_PreviousFpsUpdate = m_CurrentTime;
                    m_Frames = 0;
                }
            }
        }
    }

    double GameLoop::GetDelta() const
    {
        return m_DeltaTime;
    }

    double GameLoop::GetElapsedTime() const
    {
        return m_ElapsedTime;
    }

    unsigned int GameLoop::GetFPS() const
    {
        return m_Fps;
    }

    void GameLoop::SetTargetFrameRate(unsigned int aTarget)
    {
        m_TargetFrameTime = 1000.0 / static_cast<double>(aTarget);
    }

    unsigned int GameLoop::GetTargetFrameRate()
    {
        return static_cast<unsigned int>(m_TargetFrameTime * 1000.0);
    }

    void GameLoop::EnableFrameRateLimit(bool aIsLimitted)
    {
        m_LimitFramerate = aIsLimitted;
    }

    bool GameLoop::IsFrameRateLimit()
    {
        return m_LimitFramerate;
    }

    double GameLoop::GetTime()
    {
        static const double timerPeriod = GetTimerFrequency();

        LARGE_INTEGER largeInt;
        QueryPerformanceCounter(&largeInt);
        return (double)largeInt.QuadPart * timerPeriod;
    }

    double GameLoop::GetTimerFrequency()
    {
        LARGE_INTEGER largeInt;
        if (QueryPerformanceFrequency(&largeInt) != 0 && largeInt.QuadPart != 0)
        {
            return 1.0 / (double)largeInt.QuadPart;
        }
        return 0.0;
    }

    bool GameLoop::CanStep()
    {
        // limit framerate;
        if (m_LimitFramerate == true)
        {
            double frameTime = (GetTime() - m_CurrentTime) * 1000.0; //In milleseconds
            if (frameTime < m_TargetFrameTime)
            {
                return false;
            }
        }
        return true;
    }
}