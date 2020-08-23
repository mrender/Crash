#include "GamePadLeftTriggerEvent.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    GamePadLeftTriggerEvent::GamePadLeftTriggerEvent(GamePad* aGamePad, float aValue) : Event(GAMEPAD_LEFT_TRIGGER_EVENT),
        m_GamePad(aGamePad),
        m_Value(aValue)
    {

    }

    GamePad* GamePadLeftTriggerEvent::GetGamePad()
    {
        return m_GamePad;
    }

    float GamePadLeftTriggerEvent::GetValue()
    {
        return m_Value;
    }

    void GamePadLeftTriggerEvent::LogEvent()
    {
        Log::Message(Log::Verbosity_Input_GamePad, "[GamePadLeftTriggerEvent] Controller on port: %u, left trigger: %f", m_GamePad->GetPort() + 1, m_Value);
    }
}