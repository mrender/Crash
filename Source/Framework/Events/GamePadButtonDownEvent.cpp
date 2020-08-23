#include "GamePadButtonDownEvent.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    GamePadButtonDownEvent::GamePadButtonDownEvent(GamePad* aGamePad, GamePad::Button aButton) : Event(GAMEPAD_BUTTON_DOWN_EVENT),
        m_GamePad(aGamePad),
        m_Button(aButton)
    {

    }

    GamePad* GamePadButtonDownEvent::GetGamePad()
    {
        return m_GamePad;
    }

    GamePad::Button GamePadButtonDownEvent::GetButton()
    {
        return m_Button;
    }

    void GamePadButtonDownEvent::LogEvent()
    {
        Log::Message(Log::Verbosity_Input_GamePad, "[GamePadButtonDownEvent] Controller on port: %u, button: %s pressed", m_GamePad->GetPort() + 1, GamePad::ButtonToString(m_Button).c_str());
    }
}