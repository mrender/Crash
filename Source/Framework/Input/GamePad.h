#ifndef __GameDev2D__GamePad__
#define __GameDev2D__GamePad__

#include "../Events/EventDispatcher.h"
#include "../Math/Vector2.h"
#include <map>
#include <string>
#include <Xinput.h>


//GamePad constants
const double GAMEPAD_DETECTION_FREQUENCY = 3.0; //In seconds


namespace GameDev2D
{    
    // The GamePad class holds all relevant state data for one GamePad,
    // The GamePad class dispatches the following events:
    // GAMEPAD_CONNECTED_EVENT
    // GAMEPAD_DISCONNECTED_EVENT
    // GAMEPAD_BUTTON_UP_EVENT
    // GAMEPAD_BUTTON_DOWN_EVENT
    // GAMEPAD_LEFT_TRIGGER_EVENT
    // GAMEPAD_RIGHT_TRIGGER_EVENT
    // GAMEPAD_LEFT_THUMBSTICK_EVENT
    // GAMEPAD_RIGHT_THUMBSTICK_EVENT
    class GamePad : public EventDispatcher
    {
    public:
        enum Port
        {
            Port_1 = 0,
            Port_2,
            Port_3,
            Port_4,
            PortCount
        };

        //Create a GamePad for a specific port
        GamePad(Port port);
        ~GamePad();

        //Update's the interal duration variables for ButtonState (to track how long a 
        //button has been pressed for) and to time how long to vibrate the GamePad for
        void Update(double delta);

        //Returns wether the GamePad is connected or not
        bool IsConnected();

        //Returns the port of the GamePad
        Port GetPort();

        enum Button
        {
            //Dpad buttons
            DpadUp = XINPUT_GAMEPAD_DPAD_UP,
            DpadDown = XINPUT_GAMEPAD_DPAD_DOWN,
            DpadLeft = XINPUT_GAMEPAD_DPAD_LEFT,
            DpadRight = XINPUT_GAMEPAD_DPAD_RIGHT,

            //Clickable thumbstick buttons
            LeftThumbStick = XINPUT_GAMEPAD_LEFT_THUMB,
            RightThumbStick = XINPUT_GAMEPAD_RIGHT_THUMB,

            //Shoulder buttons
            LeftShoulder = XINPUT_GAMEPAD_LEFT_SHOULDER,
            RightShoulder = XINPUT_GAMEPAD_RIGHT_SHOULDER,

            //Face buttons
            A = XINPUT_GAMEPAD_A,
            B = XINPUT_GAMEPAD_B,
            X = XINPUT_GAMEPAD_X,
            Y = XINPUT_GAMEPAD_Y,

            //Start and select buttons
            Start = XINPUT_GAMEPAD_START,
            Select = XINPUT_GAMEPAD_BACK
        };

        //Returns the Button is pressed or not
        bool IsButtonPressed(Button button);

        //Returns the value of the left thumb stick, values have a range from 0.0f to 1.0f
        Vector2 GetLeftThumbStick();

        //Returns the value of the right thumb stick, values have a range from 0.0f to 1.0f
        Vector2 GetRightThumbStick();

        //Returns the value of the left trigger, values have a range from 0.0f to 1.0f
        float GetLeftTrigger();

        //Returns the value of the right trigger, values have a range from 0.0f to 1.0f
        float GetRightTrigger();

        //Call, when you want to vibrate the GamePad, the motor values have a range from 0.0f to 1.0f
        void Vibrate(float leftMotor, float rightMotor, double duration);

        //Conveniance method to convert a Button into a string
        static std::string ButtonToString(Button button);

    protected:
        //Called from GameWindow when a GamePad is connected
        void Connect(XINPUT_STATE state);

        //Called from GameWindow when a GamePad is disconnected
        void Disconnect();

        //Called from GameWindow to detect new GamePad input
        void RefreshState(XINPUT_STATE state);

        //Update's the Button state of an individual button
        void UpdateButtonState(Button button, unsigned short current, unsigned short previous);

        //Converts analog data (short to float)
        static float ConvertAnalogToFloat(short value);

        //Allows the GameWindow to access the private members, specifically Connect(), Disconnect() and RefreshState()
        friend class GameWindow;

        //Used to help organize internal Vibration data
        struct Vibration
        {
            Vibration() :
                duration(0.0),
                leftMotor(0.0f),
                rightMotor(0.0f)
            {
            }

            Vibration(const Vibration& vibration) :
                duration(vibration.duration),
                leftMotor(vibration.leftMotor),
                rightMotor(vibration.rightMotor)
            {
            }

            double duration;
            float leftMotor;
            float rightMotor;
        };

        struct State
        {
            State() :
                duration(0.0),
                isPressed(false)
            {
            }

            State(const State& state) :
                duration(state.duration),
                isPressed(state.isPressed)
            {
            }

            double duration;
            bool isPressed;
        };

        //Member variables
        XINPUT_STATE m_State;
        std::map<Button, State> m_Buttons;
        Vibration m_Vibration;
        Vector2 m_LeftThumbStick;
        Vector2 m_RightThumbStick;
        float m_LeftTrigger;
        float m_RightTrigger;
        Port m_Port;
        float m_Deadzone;
        bool m_IsConnected;
    };
}

#endif
