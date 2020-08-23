#include "GamePadButtonUpEvent.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    GamePadButtonUpEvent::GamePadButtonUpEvent(GamePad* aGamePad, GamePad::Button aButton, double aDuration) : Event(GAMEPAD_BUTTON_UP_EVENT),
        m_GamePad(aGamePad),
        m_Button(aButton),
        m_Duration(aDuration)
    {

    }

    GamePad* GamePadButtonUpEvent::GetGamePad()
    {
        return m_GamePad;
    }

    GamePad::Button GamePadButtonUpEvent::GetButton()
    {
        return m_Button;
    }

    double GamePadButtonUpEvent::GetDuration()
    {
        return m_Duration;
    }

    void GamePadButtonUpEvent::LogEvent()
    {
        Log::Message(Log::Verbosity_Input_GamePad, "[GamePadButtonUpEvent] Controller on port: %u, button: %s released after %f seconds", m_GamePad->GetPort() + 1, GamePad::ButtonToString(m_Button).c_str(), m_Duration);
    }
}