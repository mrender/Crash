#ifndef __GameDev2D__GameLoop__
#define __GameDev2D__GameLoop__

#include "../Events/EventHandler.h"
#include <functional>


namespace GameDev2D
{
    class GameLoopCallback
    {
    public:
        virtual ~GameLoopCallback() {}
        virtual void Update(double delta) = 0;
        virtual void Draw() = 0;
    };


    class GameLoop : public EventHandler
    {
    public:
        GameLoop(GameLoopCallback* callback);

        //Used to handle a resume event, to ensure we don't get a big delta time spike
        void HandleEvent(Event* event);

        //Measures the time between this call and the previous call, and 
        //notify's the GameLoopCallback
        void Step();

        //Returns the time between the last two frames
        double GetDelta() const;

        //Returns the total elapsed time, since the start of the application
        double GetElapsedTime() const;

        //Gets the average FPS over the last second
        unsigned int GetFPS() const;

        //Sets and Returns the target frame rate
        void SetTargetFrameRate(unsigned int target);
        unsigned int GetTargetFrameRate();

        //Sets and returns wether to limit the framerate
        void EnableFrameRateLimit(bool isLimitted);
        bool IsFrameRateLimit();

        //Get the current 'time' it's an aribitrary time
        static double GetTime();

    private:
        //High frequency timer methods
        static double GetTimerFrequency();

        //Conveniance method to determine if we can step
        bool CanStep();

        //Timing variables
        GameLoopCallback* m_Callback;
        double m_DeltaTime; // The current timestep
        double m_ElapsedTime;
        double m_CurrentTime;
        double m_PreviousTime;
        double m_PreviousFpsUpdate;
        double m_FpsUpdateFrequency;  // The frequency by which to update the FPS
        double m_TargetFrameTime;
        unsigned int m_Fps;
        unsigned int m_Frames;  // Frames since last FPS update
        bool m_LimitFramerate;
    };
}

#endif