#ifndef __GameDev2D__GamePadRightTriggerEvent__
#define __GameDev2D__GamePadRightTriggerEvent__

#include "Event.h"
#include "../Input/GamePad.h"


namespace GameDev2D
{
    class GamePadRightTriggerEvent : public Event
    {
    public:
        GamePadRightTriggerEvent(GamePad* gamePad, float value);

        //Returns the GamePad that was just connected
        GamePad* GetGamePad();

        //Returns the right trigger's value
        float GetValue();

    protected:
        void LogEvent();

    private:
        GamePad* m_GamePad;
        float m_Value;
    };
}

#endif
