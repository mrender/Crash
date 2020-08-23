#include "MouseButtonUpEvent.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    MouseButtonUpEvent::MouseButtonUpEvent(Mouse::Button aButton, double aDuration, Vector2 aPosition) : Event(MOUSE_BUTTON_UP_EVENT),
        m_Button(aButton),
        m_Position(aPosition),
        m_Duration(aDuration)
    {

    }

    Mouse::Button MouseButtonUpEvent::GetButton()
    {
        return m_Button;
    }

    Vector2 MouseButtonUpEvent::GetPosition()
    {
        return m_Position;
    }

    double MouseButtonUpEvent::GetDuration()
    {
        return m_Duration;
    }

    void MouseButtonUpEvent::LogEvent()
    {
        Log::Message(Log::Verbosity_Input_Mouse, "[MouseButtonUpEvent] %s mouse button up after being held for %f, at position (%f, %f)", Mouse::ButtonToString(m_Button), m_Duration, m_Position.x, m_Position.y);
    }
}