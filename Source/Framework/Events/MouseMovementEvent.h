#ifndef __GameDev2D__MouseMovementEvent__
#define __GameDev2D__MouseMovementEvent__

#include "Event.h"
#include "../Math/Vector2.h"


namespace GameDev2D
{
    class MouseMovementEvent : public Event
    {
    public:
        MouseMovementEvent(Vector2 position, Vector2 previousPosition, Vector2 delta);
        
        //Returns the current position of the mouse,
        //caution it could be outside of the active window
        Vector2 GetPosition();
        

        //Returns the previous position of the mouse,
        //caution it could be outside of the active window
        Vector2 GetPreviousPosition();

        //Returns the delta movement of the mouse
        //(ie how much the mouse moved in the x and y axis)
        Vector2 GetDeltaMovement();
        
    protected:        
        void LogEvent();
      
    private:
        Vector2 m_Position;
        Vector2 m_PreviousPosition;
        Vector2 m_DeltaPosition;
    };
}

#endif
