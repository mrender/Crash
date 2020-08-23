#include "KeyUpEvent.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    KeyUpEvent::KeyUpEvent(Keyboard::Key aKey, double aDuration) : Event(KEY_UP_EVENT),
        m_Key(aKey),
        m_Duration(aDuration)
    {


    }

    Keyboard::Key KeyUpEvent::GetKey()
    {
        return m_Key;
    }

    double KeyUpEvent::GetDuration()
    {
        return m_Duration;
    }
    
    void KeyUpEvent::LogEvent()
    {
        Log::Message(Log::Verbosity_Input_Keyboard, "[KeyUpEvent] Key up: 0x%02x - %s - Duration: %f seconds", m_Key, Keyboard::KeyToString(m_Key).c_str(), m_Duration);
    }
}