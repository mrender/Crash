#ifndef __GameDev2D__Keyboard__
#define __GameDev2D__Keyboard__

#include "../Events/EventDispatcher.h"
#include <map>
#include <string>
#include <Windows.h>


namespace GameDev2D
{
    // The Keyboard class holds all relevant key data for one specific key code,
    // this class is managed by the InputManager.
    // The Keyboard class dispatches the following events:
    // KEYUP_EVENT
    // KEYDOWN_EVENT
    // KEYREPEAT_EVENT
    class Keyboard : public EventDispatcher
    {
    public:
        Keyboard();
        ~Keyboard();

        enum Key
        {
            Up = VK_UP,
            Down = VK_DOWN,
            Left = VK_LEFT,
            Right = VK_RIGHT,

            Spacebar = VK_SPACE,
            Return = VK_RETURN,
            Tab = VK_TAB,
            Escape = VK_ESCAPE,
            Backspace = VK_BACK,
            Delete = VK_DELETE,

            A = 'A',
            B = 'B',
            C = 'C',
            D = 'D',
            E = 'E',
            F = 'F',
            G = 'G',
            H = 'H',
            I = 'I',
            J = 'J',
            K = 'K',
            L = 'L',
            M = 'M',
            N = 'N',
            O = 'O',
            P = 'P',
            Q = 'Q',
            R = 'R',
            S = 'S',
            T = 'T',
            U = 'U',
            V = 'V',
            W = 'W',
            X = 'X',
            Y = 'Y',
            Z = 'Z',

            Zero = '0',
            One = '1',
            Two = '2',
            Three = '3',
            Four = '4',
            Five = '5',
            Six = '6',
            Seven = '7',
            Eight = '8',
            Nine = '9',

            Hyphen = 0xBD,
            Equal = 0xBB,

            SemiColon = 0xBA,
            SingleQuote = 0xC0,

            Comma = 0xBC,
            Period = 0xBE,

            ForwardSlash = 0xBF,
            BackSlash = 0xDE,

            RightSquareBracket = 0xDD,
            LeftSquareBracket = 0xDB,

            RightShift = VK_RSHIFT,
            LeftShift = VK_LSHIFT,

            RightControl = VK_RCONTROL,
            LeftControl = VK_LCONTROL,

            RightAlt = VK_RMENU,
            LeftAlt = VK_LMENU,

            RightWindows = VK_RWIN,
            LeftWindows = VK_LWIN,

            NumLock = VK_NUMLOCK,
            CapsLock = VK_CAPITAL,
            ScrollLock = VK_SCROLL,

            Clear = VK_CLEAR,
            Pause = VK_PAUSE,
            Insert = VK_INSERT,
            Home = VK_HOME,
            End = VK_END,
            Prior = VK_PRIOR,
            Next = VK_NEXT,

            F1 = VK_F1,
            F2 = VK_F2,
            F3 = VK_F3,
            F4 = VK_F4,
            F5 = VK_F5,
            F6 = VK_F6,
            F7 = VK_F7,
            F8 = VK_F8,
            F9 = VK_F9,
            F10 = VK_F10,
            F11 = VK_F11,
            F12 = VK_F12,
            F13 = VK_F13,
            F14 = VK_F14,
            F15 = VK_F15
        };

        enum KeyState
        {
            KeyDown = 0,
            KeyUp,
            KeyRepeat
        };

        struct State
        {
            State() : 
                duration(0.0),
                keyState(KeyUp)
            {
            }

            State(const State& state) :
                duration(state.duration),
                keyState(state.keyState)
            {
            }

            double duration;
            KeyState keyState;
        };

        //Returns the State for a specific key
        State GetState(Key key);
        
        //Updates the key data, specifically to update the duration a
        //key has been pressed for.
        void Update(double delta);
        
		//
		void HandleEvent(Event* event);

        // Registers a callback function to be called whenever the key is pressed down
        void RegisterKeyPressedCallback(std::function<void(Key)> keyPressedCallback);

        // Registers a callback function to be called whenever the key is released
        void RegisterKeyReleasedCallback(std::function<void(Key)> keyReleasedCallback);


        //Conveniance method to display the names for each key event type
        static std::string KeyStateToString(KeyState keyState);
        static std::string KeyToString(Key key);

    private:
        friend class InputManager;

        //Handle key down events, and dispatches the appropriate event (down or repeat)
        void HandleKeyDown(Key key);

        //Handle key up events, and dispatches the appropriate event (up)
        void HandleKeyUp(Key key);

        //Member variable
        std::map<Key, State> m_KeyStates;
        std::function<void(Key)> m_KeyPressedCallback;
        std::function<void(Key)> m_KeyReleasedCallback;
    };
}

#endif
