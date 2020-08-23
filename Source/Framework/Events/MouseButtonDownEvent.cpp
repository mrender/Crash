#include "MouseButtonDownEvent.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    MouseButtonDownEvent::MouseButtonDownEvent(Mouse::Button aButton, Vector2 aPosition) : Event(MOUSE_BUTTON_DOWN_EVENT),
        m_Button(aButton),
        m_Position(aPosition)
    {

    }
    
    Mouse::Button MouseButtonDownEvent::GetButton()
    {
        return m_Button;
    }
    
    Vector2 MouseButtonDownEvent::GetPosition()
    {
        return m_Position;
    }
    
    void MouseButtonDownEvent::LogEvent()
    {
        Log::Message(Log::Verbosity_Input_Mouse, "[MouseButtonDownEvent] %s mouse button down at position (%f, %f)", Mouse::ButtonToString(m_Button), m_Position.x, m_Position.y);
    }
}