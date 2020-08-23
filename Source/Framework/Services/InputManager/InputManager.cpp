#include "InputManager.h"
#include "../Services.h"
#include "../../Events/UpdateEvent.h"
#include "../../Windows/Application.h"
#include "../../Debug/Log.h"


namespace GameDev2D
{
    InputManager::InputManager() : EventHandler(),
        m_Keyboard(nullptr),
        m_Mouse(nullptr)
    {
        //Create the Keyboard object
        m_Keyboard = new Keyboard();

        //Create the Mouse object
        m_Mouse = new Mouse();

        //Create the GamePad objects, one for each port
        for (unsigned int port = 0; port < GamePad::PortCount; port++)
        {
            m_GamePads[port] = new GamePad(static_cast<GamePad::Port>(port));
        }

        //Add an event listener callback for the Update event
        Services::GetApplication()->AddEventListener(this, UPDATE_EVENT);
    }
    
    InputManager::~InputManager()
    {
        if (m_Keyboard != nullptr)
        {
            delete m_Keyboard;
            m_Keyboard = nullptr;
        }

        if (m_Mouse != nullptr)
        {
            delete m_Mouse;
            m_Mouse = nullptr;
        }

        for (unsigned int i = 0; i < GamePad::PortCount; i++)
        {
            if (m_GamePads[i] != nullptr)
            {
                delete m_GamePads[i];
                m_GamePads[i] = nullptr;
            }
        }

        //Remove the event listener callback for the Update event
        Services::GetApplication()->RemoveEventListener(this, UPDATE_EVENT);
    }

    void InputManager::HandleEvent(Event* aEvent)
    {
        if (aEvent != nullptr)
        {
            if (aEvent->GetEventCode() == UPDATE_EVENT)
            {
                //Get the UpdateEvent
                UpdateEvent* updateEvent = (UpdateEvent*)aEvent;

                //Update the Camera
                Update(updateEvent->GetDelta());
            }
        }
    }

    void InputManager::Update(double aDelta)
    {
        //Update the Keyboard
        if (m_Keyboard != nullptr)
        {
            m_Keyboard->Update(aDelta);
        }

        //Update the Mouse
        if (m_Mouse != nullptr)
        {
            m_Mouse->Update(aDelta);
        }

        //Check with the GameWindow for GamePad input
        if (Services::GetApplication()->GetWindow() != nullptr)
        {
            Services::GetApplication()->GetWindow()->CheckForGamePadInput();
        }

        //Create the GamePad objects, one for each port
        for (unsigned int i = 0; i < GamePad::PortCount; i++)
        {
            if (m_GamePads[i] != nullptr)
            {
                m_GamePads[i]->Update(aDelta);
            }
        }
    }

    Keyboard* InputManager::GetKeyboard()
    {
        return m_Keyboard;
    }

    Mouse* InputManager::GetMouse()
    {
        return m_Mouse;
    }

    GamePad* InputManager::GetGamePad(GamePad::Port aPort)
    {
        return m_GamePads[aPort];
    }
    
    void InputManager::HandleKeyDown(unsigned int aKey)
    {
        //Ensure the application hasn't been suspended
        if (Services::GetApplication()->IsSuspended() == true)
        {
            return;
        }

        //Have the Keyboard handle the key up
        if (m_Keyboard != nullptr)
        {
            m_Keyboard->HandleKeyDown(static_cast<Keyboard::Key>(aKey));
        }
    }

    void InputManager::HandleKeyUp(unsigned int aKey)
    {
        //Ensure the application hasn't been suspended
        if (Services::GetApplication()->IsSuspended() == true)
        {
            return;
        }

        //Have the Keyboard handle the key up
        if (m_Keyboard != nullptr)
        {
            m_Keyboard->HandleKeyUp(static_cast<Keyboard::Key>(aKey));
        }
    }
    
    void InputManager::HandleLeftMouseDown(float aX, float aY)
    {
        //Ensure the application hasn't been suspended
        if(Services::GetApplication()->IsSuspended() == true)
        {
            return;
        }
    
        //Set that the left mouse button is pressed
        m_Mouse->HandleButtonDown(Mouse::Left);
    }
    
    void InputManager::HandleLeftMouseUp(float aX, float aY)
    {
        //Ensure the application hasn't been suspended
        if(Services::GetApplication()->IsSuspended() == true)
        {
            return;
        }

        //Set that the left mouse button is pressed
        m_Mouse->HandleButtonUp(Mouse::Left);
    }
    
    void InputManager::HandleCenterMouseDown(float aX, float aY)
    {
        //Ensure the application hasn't been suspended
        if(Services::GetApplication()->IsSuspended() == true)
        {
            return;
        }
    
        //Set that the left mouse button is pressed
        m_Mouse->HandleButtonDown(Mouse::Center);
    }
    
    void InputManager::HandleCenterMouseUp(float aX, float aY)
    {
        //Ensure the application hasn't been suspended
        if(Services::GetApplication()->IsSuspended() == true)
        {
            return;
        }
    
        //Set that the left mouse button is pressed
        m_Mouse->HandleButtonUp(Mouse::Center);
    }
    
    void InputManager::HandleRightMouseDown(float aX, float aY)
    {
        //Ensure the application hasn't been suspended
        if(Services::GetApplication()->IsSuspended() == true)
        {
            return;
        }
    
        //Set that the left mouse button is pressed
        m_Mouse->HandleButtonDown(Mouse::Right);
    }
    
    void InputManager::HandleRightMouseUp(float aX, float aY)
    {
        //Ensure the application hasn't been suspended
        if(Services::GetApplication()->IsSuspended() == true)
        {
            return;
        }
    
        //Set that the left mouse button is pressed
        m_Mouse->HandleButtonUp(Mouse::Right);
    }
    
    void InputManager::HandleMouseMoved(float aX, float aY, float aDeltaX, float aDeltaY)
    {
        //Ensure the application hasn't been suspended
        if (Services::GetApplication()->IsSuspended() == true)
        {
            return;
        }

        //Set the mouse data's position
        m_Mouse->HandleMouseMoved(Vector2(aX, aY), Vector2(aDeltaX, aDeltaY));
    }
    
    void InputManager::HandleScrollWheel(float aDelta)
    {
        //Ensure the application hasn't been suspended
        if(Services::GetApplication()->IsSuspended() == true)
        {
            return;
        }

        //Pass the scroll information onto the Mouse
        m_Mouse->HandleScroll(aDelta);
    }
}