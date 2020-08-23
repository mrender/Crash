#include "GamePad.h"
#include "../Debug/Log.h"
#include "../Events/GamePadConnectedEvent.h"
#include "../Events/GamePadDisconnectedEvent.h"
#include "../Events/GamePadButtonDownEvent.h"
#include "../Events/GamePadButtonUpEvent.h"
#include "../Events/GamePadLeftTriggerEvent.h"
#include "../Events/GamePadRightTriggerEvent.h"
#include "../Events/GamePadLeftThumbStickEvent.h"
#include "../Events/GamePadRightThumbStickEvent.h"
#include "../Math/Math.h"
#include <Windows.h>


namespace GameDev2D
{
    GamePad::GamePad(Port aPort) : EventDispatcher(),
        m_Vibration(Vibration()),
        m_LeftThumbStick(Vector2(0.0f, 0.0f)),
        m_RightThumbStick(Vector2(0.0f, 0.0f)),
        m_LeftTrigger(0.0f),
        m_RightTrigger(0.0f),
        m_Deadzone(0.2f),
        m_Port(aPort),
        m_IsConnected(false)
    {
        //Zero the memory of the state struct
        ZeroMemory(&m_State, sizeof(XINPUT_STATE));
    }

    GamePad::~GamePad()
    {

    }

    void GamePad::Update(double aDelta)
    {
        //Is the controller connected?
        if (IsConnected() == true)
        {
            //If a button is down, then update the duration in which it's been held down for
            for (std::map<Button, State>::iterator i = m_Buttons.begin(); i != m_Buttons.end(); ++i)
            {
                if (i->second.isPressed == true)
                {
                    i->second.duration += aDelta;
                }
            }

            //Is there a vibration to apply?
            if (m_Vibration.duration > 0.0)
            {
                m_Vibration.duration -= aDelta;
                if (m_Vibration.duration <= 0.0)
                {
                    m_Vibration.duration = 0.0;
                    m_Vibration.leftMotor = 0.0f;
                    m_Vibration.rightMotor = 0.0f;
                }

                // Create a Vibraton State
                XINPUT_VIBRATION vibration;
                vibration.wLeftMotorSpeed = (unsigned short)(m_Vibration.leftMotor * USHRT_MAX);
                vibration.wRightMotorSpeed = (unsigned short)(m_Vibration.rightMotor * USHRT_MAX);
                XInputSetState(m_Port, &vibration);
            }
        }
    }

    bool GamePad::IsConnected()
    {
        return m_IsConnected;
    }

    GamePad::Port GamePad::GetPort()
    {
        return m_Port;
    }

    bool GamePad::IsButtonPressed(Button aButton)
    {
        return m_Buttons[aButton].isPressed;
    }

    Vector2 GamePad::GetLeftThumbStick()
    {
        return m_LeftThumbStick;
    }

    Vector2 GamePad::GetRightThumbStick()
    {
        return m_RightThumbStick;
    }

    float GamePad::GetLeftTrigger()
    {
        return m_LeftTrigger;
    }

    float GamePad::GetRightTrigger()
    {
        return m_RightTrigger;
    }

    void GamePad::Vibrate(float aLeftMotor, float aRightMotor, double aDuration)
    {
        if (IsConnected() == true)
        {
            m_Vibration.leftMotor = Math::Clamp(aLeftMotor, 0.0f, 1.0f);;
            m_Vibration.rightMotor = Math::Clamp(aRightMotor, 0.0f, 1.0f);;
            m_Vibration.duration = aDuration;
        }
    }

    void GamePad::Connect(XINPUT_STATE aState)
    {
        //Set the is connected flag to true
        m_IsConnected = true;

        //Set the initial state
        m_State = aState;

        //Dispatch the event
        DispatchEvent(GamePadConnectedEvent(this));
    }

    void GamePad::Disconnect()
    {
        //Set the is connected flag to false
        m_IsConnected = false;

        //Zero the memory of the state struct
        ZeroMemory(&m_State, sizeof(XINPUT_STATE));

        //Clear the input variables
        m_LeftTrigger = 0.0f;
        m_RightTrigger = 0.0f;
        m_LeftThumbStick = Vector2(0.0f, 0.0f);
        m_RightThumbStick = Vector2(0.0f, 0.0f);

        //Dispatch the event
        DispatchEvent(GamePadDisconnectedEvent(this));
    }

