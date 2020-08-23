#include "MouseMovementEvent.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    MouseMovementEvent::MouseMovementEvent(Vector2 aPosition, Vector2 aPreviousPosition, Vector2 aDelta) : Event(MOUSE_MOVEMENT_EVENT),
        m_Position(aPosition),
        m_PreviousPosition(aPreviousPosition),
        m_DeltaPosition(aDelta)
    {

    }
 
    Vector2 MouseMovementEvent::GetPosition()
    {
        return m_Position;
    }

    Vector2 MouseMovementEvent::GetPreviousPosition()
    {
        return m_PreviousPosition;
    }
    
    Vector2 MouseMovementEvent::GetDeltaMovement()
    {
        return m_DeltaPosition;
    }
    
    void MouseMovementEvent::LogEvent()
    {
        Log::Message(Log::Verbosity_Input_Mouse, "[MouseMovementEvent] Mouse moved to position: (%f, %f) - Delta movement: (%f, %f)", m_Position.x, m_Position.y, m_DeltaPosition.x, m_DeltaPosition.y);
    }
}