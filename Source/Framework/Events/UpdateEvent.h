#ifndef __GameDev2D__UpdateEvent__
#define __GameDev2D__UpdateEvent__

#include "Event.h"


namespace GameDev2D
{
    //The UpdateEvent class is a subclass of the Event class and is sent by the
    //PlatformLayer whenever the frame receives its update
    class UpdateEvent : public Event
    {
    public:
        UpdateEvent(double delta);

        //Returns the delta time
        double GetDelta();

    protected:
        void LogEvent();

    private:
        double m_Delta;
    };
}

#endif
