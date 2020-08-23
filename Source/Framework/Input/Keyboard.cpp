#include "Keyboard.h"
#include "../Events/KeyUpEvent.h"
#include "../Events/KeyDownEvent.h"
#include "../Events/KeyRepeatEvent.h"
#include "../Services/Services.h"
#include "../Windows/Application.h"


namespace GameDev2D
{
    Keyboard::Keyboard()
    {
		Services::GetApplication()->AddEventListener(this, SUSPEND_EVENT);
    }
    
    Keyboard::~Keyboard()
    {
		Services::GetApplication()->RemoveEventListener(this, SUSPEND_EVENT);
    }

    void Keyboard::Update(double aDelta)
    {
        //If a key is down, then update the duration in which it's been held down for
        for (std::map<Key, State>::iterator i = m_KeyStates.begin(); i != m_KeyStates.end(); ++i)
        {
            if (i->second.keyState == KeyDown || i->second.keyState == KeyRepeat)
            {
                i->second.duration += aDelta;
            }
        }
    }

    void Keyboard::HandleKeyDown(Key aKey)
    {
        //If the key is Down?
        if (m_KeyStates[aKey].keyState == KeyDown || m_KeyStates[aKey].keyState == KeyRepeat)
        {
            //We now have a Repeat KeyState
            m_KeyStates[aKey].keyState = KeyRepeat;

            //Dispatch the event for anyone else listening
            DispatchEvent(KeyRepeatEvent(aKey, m_KeyStates[aKey].duration = 0.0));
        }
        else
        {
            //Other wise, set the key state to Down
            m_KeyStates[aKey].keyState = KeyDown;

            //And dispatch and KeyDown event
            DispatchEvent(KeyDownEvent(aKey));

            //Handle the callback
            if (m_KeyPressedCallback != nullptr)
            {
                m_KeyPressedCallback(static_cast<Keyboard::Key>(aKey));
            }
        }
    }

    void Keyboard::HandleKeyUp(Key aKey)
    {
        //Dispatch a KeyUp event
        DispatchEvent(KeyUpEvent(aKey, m_KeyStates[aKey].duration));

        //And set that the key is no longer pressed
        m_KeyStates[aKey].keyState = KeyUp;

        //Reset the duration
        m_KeyStates[aKey].duration = 0.0;

        //Handle the callback
        if (m_KeyReleasedCallback != nullptr)
        {
            m_KeyReleasedCallback(static_cast<Keyboard::Key>(aKey));
        }
    }

    Keyboard::State Keyboard::GetState(Key aKey)
    {
        return m_KeyStates[aKey];
    }

	void Keyboard::HandleEvent(Event* aEvent)
	{
		if (aEvent->GetEventCode() == SUSPEND_EVENT)
		{
			for (std::map<Key, State>::iterator it = m_KeyStates.begin(); it != m_KeyStates.end(); ++it)
			{
				if (it->second.keyState == KeyDown)
				{
					HandleKeyUp(it->first);
				}
			}
		}
	}

    void Keyboard::RegisterKeyPressedCallback(std::function<void(Key)> aKeyPressedCallback)
    {
        m_KeyPressedCallback = aKeyPressedCallback;
    }

    void Keyboard::RegisterKeyReleasedCallback(std::function<void(Key)> aKeyReleasedCallback)
    {
        m_KeyReleasedCallback = aKeyReleasedCallback;
    }

    std::string Keyboard::KeyStateToString(KeyState aKeyState)
    {
        switch (aKeyState)
        {
        case KeyDown:
            return "Key Down";

        case KeyUp:
            return "Key Up";

        case KeyRepeat:
            return "Key Repeat";

        default:
            break;
        }

        return "Unknown";
    }

    std::string Keyboard::KeyToString(Key aKey)
    {
        switch (aKey)
        {
        case Up:
            return "Up";
        case Down:
            return "Down";
        case Right:
            return "Right";
        case Left:
            return "Left";

        case Spacebar:
            return "Spacebar";
        case Return:
            return "Return";
        case Tab:
            return "Tab";
        case Escape:
            return "Escape";
        case Backspace:
            return "Backspace";
        case Delete:
            return "Delete";

        case A:
            return "A";
        case B:
            return "B";
        case C:
            return "C";
        case D:
            return "D";
        case E:
            return "E";
        case F:
            return "F";
        case G:
            return "G";
        case H:
            return "H";
        case I:
            return "I";
        case J:
            return "J";
        case K:
            return "K";
        case L:
            return "L";
        case M:
            return "M";
        case N:
            return "N";
        case O:
            return "O";
        case P:
            return "P";
        case Q:
            return "Q";
        case R:
            return "R";
        case S:
            return "S";
        case T:
            return "T";
        case U:
            return "U";
        case V:
            return "V";
        case W:
            return "W";
        case X:
            return "X";
        case Y:
            return "Y";
        case Z:
            return "Z";

        case Zero:
            return "0";
        case One:
            return "1";
        case Two:
            return "2";
        case Three:
            return "3";
        case Four:
            return "4";
        case Five:
            return "5";
        case Six:
            return "6";
        case Seven:
            return "7";
        case Eight:
            return "8";
        case Nine:
            return "9";

        case Hyphen:
            return "-";
        case Equal:
            return "=";

        case SemiColon:
            return ";";
        case SingleQuote:
            return "'";
  
        case Comma:
            return ",";
        case Period:
            return ".";

        case ForwardSlash:
            return "/";
        case BackSlash:
            return "\\";

        case RightSquareBracket:
            return "]";
        case LeftSquareBracket:
            return "[";

        case RightShift:
            return "Right Shift";
        case LeftShift:
            return "Left Shift";
        case RightControl:
            return "Right Control";
        case LeftControl:
            return "Left Control";
        case RightAlt:
            return "Right Alt";
        case LeftAlt:
            return "Left Alt";
        case RightWindows:
            return "Right Windows";
        case LeftWindows:
            return "Left Windows"; 

        case NumLock:
            return "Num Lock";
        case CapsLock:
            return "Caps Lock";
        case ScrollLock:
            return "Scroll Lock";

        case Clear:
            return "Clear";
        case Pause:
            return "Pause";
        case Insert:
            return "Insert";
        case Home:
            return "Home";
        case End:
            return "End";
        case Prior:
            return "Page Up";
        case Next:
            return "Page Down";

        case F1:
            return "F1";
        case F2:
            return "F2";
        case F3:
            return "F3";
        case F4:
            return "F4";
        case F5:
            return "F5";
        case F6:
            return "F6";
        case F7:
            return "F7";
        case F8:
            return "F8";
        case F9:
            return "F9";
        case F10:
            return "F10";
        case F11:
            return "F11";
        case F12:
            return "F12";
        case F13:
            return "F13";
        case F14:
            return "F14";
        case F15:
            return "F15";

        default:
            break;
        }

        return "Unknown";
    }
}
