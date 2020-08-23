#ifndef __GameDev2D__KeyUpEvent__
#define __GameDev2D__KeyUpEvent__

#include "Event.h"
#include "../Input/Keyboard.h"


namespace GameDev2D
{
    class KeyUpEvent : public Event
    {
    public:
        KeyUpEvent(Keyboard::Key key, double duration);

        //Returns the key was just released
        Keyboard::Key GetKey();
        
        //Returns the duration the key was held down before being released
        double GetDuration();
        
    protected:
        void LogEvent();
      
    private:
        Keyboard::Key m_Key;
        double m_Duration;
    };
}

#endif
