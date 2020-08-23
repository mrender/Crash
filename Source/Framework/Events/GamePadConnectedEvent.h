#ifndef __GameDev2D__GamePadConnectedEvent__
#define __GameDev2D__GamePadConnectedEvent__

#include "Event.h"
#include "../Input/GamePad.h"


namespace GameDev2D
{
    class GamePadConnectedEvent : public Event
    {
    public:
        GamePadConnectedEvent(GamePad* gamePad);

        //Returns the GamePad that was just connected
        GamePad* GetGamePad();

    protected:
        void LogEvent();

    private:
        GamePad* m_GamePad;
    };
}

#endif
