#include "Event.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
	Event::Event(int aEventCode) :
        m_EventCode(aEventCode),
        m_Dispatcher(nullptr)
    {        

	}
	
	Event::~Event()
    {
		
	}
    
    unsigned int Event::GetEventCode()
    {
		return m_EventCode;
	}
	
	EventDispatcher* Event::GetDispatcher() const
    {
		return m_Dispatcher;
	}
    
    void Event::SetEventCode(unsigned int aEventCode)
    {
        m_EventCode = aEventCode;
    }

    std::string Event::EventCodeToString(unsigned int aEventCode)
    {
        switch (aEventCode)
        {
        case SHUTDOWN_EVENT:
            return "Shutdown";
        case SUSPEND_EVENT:
            return "Suspend";
        case RESUME_EVENT:
            return "Resume";
        case UPDATE_EVENT:
            return "Update";
        case DRAW_EVENT:
            return "Draw";
        case RESIZE_EVENT:
            return "Resize";
        case FULLSCREEN_EVENT:
            return "Fullscreen";

		case TEXTURE_RESOURCE_LOADED:
			return "Texture resource loaded";
		case TEXTURE_RESOURCE_UNLOADED:
			return "Texture resource unloaded";

		case AUDIO_PLAYBACK_STARTED:
			return "Audio playback started";
		case AUDIO_PLAYBACK_ENDED:
			return "Audio playback ended";
		case AUDIO_LOOP_ENDED:
			return "Audio loop ended";

        case KEY_UP_EVENT:
            return "Key up";
        case KEY_DOWN_EVENT:
            return "Key down";
        case KEY_REPEAT_EVENT:
            return "Key repeat";

        case MOUSE_MOVEMENT_EVENT:
            return "Mouse movement";
        case MOUSE_BUTTON_DOWN_EVENT:
            return "Mouse button down";
        case MOUSE_BUTTON_UP_EVENT:
            return "Mouse button down";
        case MOUSE_WHEEL_EVENT:
            return "Mouse wheel";

        case GAMEPAD_CONNECTED_EVENT:
            return "Gamepad connected";
        case GAMEPAD_DISCONNECTED_EVENT:
            return "Gamepad disconnected";

        case GAMEPAD_BUTTON_UP_EVENT:
            return "Gamepad button up";

        case GAMEPAD_BUTTON_DOWN_EVENT:
            return "Gamepad button down";

        case GAMEPAD_LEFT_TRIGGER_EVENT:
            return "Gamepad left trigger";

        case GAMEPAD_RIGHT_TRIGGER_EVENT:
            return "Gamepad right trigger";

        case GAMEPAD_LEFT_THUMBSTICK_EVENT:
            return "Gamepad left thumb stick";

        case GAMEPAD_RIGHT_THUMBSTICK_EVENT:
            return "Gamepad right thumb stick";

        default:
            break;
        }

        return "Unknown";
    }

    void Event::LogEvent()
    {
        if (m_EventCode != DRAW_EVENT)
        {
            Log::Message(Log::Verbosity_Application, "%s", EventCodeToString(m_EventCode).c_str());
        }
    }
	
	void Event::SetDispatcher(EventDispatcher* aDispatcher)
    {
		m_Dispatcher = aDispatcher;
	}
}