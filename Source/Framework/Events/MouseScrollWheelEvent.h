#ifndef __GameDev2D__MouseScrollWheelEvent__
#define __GameDev2D__MouseScrollWheelEvent__

#include "Event.h"


namespace GameDev2D
{
    //The MouseScrollWheelEvent class is a subclass of the event class and is sent by the
    //InputManager whenever the user scrolls the wheel one of the mouse.
    class MouseScrollWheelEvent : public Event
    {
    public:
        MouseScrollWheelEvent(float delta);
        
        //Returns how much the scroll wheel was scrolled
        float GetDetla();
        
    protected:
        void LogEvent();

    private:
        float m_Delta;
    };
}

#endif
