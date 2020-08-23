#ifndef __GameDev2D__GamePadLeftTriggerEvent__
#define __GameDev2D__GamePadLeftTriggerEvent__

#include "Event.h"
#include "../Input/GamePad.h"


namespace GameDev2D
{
    class GamePadLeftTriggerEvent : public Event
    {
    public:
        GamePadLeftTriggerEvent(GamePad* gamePad, float value);

        //Returns the GamePad that was just connected
        GamePad* GetGamePad();

        //Returns the left trigger's value
        float GetValue();

    protected:
        void LogEvent();

    private:
        GamePad* m_GamePad;
        float m_Value;
    };
}

#endif
