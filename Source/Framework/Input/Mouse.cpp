#include "Mouse.h"
#include "../Events/MouseButtonDownEvent.h"
#include "../Events/MouseButtonUpEvent.h"
#include "../Events/MouseMovementEvent.h"
#include "../Events/MouseScrollWheelEvent.h"


namespace GameDev2D
{
    Mouse::Mouse() : EventDispatcher(),
        m_Position(0.0f, 0.0f),
        m_PreviousPosition(0.0f, 0.0f)
    {

    }
    
    Mouse::~Mouse()
    {
    
    }

    void Mouse::Update(double aDelta)
    {
        //Cycle through all the mouse button, and if any of them are pressed, update the duration
        for (std::map<Button, State>::iterator i = m_ButtonStates.begin(); i != m_ButtonStates.end(); ++i)
        {
            if (i->second.isPressed == true)
            {
                i->second.duration += aDelta;
            }
        }
    }

    void Mouse::RegisterLeftMouseButtonPressedCallback(std::function<void(float, float)> aLeftMouseButtonPressedCallback)
    {
        m_LeftMouseButtonPressedCallback = aLeftMouseButtonPressedCallback;
    }

    void Mouse::RegisterLeftMouseButtonReleasedCallback(std::function<void(float, float)> aLeftMouseButtonReleasedCallback)
    {
        m_LeftMouseButtonReleasedCallback = aLeftMouseButtonReleasedCallback;
    }

    void Mouse::RegisterRightMouseButtonPressedCallback(std::function<void(float, float)> aRightMouseButtonPressedCallback)
    {
        m_RightMouseButtonPressedCallback = aRightMouseButtonPressedCallback;
    }

    void Mouse::RegisterRightMouseButtonReleasedCallback(std::function<void(float, float)> aRightMouseButtonReleasedCallback)
    {
        m_RightMouseButtonReleasedCallback = aRightMouseButtonReleasedCallback;
    }

    void Mouse::RegisterMouseMoveCallback(std::function<void(float, float, float, float)> aMouseMoveCallback)
    {
        m_MouseMoveCallback = aMouseMoveCallback;
    }
    
    Vector2 Mouse::GetPosition()
    {
        return m_Position;
    }
    
    Vector2 Mouse::GetPreviousPosition()
    {
        return m_PreviousPosition;
    }

    bool Mouse::IsButtonPressed(Button aButton)
    {
        return m_ButtonStates[aButton].isPressed;
    }

    void Mouse::HandleMouseMoved(Vector2 aPosition, Vector2 aDelta)
    {
        //Set the previous position
        m_PreviousPosition = m_Position;

        //Set the new position
        m_Position = aPosition;

        //Dispatch the mouse event
        DispatchEvent(MouseMovementEvent(m_Position, m_PreviousPosition, aDelta));

        //Handle the callback
        if (m_MouseMoveCallback != nullptr)
        {
            m_MouseMoveCallback(m_Position.x, m_Position.y, m_PreviousPosition.x, m_PreviousPosition.y);
        }
    }
    
    void Mouse::HandleButtonDown(Button aButton)
    {
        //Set the button state's isPressed flag
        m_ButtonStates[aButton].isPressed = true;

        //Dispatch the Mouse event
        DispatchEvent(MouseButtonDownEvent(aButton, GetPosition()));

        //Handle the callback
        if (aButton == Left)
        {
            if (m_LeftMouseButtonPressedCallback != nullptr)
            {
                m_LeftMouseButtonPressedCallback(GetPosition().x, GetPosition().y);
            }
        }
        else if (aButton == Right)
        {
            if (m_RightMouseButtonPressedCallback != nullptr)
            {
                m_RightMouseButtonPressedCallback(GetPosition().x, GetPosition().y);
            }
        }
    }
    
    void Mouse::HandleButtonUp(Button aButton)
    {
        //Set the button state's isPressed flag
        m_ButtonStates[aButton].isPressed = false;

        //Dispatch the Mouse event
        DispatchEvent(MouseButtonUpEvent(aButton, m_ButtonStates[aButton].duration, GetPosition()));

        //Reset the duration value
        m_ButtonStates[aButton].duration = 0.0;

        //Handle the callback
        if (aButton == Left)
        {
            if (m_LeftMouseButtonReleasedCallback != nullptr)
            {
                m_LeftMouseButtonReleasedCallback(GetPosition().x, GetPosition().y);
            }
        }
        else if (aButton == Right)
        {
            if (m_RightMouseButtonReleasedCallback != nullptr)
            {
                m_RightMouseButtonReleasedCallback(GetPosition().x, GetPosition().y);
            }
        }
    }

    void Mouse::HandleScroll(float aDelta)
    {
        DispatchEvent(MouseScrollWheelEvent(aDelta));
    }
    
    std::string Mouse::ButtonToString(Mouse::Button aButton)
    {
        switch (aButton)
        {
            case Left:
                return "Left";
            
            case Center:
                return "Center";
                
            case Right:
                return "Right";
                
            default:
                break;
        }
        return "Unknown";
    }
}