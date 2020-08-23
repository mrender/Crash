#ifndef __GameDev2D__GamePadDisonnectedEvent__
#define __GameDev2D__GamePadDisonnectedEvent__

#include "Event.h"
#include "../Input/GamePad.h"


namespace GameDev2D
{
    class GamePadDisconnectedEvent : public Event
    {
    public:
        GamePadDisconnectedEvent(GamePad* gamePad);

        //Returns the GamePad that was just connected
        GamePad* GetGamePad();

    protected:
        void LogEvent();

    private:
        GamePad* m_GamePad;
    };
}

#endif
