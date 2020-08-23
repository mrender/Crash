#ifndef __GameDev2D__MouseButtonDownEvent__
#define __GameDev2D__MouseButtonDownEvent__

#include "Event.h"
#include "../Input/Mouse.h"


namespace GameDev2D
{
    class MouseButtonDownEvent : public Event
    {
    public:
        MouseButtonDownEvent(Mouse::Button button, Vector2 position);

        //Returns which mouse button was clicked
        Mouse::Button GetButton();
        
        //Returns the current position of the mouse
        Vector2 GetPosition();
      
    protected:
        void LogEvent();
      
    private:
        Mouse::Button m_Button;
        Vector2 m_Position;
    };
}

#endif
