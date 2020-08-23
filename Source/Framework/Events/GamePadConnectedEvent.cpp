#include "GamePadConnectedEvent.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    GamePadConnectedEvent::GamePadConnectedEvent(GamePad* aGamePad) : Event(GAMEPAD_CONNECTED_EVENT),
        m_GamePad(aGamePad)
    {

    }

    GamePad* GamePadConnectedEvent::GetGamePad()
    {
        return m_GamePad;
    }

    void GamePadConnectedEvent::LogEvent()
    {
        Log::Message(Log::Verbosity_Input_GamePad, "[GamePadConnectedEvent] Controller on port: %u connected", m_GamePad->GetPort() + 1);
    }
}