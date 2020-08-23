#include "KeyRepeatEvent.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    KeyRepeatEvent::KeyRepeatEvent(Keyboard::Key aKey, double aDuration) : Event(KEY_REPEAT_EVENT),
        m_Key(aKey),
        m_Duration(aDuration)
    {


    }

    Keyboard::Key KeyRepeatEvent::GetKey()
    {
        return m_Key;
    }

    double KeyRepeatEvent::GetDuration()
    {
        return m_Duration;
    }

    void KeyRepeatEvent::LogEvent()
    {
        Log::Message(Log::Verbosity_Input_Keyboard, "[KeyRepeatEvent] Key repeat: 0x%02x - %s - Duration: %f seconds", m_Key, Keyboard::KeyToString(m_Key).c_str(), m_Duration);
    }
}