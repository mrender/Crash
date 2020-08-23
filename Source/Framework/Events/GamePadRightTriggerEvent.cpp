#include "GamePadRightTriggerEvent.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    GamePadRightTriggerEvent::GamePadRightTriggerEvent(GamePad* aGamePad, float aValue) : Event(GAMEPAD_RIGHT_TRIGGER_EVENT),
        m_GamePad(aGamePad),
        m_Value(aValue)
    {

    }

    GamePad* GamePadRightTriggerEvent::GetGamePad()
    {
        return m_GamePad;
    }

    float GamePadRightTriggerEvent::GetValue()
    {
        return m_Value;
    }

    void GamePadRightTriggerEvent::LogEvent()
    {
        Log::Message(Log::Verbosity_Input_GamePad, "[GamePadRightTriggerEvent] Controller on port: %u, right trigger: %f", m_GamePad->GetPort() + 1, m_Value);
    }
}