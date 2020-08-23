#ifndef __GameDev2D__MouseButtonUpEvent__
#define __GameDev2D__MouseButtonUpEvent__

#include "Event.h"
#include "../Input/Mouse.h"


namespace GameDev2D
{
    class MouseButtonUpEvent : public Event
    {
    public:
        MouseButtonUpEvent(Mouse::Button button, double duration, Vector2 position);

        //Returns which mouse button was clicked
        Mouse::Button GetButton();

        //Returns the current position of the mouse
        Vector2 GetPosition();

        //Returns the duration the mouse button was held down for
        double GetDuration();

    protected:
        void LogEvent();

    private:
        Mouse::Button m_Button;
        Vector2 m_Position;
        double m_Duration;
    };
}

#endif
