#ifndef __GameDev2D__InputManager__
#define __GameDev2D__InputManager__

#include "../../Events/EventHandler.h"
#include "../../Input/Keyboard.h"
#include "../../Input/Mouse.h"
#include "../../Input/GamePad.h"
#include "../../Math/Vector2.h"
#include <map>


namespace GameDev2D
{
    //Forward declarations
    class MouseMovementEvent;

    // The InputManager game service handles all input for all platforms, including mouse, keyboard and GamePad
    class InputManager : public EventHandler
    {
    public:
        InputManager();
        ~InputManager();
        
        //The HandleEvent is used to notify the InputManager class of various GameDev2D events
        void HandleEvent(Event* event);

        //Used to Update the mouse, keyboard and GamePad objects
        void Update(double delta);

        //Returns the Keyboard object, which manages the button state for all the keys
        Keyboard* GetKeyboard();

        //Returns the Mouse object, which manages the position and button states for the mouse buttons
        Mouse* GetMouse();

        //Returns the GamePad for a port, the GamePad the thumb stick, trigger and button states
        GamePad* GetGamePad(GamePad::Port port);

        //Methods to handle Keyboard input
        void HandleKeyDown(unsigned int key);
        void HandleKeyUp(unsigned int key);

        //Methods to handle Mouse input
        void HandleLeftMouseDown(float x, float y);
        void HandleLeftMouseUp(float x, float y);
        void HandleCenterMouseDown(float x, float y);
        void HandleCenterMouseUp(float x, float y);
        void HandleRightMouseDown(float x, float y);
        void HandleRightMouseUp(float x, float y);
        void HandleMouseMoved(float x, float y, float deltaX, float deltaY);
        void HandleScrollWheel(float delta);
    
    private:
        //Keyboard data
        Keyboard* m_Keyboard;
        Mouse* m_Mouse;
        GamePad* m_GamePads[GamePad::PortCount];
    };
}

#endif
