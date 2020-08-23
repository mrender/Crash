#ifndef __GameDev2D__KeyDownEvent__
#define __GameDev2D__KeyDownEvent__

#include "Event.h"
#include "../Input/Keyboard.h"


namespace GameDev2D
{
    class KeyDownEvent : public Event
    {
    public:
        KeyDownEvent(Keyboard::Key key);

        //Returns the key was just released
        Keyboard::Key GetKey();

    protected:
        void LogEvent();

    private:
        Keyboard::Key m_Key;
    };
}

#endif