    void GamePad::RefreshState(XINPUT_STATE aState)
    {
        //Handle the left thumb stick
        if (aState.Gamepad.sThumbLX != m_State.Gamepad.sThumbLX || aState.Gamepad.sThumbLY != m_State.Gamepad.sThumbLY)
        {
            m_LeftThumbStick.x = ConvertAnalogToFloat(aState.Gamepad.sThumbLX);
            m_LeftThumbStick.y = ConvertAnalogToFloat(aState.Gamepad.sThumbLY);

            //Is the magnitude less than the deadzone? if it is, zero the analog input
            if (m_LeftThumbStick.Length() < m_Deadzone)
            {
                m_LeftThumbStick = Vector2(0.0f, 0.0f);
            }
            else
            {
                DispatchEvent(GamePadLeftThumbStickEvent(this, m_LeftThumbStick));
            }
        }

        //Handle the right thumb stick
        if (aState.Gamepad.sThumbRX != m_State.Gamepad.sThumbRX || aState.Gamepad.sThumbRY != m_State.Gamepad.sThumbRY)
        {
            m_RightThumbStick.x = ConvertAnalogToFloat(aState.Gamepad.sThumbRX);
            m_RightThumbStick.y = ConvertAnalogToFloat(aState.Gamepad.sThumbRY);

            if (m_RightThumbStick.Length() < m_Deadzone)
            {
                m_RightThumbStick = Vector2(0.0f, 0.0f);
            }
            else
            {
                DispatchEvent(GamePadRightThumbStickEvent(this, m_RightThumbStick));
            }
        }

        //Handle the left trigger
        if (aState.Gamepad.bLeftTrigger != m_State.Gamepad.bLeftTrigger)
        {
            m_LeftTrigger = static_cast<float>(aState.Gamepad.bLeftTrigger) / 255.0f;
            DispatchEvent(GamePadLeftTriggerEvent(this, m_LeftTrigger));
        }

        //Handle the right trigger
        if (aState.Gamepad.bRightTrigger != m_State.Gamepad.bRightTrigger)
        {
            m_RightTrigger = static_cast<float>(aState.Gamepad.bRightTrigger) / 255.0f;
            DispatchEvent(GamePadRightTriggerEvent(this, m_RightTrigger));
        }

        //Handle button input
        if (aState.Gamepad.wButtons != m_State.Gamepad.wButtons)
        {
            //Check the dpad buttons
            UpdateButtonState(DpadUp, aState.Gamepad.wButtons, m_State.Gamepad.wButtons);
            UpdateButtonState(DpadDown, aState.Gamepad.wButtons, m_State.Gamepad.wButtons);
            UpdateButtonState(DpadLeft, aState.Gamepad.wButtons, m_State.Gamepad.wButtons);
            UpdateButtonState(DpadRight, aState.Gamepad.wButtons, m_State.Gamepad.wButtons);

            //Check the clickable thumbstick buttons
            UpdateButtonState(LeftThumbStick, aState.Gamepad.wButtons, m_State.Gamepad.wButtons);
            UpdateButtonState(RightThumbStick, aState.Gamepad.wButtons, m_State.Gamepad.wButtons);
                
            //Check the shoulder buttons
            UpdateButtonState(LeftShoulder, aState.Gamepad.wButtons, m_State.Gamepad.wButtons);
            UpdateButtonState(RightShoulder, aState.Gamepad.wButtons, m_State.Gamepad.wButtons);

            //Check the face buttons
            UpdateButtonState(A, aState.Gamepad.wButtons, m_State.Gamepad.wButtons);
            UpdateButtonState(B, aState.Gamepad.wButtons, m_State.Gamepad.wButtons);
            UpdateButtonState(X, aState.Gamepad.wButtons, m_State.Gamepad.wButtons);
            UpdateButtonState(Y, aState.Gamepad.wButtons, m_State.Gamepad.wButtons);

            //Start and select buttons
            UpdateButtonState(Start, aState.Gamepad.wButtons, m_State.Gamepad.wButtons);
            UpdateButtonState(Select, aState.Gamepad.wButtons, m_State.Gamepad.wButtons);
        }

        //Update the Xinput state
        m_State = aState;
    }

    void GamePad::UpdateButtonState(Button aButton, unsigned short aCurrent, unsigned short aPrevious)
    {
        if ((aPrevious ^ aCurrent) & aButton)
        {
            //Is the button's state up or down?
            bool isDown = !!(aCurrent & aButton);

            //Dispatch a GamePad Button event
            if (isDown == true)
            {
                m_Buttons[aButton].isPressed = isDown;
                DispatchEvent(GamePadButtonDownEvent(this, aButton));
            }
            else
            {
                m_Buttons[aButton].isPressed = isDown;
                DispatchEvent(GamePadButtonUpEvent(this, aButton, m_Buttons[aButton].duration));
                m_Buttons[aButton].duration = 0.0;
            }
        }
    }

    std::string GamePad::ButtonToString(Button aButton)
    {
        switch (aButton)
        {
        case DpadUp:
            return "Dpad up";

        case DpadDown:
            return "Dpad down";

        case DpadLeft:
            return "Dpad left";

        case DpadRight:
            return "Dpad right";

        case LeftThumbStick:
            return "Left thumb stick";

        case RightThumbStick:
            return "Right thumb stick";

        case LeftShoulder:
            return "Left shoulder";

        case RightShoulder:
            return "Right shoulder";

        case A:
            return "A";

        case B:
            return "B";

        case X:
            return "X";

        case Y:
            return "Y";

        case Start:
            return "Start";

        case Select:
            return "Select";

        default:
            break;
        }

        return "Unknown";
    }

    float GamePad::ConvertAnalogToFloat(short aValue)
    {
        float value = ((float)(aValue - SHRT_MIN) / (float)(SHRT_MAX - SHRT_MIN) * 2.0f - 1.0f);
        return value; // *(isPlatformInverted == true ? -1.0f : 1.0f) * (isUserInverted == true ? -1.0f : 1.0f);
    }
}