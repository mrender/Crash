#include "GamePadLeftThumbStickEvent.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    GamePadLeftThumbStickEvent::GamePadLeftThumbStickEvent(GamePad* aGamePad, Vector2 aValue) : Event(GAMEPAD_LEFT_THUMBSTICK_EVENT),
        m_GamePad(aGamePad),
        m_Value(aValue)
    {

    }

    GamePad* GamePadLeftThumbStickEvent::GetGamePad()
    {
        return m_GamePad;
    }

    Vector2 GamePadLeftThumbStickEvent::GetValue()
    {
        return m_Value;
    }

    void GamePadLeftThumbStickEvent::LogEvent()
    {
        Log::Message(Log::Verbosity_Input_GamePad, "[GamePadLeftThumbStickEvent] Controller on port: %u, left thumb stick: (%f, %f)", m_GamePad->GetPort() + 1, m_Value.x, m_Value.y);
    }
}