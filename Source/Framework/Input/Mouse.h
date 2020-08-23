#ifndef __GameDev2D__MouseData__
#define __GameDev2D__MouseData__

#include "../Math/Vector2.h"
#include "../Events/EventDispatcher.h"
#include <map>
#include <string>


namespace GameDev2D
{
    //The Mouse class holds all relevant mouse data, this class is managed by the InputManager.
    class Mouse : public EventDispatcher
    {
    public:
        Mouse();
        ~Mouse();
        
        //Updates the mouse data, specifically to update the duration a
        //mouse button has been pressed for.
        void Update(double delta);
        
        // Registers a callback function to be called whenever the left or right mouse button is pressed OR released
        void RegisterLeftMouseButtonPressedCallback(std::function<void(float, float)> leftMouseButtonPressedCallback);
        void RegisterLeftMouseButtonReleasedCallback(std::function<void(float, float)> leftMouseButtonReleasedCallback);
        void RegisterRightMouseButtonPressedCallback(std::function<void(float, float)> rightMouseButtonPressedCallback);
        void RegisterRightMouseButtonReleasedCallback(std::function<void(float, float)> rightMouseButtonReleasedCallback);

        // Registers a callback function to be called whenever the mouse moves
        void RegisterMouseMoveCallback(std::function<void(float, float, float, float)> mouseMoveCallback);

        //Returns the current mouse's position
        Vector2 GetPosition();
        
        //Returns the previous mouse's position
        Vector2 GetPreviousPosition();

        //This enum describes the mouse
        //buttons on a standard mouse
        enum Button
        {
            Left = 0,
            Center,
            Right
        };

        //Returns wether a mouse button is pressed or not
        bool IsButtonPressed(Button button);

        //Conveniance method to convert a Mouse button to a string
        static std::string ButtonToString(Button button);
            
    private:
        friend class InputManager;

        //Sets the current mouse's position
        void HandleMouseMoved(Vector2 position, Vector2 delta);

        //Sets that a mouse button has been pressed or released
        void HandleButtonDown(Button button);
        void HandleButtonUp(Button button);

        //Dispatches a Mouse scrolled event
        void HandleScroll(float delta);

        //Manages the internal State for each Mouse Button
        struct State
        {
            State() :
            duration(0.0),
            isPressed(isPressed)
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
        Vector2 m_Position;
        Vector2 m_PreviousPosition;
        std::map<Button, State> m_ButtonStates;
        std::function<void(float, float)> m_LeftMouseButtonPressedCallback;
        std::function<void(float, float)> m_LeftMouseButtonReleasedCallback;
        std::function<void(float, float)> m_RightMouseButtonPressedCallback;
        std::function<void(float, float)> m_RightMouseButtonReleasedCallback;
        std::function<void(float, float, float, float)> m_MouseMoveCallback;
    };
}

#endif
