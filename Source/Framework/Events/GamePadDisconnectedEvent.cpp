#include "GamePadDisconnectedEvent.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    GamePadDisconnectedEvent::GamePadDisconnectedEvent(GamePad* aGamePad) : Event(GAMEPAD_DISCONNECTED_EVENT),
        m_GamePad(aGamePad)
    {

    }

    GamePad* GamePadDisconnectedEvent::GetGamePad()
    {
        return m_GamePad;
    }

    void GamePadDisconnectedEvent::LogEvent()
    {
        Log::Message(Log::Verbosity_Input_GamePad, "[GamePadDisconnectedEvent] Controller on port: %u disonnected", m_GamePad->GetPort() + 1);
    }
}