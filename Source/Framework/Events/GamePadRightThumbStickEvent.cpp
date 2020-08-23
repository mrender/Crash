#include "GamePadRightThumbStickEvent.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    GamePadRightThumbStickEvent::GamePadRightThumbStickEvent(GamePad* aGamePad, Vector2 aValue) : Event(GAMEPAD_RIGHT_THUMBSTICK_EVENT),
        m_GamePad(aGamePad),
        m_Value(aValue)
    {

    }

    GamePad* GamePadRightThumbStickEvent::GetGamePad()
    {
        return m_GamePad;
    }

    Vector2 GamePadRightThumbStickEvent::GetValue()
    {
        return m_Value;
    }

    void GamePadRightThumbStickEvent::LogEvent()
    {
        Log::Message(Log::Verbosity_Input_GamePad, "[GamePadRightThumbStickEvent] Controller on port: %u, right thumb stick: (%f, %f)", m_GamePad->GetPort() + 1, m_Value.x, m_Value.y);
    }
}