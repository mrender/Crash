#include <GameDev2D.h>
#include "Game.h"

//Function prototypes
void Init();
void Shutdown();
void Update(double delta);
void Draw();
void LeftMouseClicked(float x, float y);
void RightMouseClicked(float x, float y);
void MouseMoved(float x, float y, float previousX, float previousY);
void KeyPressed(GameDev2D::Keyboard::Key key);
void KeyReleased(GameDev2D::Keyboard::Key key);


//The ONLY Global variable you should have in your project
Game* game = nullptr;

//Entry point to the application
int WINAPI WinMain(HINSTANCE aCurrentInstance, HINSTANCE aPreviousInstance, LPSTR aCommandLine, int aCommandShow)
{
    //Run GameDev2D, pass in the Init, Shutdown, Update and Draw methods
    GameDev2D::Run(Init, Shutdown, Update, Draw);
    return 0;
}

void Init()
{
	//Create the Game object
	game = new Game();

    //Register the callbacks
    GameDev2D::RegisterLeftMouseButtonPressedCallback(LeftMouseClicked);
    GameDev2D::RegisterRightMouseButtonPressedCallback(RightMouseClicked);
    GameDev2D::RegisterMouseMoveCallback(MouseMoved);
    GameDev2D::RegisterKeyPressedCallback(KeyPressed);
	GameDev2D::RegisterKeyReleasedCallback(KeyReleased);
}

void Shutdown()
{
    if (game != nullptr)
    {
        delete game;
        game = nullptr;
    }
}

void Update(double aDelta)
{
    if (game != nullptr)
    {
        game->Update(aDelta);
    }
}

void Draw()
{
    if (game != nullptr)
    {
        game->Draw();
    }
}

void LeftMouseClicked(float aX, float aY)
{
    if (game != nullptr)
    {
        game->HandleLeftMouseClick(aX, aY);
    }
}

void RightMouseClicked(float aX, float aY)
{
    if (game != nullptr)
    {
        game->HandleRightMouseClick(aX, aY);
    }
}

void KeyPressed(GameDev2D::Keyboard::Key aKey)
{
    if (game != nullptr)
    {
        game->HandleKeyPress(aKey);
    }
}

void KeyReleased(GameDev2D::Keyboard::Key aKey)
{
	if (game != nullptr)
	{
		game->HandleKeyReleased(aKey);
	}
}

void MouseMoved(float aX, float aY, float aPreviousX, float aPreviousY)
{
    if (game != nullptr)
    {
        game->HandleMouseMove(aX, aY, aPreviousX, aPreviousY);
    }
}