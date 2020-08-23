#ifndef __GameDev2D__GamePadButtonDownEvent__
#define __GameDev2D__GamePadButtonDownEvent__

#include "Event.h"
#include "../Input/GamePad.h"


namespace GameDev2D
{
    class GamePadButtonDownEvent : public Event
    {
    public:
        GamePadButtonDownEvent(GamePad* gamePad, GamePad::Button button);

        //Returns the GamePad that was just connected
        GamePad* GetGamePad();

        //Get the GamePad Button that was just released
        GamePad::Button GetButton();

    protected:
        void LogEvent();

    private:
        GamePad* m_GamePad;
        GamePad::Button m_Button;
    };
}

#endif
