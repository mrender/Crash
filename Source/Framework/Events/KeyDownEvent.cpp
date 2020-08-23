#include "KeyDownEvent.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    KeyDownEvent::KeyDownEvent(Keyboard::Key aKey) : Event(KEY_DOWN_EVENT),
        m_Key(aKey)
    {

    }

    Keyboard::Key KeyDownEvent::GetKey()
    {
        return m_Key;
    }

    void KeyDownEvent::LogEvent()
    {
        Log::Message(Log::Verbosity_Input_Keyboard, "[KeyDownEvent] Key down: 0x%02x - %s", m_Key, Keyboard::KeyToString(m_Key).c_str());
    }
}