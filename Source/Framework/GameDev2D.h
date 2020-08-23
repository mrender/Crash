#pragma once

//Include statements
#include "GameDev2D_Settings.h"
#include "Animation/Animator.h"
#include "Animation/Easing.h"
#include "Audio/Audio.h"
#include "Audio/AudioTypes.h"
#include "Core/Drawable.h"
#include "Core/Transformable.h"
#include "Debug/Log.h"
#include "Debug/Profile.h"
#include "Events/AudioEvent.h"
#include "Events/Event.h"
#include "Events/EventDispatcher.h"
#include "Events/EventHandler.h"
#include "Events/FullscreenEvent.h"
#include "Events/GamePadButtonDownEvent.h"
#include "Events/GamePadButtonUpEvent.h"
#include "Events/GamePadConnectedEvent.h"
#include "Events/GamePadDisconnectedEvent.h"
#include "Events/GamePadLeftThumbStickEvent.h"
#include "Events/GamePadLeftTriggerEvent.h"
#include "Events/GamePadRightThumbStickEvent.h"
#include "Events/GamePadRightTriggerEvent.h"
#include "Events/KeyUpEvent.h"
#include "Events/KeyDownEvent.h"
#include "Events/KeyRepeatEvent.h"
#include "Events/MouseButtonDownEvent.h"
#include "Events/MouseButtonUpEvent.h"
#include "Events/MouseMovementEvent.h"
#include "Events/MouseScrollWheelEvent.h"
#include "Events/ResizeEvent.h"
#include "Events/TextureResourceEvent.h"
#include "Graphics/AnimatedSprite.h"
#include "Graphics/Camera.h"
#include "Graphics/Color.h"
#include "Graphics/GraphicTypes.h"
#include "Graphics/Polygon.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/Shader.h"
#include "Graphics/Sprite.h"
#include "Graphics/SpriteAtlas.h"
#include "Graphics/SpriteBatch.h"
#include "Graphics/SpriteFont.h"
#include "Graphics/Texture.h"
#include "Graphics/VertexData.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "IO/File.h"
#include "Math/Math.h"
#include "Math/Matrix.h"
#include "Math/Rotation.h"
#include "Math/Vector2.h"
#include "Services/Services.h"
#include "Services/DebugUI/DebugUI.h"
#include "Services/Graphics/Graphics.h"
#include "Services/InputManager/InputManager.h"
#include "Services/ResourceManager/ResourceManager.h"
#include "Utils/Png/Png.h"
#include "Utils/Text/Text.h"
#include "Utils/Wave/Wave.h"
#include "Windows/Application.h"
#include "Windows/GameLoop.h"
#include "Windows/GameWindow.h"
#include "crtdbg.h"
#include <Windows.h>
#include <functional>

//Used to track down memory leaks
#ifdef _DEBUG
#if CHECK_FOR_MEMORY_LEAKS
#define new   new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif 


#define SafeDelete(p)  do{ if(p != nullptr) {delete p; p = nullptr;} }while(0)
#define SafeDeleteArray(p)   do{if(p != nullptr){delete[] p; p = nullptr;} }while(0)

namespace GameDev2D
{
    // The Run() function is the entry point into GameDev2D, you must provide four (4) functions to the Run() function
    //
    // 1. The first function, is an initialization function, that has a return type of void and take zero parameters.
    //   The initialization function will only be called once, shortly after the GameWindow is setup.
    //
    // 2. The second function, is an shutdown function, that has a return type of void and take zero parameters. The 
    //   shutdown function will only be called once, at the end of the application's lifecycle to ensure all 
    //   resources all properly deleted/unloaded.
    //   
    // 3. The third function, is an update function, that has a return type of void and take one parameter of type double,
    //   the update function is called once a frame, you should perform ALL game logic in the update function
    //
    // 4. The fourth function, is an draw function, that has a return type of void and takes zero parameters, the draw 
    //   function is called once a frame, you should perform ALL rendering and ONLY rendering in the draw function.
    //   Drawing anything outside of the draw function will not work.
    void Run(std::function<void()> initCallback, std::function<void()> shutdownCallback, std::function<void(double)> updateCallback, std::function<void()> drawCallback);


    // Returns the width of the Window
    unsigned int GetScreenWidth();


    // Returns the height of the Window
    unsigned int GetScreenHeight();


	// Loads an audio file. All audio files MUST be of type wav. If the audio file doesn't exist, then a default tone will be 
	// loaded instead
	void LoadAudio(const std::string& filename);


	// Unloads an already loaded audio file. You must make sure to unload all audio files that you load, if you don't then you 
	// aren't properly releasing precious memory back to the operating system
	void UnloadAudio(const std::string& filename);


    // Loads a Texture from a file. All Texture files MUST be of type png. If a Texture file doesn't exist a default 'checkboard'
    // Texture will be loaded in its place.
    void LoadTexture(const std::string& filename);


    // Unloads an already loaded Texture. You must make sure to unload any Texture that you load, if you don't you aren't properly 
    // releasing precious memory back to the operating system
    void UnloadTexture(const std::string& filename);


	// Loads a Font from a file. Font files can be of type ttf or otf. If a Font file doesn't exist a default 'font'
	// will be loaded in its place.
	void LoadFont(const std::string& filename);


	// Unloads an already loaded Font. You must make sure to unload any Font that you load, if you don't you are properly 
	// releasing precious memory back to the operating system
	void UnloadFont(const std::string& filename);


	// Registers a callback function to be called whenever the key is pressed down
	void RegisterKeyPressedCallback(std::function<void(Keyboard::Key)> keyPressedCallback);


    // Registers a callback function to be called whenever the key is released
    void RegisterKeyReleasedCallback(std::function<void(Keyboard::Key)> keyReleasedCallback);


	// Registers a callback function to be called whenever the left mouse button is clicked down
	void RegisterLeftMouseButtonPressedCallback(std::function<void(float, float)> leftMouseButtonPressedCallback);


    // Registers a callback function to be called whenever the left mouse button is released
    void RegisterLeftMouseButtonReleasedCallback(std::function<void(float, float)> leftMouseButtonReleasedCallback);


	// Registers a callback function to be called whenever the right mouse button is clicked
	void RegisterRightMouseButtonPressedCallback(std::function<void(float, float)> rightMouseButtonPressedCallback);


    // Registers a callback function to be called whenever the right mouse button is released
    void RegisterRightMouseButtonReleasedCallback(std::function<void(float, float)> rightMouseButtonReleasedCallback);


	// Registers a callback function to be called whenever the mouse moves
	void RegisterMouseMoveCallback(std::function<void(float, float, float, float)> mouseMoveCallback);


    // Returns wether a keyboard key is Up or not
    bool IsKeyUp(unsigned int key);


    // Returns wether a keyboard key is Down or not
    bool IsKeyDown(unsigned int key);


    // Returns wether the left mouse button is Up or not
    bool IsLeftMouseButtonUp();


    // Returns wether the left mouse button is Down or not
    bool IsLeftMouseButtonDown();


    // Returns wether the right mouse button is Up or not
    bool IsRightMouseButtonUp();


    // Returns wether the right mouse button is Down or not
    bool IsRightMouseButtonDown();


    // Returns the current position of the mouse
    Vector2 GetMousePosition();
}