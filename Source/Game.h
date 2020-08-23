#pragma once

#define Poly() GameDev2D::Polygon();

#include <vector>

// Class definition
class Game
{
public:
    Game();  // Constructor
    ~Game(); // Destructor

    // Write game logic in the Update method
    void Update(double delta);
    void UpdateText();

    // Render game objects in the Draw method
    void Draw();
    void DrawPreviousBets();

    // Input methods
    void HandleLeftMouseClick(float mouseX, float mouseY);
    void HandleRightMouseClick(float mouseX, float mouseY);
    void HandleMouseMove(float mouseX, float mouseY, float previousX, float previousY);
    void HandleKeyPress(GameDev2D::Keyboard::Key key);
    void HandleKeyReleased(GameDev2D::Keyboard::Key key);

    void Reset();
    void ResetShipPoints();
    void ResetShip();

    //Handles
    void IncrementPlayerBet();
    void DecrementPlayerBet();
    void HandleShipMovement(double delta);
    void HandleStop();
    void HandleCrash();
    void HandleCrashCounter();
    void HandlePreviousBets();
    void RemovePreviousBet();
    void OrganizePreviousBets();
    void HandleMovingShipPoint();

    //Calculations
    void CalculateMultiplier(double delta);
    void CalculateMaxMultiplier();
    double CalculateRandom(double min, double max);
    bool CalculateComparison();


private:
    // Member variables GO HERE 

    //Sprite Pointers
    GameDev2D::Sprite* m_pShip;

    //Sprite Font Pointers
    GameDev2D::SpriteFont* m_pPlayerBet;
    GameDev2D::SpriteFont* m_pMultiplier;
    GameDev2D::SpriteFont* m_pPlayerBalance;
    GameDev2D::SpriteFont* m_pCrash;
    GameDev2D::SpriteFont* m_pBetGuide;
    GameDev2D::SpriteFont* m_pPreviousBets;

    //Polygon Pointers
    GameDev2D::Polygon* m_pBetBox;
    GameDev2D::Polygon* m_pBetBoxBorder;
    GameDev2D::Polygon* m_pBetBoxSeperator1;
    GameDev2D::Polygon* m_pBetBoxSeperator2;
    GameDev2D::Polygon* m_pBetBoxSeperator3;
    GameDev2D::Polygon* m_pShipPath;

    //Integer Members
    int m_PlayerBalance;
    int m_PlayerBet;
    int m_CrashCounter;

    //Double Members
    double m_Multiplier;
    double m_Maximum;

    //Bool Members
    bool m_HasStarted;
    bool m_HasCrashed;

    //Float Memebers
    float m_Angle;
    float m_Speed;
    float m_CurrentSpeed;

    //Vector2 Members
    GameDev2D::Vector2 m_MovingShipPoint;
    GameDev2D::Vector2 m_ShipVector;

    //Vector Members
    std::vector<int> m_PreviousBets;
};
