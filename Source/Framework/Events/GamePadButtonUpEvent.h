#ifndef __GameDev2D__GamePadButtonUpEvent__
#define __GameDev2D__GamePadButtonUpEvent__

#include "Event.h"
#include "../Input/GamePad.h"


namespace GameDev2D
{
    class GamePadButtonUpEvent : public Event
    {
    public:
        GamePadButtonUpEvent(GamePad* gamePad, GamePad::Button button, double duration);

        //Returns the GamePad that was just connected
        GamePad* GetGamePad();

        //Get the GamePad Button that was just released
        GamePad::Button GetButton();

        //Returns the duration the button was held down before being released
        double GetDuration();

    protected:
        void LogEvent();

    private:
        GamePad* m_GamePad;
        GamePad::Button m_Button;
        double m_Duration;
    };
}

#endif
