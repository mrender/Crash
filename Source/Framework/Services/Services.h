#pragma once

#include "../Windows/Application.h"
#include "AudioEngine/AudioEngine.h"
#include "Graphics/Graphics.h"
#include "InputManager/InputManager.h"
#include "DebugUI/DebugUI.h"
#include "ResourceManager/ResourceManager.h"


namespace GameDev2D
{
    //Forward declarations
    class Application;

    //The Services class allows conveniant access to the Game's services (Graphics, InputManager, ResourceManager, DebugUI)
    class Services
    {
    public:
        //Initializes the default game services
        static void Init(Application* application);

        //Cleans up the default game services
        static void Cleanup();

        //Getter methods to access the Services's GameServices
        static Application* GetApplication();
		static AudioEngine* GetAudioEngine();
        static Graphics* GetGraphics();
        static ResourceManager* GetResourceManager();
        static InputManager* GetInputManager();
        static DebugUI* GetDebugUI();

    private:
        //Static variables for each GameService
        static Application* s_Application;
		static AudioEngine* s_AudioEngine;
        static Graphics* s_Graphics;
        static ResourceManager* s_ResourceManager;
        static InputManager* s_InputManager;
        static DebugUI* s_DebugUI;
    };
}
