#include <GameDev2D.h>
#include "Game.h"
#include "Constants.h"

; using namespace GameDev2D;

Game::Game() :
    m_PlayerBalance(100),
    m_PlayerBet(0),
    m_CrashCounter(0),
    m_Multiplier(MIN_MULTIPLIER),
    m_Maximum(0.0),
    m_HasStarted(false),
    m_HasCrashed(false),
    m_MovingShipPoint(SECOND_VERTICE),
    m_ShipVector(Vector2::Zero),
    m_Angle(0.0f),
    m_Speed(0.0f),
    m_CurrentSpeed(0.0f)
{
    //Load Dependencies
    LoadTexture("Ship");
    LoadFont("OpenSans-CondBold_32");

    //Initialize Sprite Pointers
    m_pShip = new Sprite("Ship");
    m_pShip->SetAnchor(Vector2(0.5f, 0.5f));

    //Initialize Sprite Font Pointers
    m_pPlayerBet = new SpriteFont("OpenSans-CondBold_32");
    m_pPlayerBet->SetAnchor(MIDDLE_ANCHOR);
    m_pPlayerBet->SetColor(Color::CrimsonRedColor());
    m_pPlayerBet->SetText("BET: " + to_string(m_PlayerBet));
    m_pPlayerBet->SetScale(Vector2(1.5f, 1.5f));
    m_pPlayerBet->SetPosition(Vector2(170.0f, WINDOW_HEIGHT - 200.0f));

    m_pPlayerBalance = new SpriteFont("OpenSans-CondBold_32");
    m_pPlayerBalance->SetAnchor(MIDDLE_ANCHOR);
    m_pPlayerBalance->SetColor(Color::CrimsonRedColor());
    m_pPlayerBalance->SetText("BALANCE: " + to_string(m_PlayerBalance));
    m_pPlayerBalance->SetScale(Vector2(1.5f, 1.5f));
    m_pPlayerBalance->SetPosition(Vector2(175.0f, WINDOW_HEIGHT - 75.0f));

    m_pMultiplier = new SpriteFont("OpenSans-CondBold_32");
    m_pMultiplier->SetAnchor(MIDDLE_ANCHOR);
    m_pMultiplier->SetColor(Color::WhiteColor());
    m_pMultiplier->SetText(to_string(m_Multiplier) + "x");
    m_pMultiplier->SetScale(Vector2(2.0f, 2.0f));
    m_pMultiplier->SetPosition(Vector2(WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2) - 100.0f));

    m_pCrash = new SpriteFont("OpenSans-CondBold_32");
    m_pCrash->SetAnchor(MIDDLE_ANCHOR);
    m_pCrash->SetColor(Color::CrimsonRedColor());
    m_pCrash->SetText("CRASH");
    m_pCrash->SetScale(Vector2(3.0f, 3.0f));
    m_pCrash->SetPosition(Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 50.0f));

    m_pBetGuide = new SpriteFont("OpenSans-CondBold_32");
    m_pBetGuide->SetAnchor(MIDDLE_ANCHOR);
    m_pBetGuide->SetColor(Color::CrimsonRedColor());
    m_pBetGuide->SetText("   +UP: INCREASE+\n-DOWN: DECREASE-");
    m_pBetGuide->SetPosition(Vector2(170.0f, WINDOW_HEIGHT - 300.0f));

    m_pPreviousBets = new SpriteFont("OpenSans-CondBold_32");
    m_pPreviousBets->SetAnchor(MIDDLE_ANCHOR);
    m_pPreviousBets->SetColor(Color::CrimsonRedColor());
    m_pPreviousBets->SetText("PREVIOUS BET: ");
    m_pPreviousBets->SetPosition(Vector2(170.0f, WINDOW_HEIGHT - 500.0f));

    //Initiliaze Polygon Pointers
    m_pBetBox = new Poly();
    m_pBetBox->MakeRectangle(500.0f, WINDOW_HEIGHT, true);
    m_pBetBox->SetAnchor(MIDDLE_ANCHOR);
    m_pBetBox->SetColor(Color::BlackColor());
    m_pBetBox->SetPosition(Vector2(100.0f, WINDOW_HEIGHT / 2));

    m_pBetBoxBorder = new Poly();
    m_pBetBoxBorder->MakeRectangle(500.0f, WINDOW_HEIGHT, false);
    m_pBetBoxBorder->SetAnchor(MIDDLE_ANCHOR);
    m_pBetBoxBorder->SetColor(Color::WhiteColor());
    m_pBetBoxBorder->SetPosition(Vector2(100.0f, WINDOW_HEIGHT / 2));

    m_pBetBoxSeperator1 = new Poly();
    m_pBetBoxSeperator1->MakeRectangle(500.0f, 10.0f, true);
    m_pBetBoxSeperator1->SetAnchor(MIDDLE_ANCHOR);
    m_pBetBoxSeperator1->SetColor(Color::WhiteColor());
    m_pBetBoxSeperator1->SetPosition(Vector2(100.0f, WINDOW_HEIGHT / 2));

    m_pBetBoxSeperator2 = new Poly();
    m_pBetBoxSeperator2->MakeRectangle(500.0f, 10.0f, true);
    m_pBetBoxSeperator2->SetAnchor(MIDDLE_ANCHOR);
    m_pBetBoxSeperator2->SetColor(Color::WhiteColor());
    m_pBetBoxSeperator2->SetPosition(Vector2(100.0f, WINDOW_HEIGHT - 150.0f));

    m_pBetBoxSeperator3 = new Poly();
    m_pBetBoxSeperator3->MakeRectangle(500.0f, 4.5f, true);
    m_pBetBoxSeperator3->SetAnchor(MIDDLE_ANCHOR);
    m_pBetBoxSeperator3->SetColor(Color::WhiteColor());
    m_pBetBoxSeperator3->SetPosition(Vector2(100.0f, 37.5f));

    m_pShipPath = new Poly();
    m_pShipPath->SetRenderMode(RenderMode_LineStrip);
    m_pShipPath->SetColor(Color::WhiteColor());
    m_pShipPath->AddVertex(FIRST_VERTICE);
    m_pShipPath->AddVertex(SECOND_VERTICE);


    //Initialize Game
    Reset();
}

Game::~Game()
{
    //Make Sprite Pointers Null
    if (m_pShip != nullptr)
    {
        delete m_pShip;
        m_pShip = nullptr;
    }

    //Make Sprite Font Pointers Null
    if (m_pPlayerBet != nullptr)
    {
        delete m_pPlayerBet;
        m_pPlayerBet = nullptr;
    }
    if (m_pPlayerBalance != nullptr)
    {
        delete m_pPlayerBalance;
        m_pPlayerBalance = nullptr;
    }
    if (m_pMultiplier != nullptr)
    {
        delete m_pMultiplier;
        m_pMultiplier = nullptr;
    }
    if (m_pCrash != nullptr)
    {
        delete m_pCrash;
        m_pCrash = nullptr;
    }
    if (m_pBetGuide != nullptr)
    {
        delete m_pBetGuide;
        m_pBetGuide = nullptr;
    }


    //Make Polygon Pointers Null
    if (m_pBetBox != nullptr)
    {
        delete m_pBetBox;
        m_pBetBox = nullptr;
    }
    if (m_pBetBoxBorder != nullptr)
    {
        delete m_pBetBoxBorder;
        m_pBetBoxBorder = nullptr;
    }
    if (m_pBetBoxSeperator1 != nullptr)
    {
        delete m_pBetBoxSeperator1;
        m_pBetBoxSeperator1 = nullptr;
    }
    if (m_pBetBoxSeperator2 != nullptr)
    {
        delete m_pBetBoxSeperator2;
        m_pBetBoxSeperator2 = nullptr;
    }
    if (m_pBetBoxSeperator3 != nullptr)
    {
        delete m_pBetBoxSeperator3;
        m_pBetBoxSeperator3 = nullptr;
    }
    if (m_pShipPath != nullptr)
    {
        delete m_pShipPath;
        m_pShipPath = nullptr;
    }

    //Unload Dependencies
    UnloadTexture("Ship");
    UnloadFont("OpenSans-CondBold_32");
}

void Game::Update(double delta)
{
    if (m_HasStarted && m_HasCrashed == false)
    {
        CalculateMultiplier(delta);
    }
    if (m_HasCrashed)
    {
        HandleCrashCounter();
    }

    UpdateText();

}

void Game::UpdateText()
{
    m_pPlayerBet->SetText("BET: " + to_string(m_PlayerBet));
    m_pPlayerBalance->SetText("BALANCE: " + to_string(m_PlayerBalance));

    double value = m_Multiplier;
    value = std::ceil(value * 100.0) / 100.0;
    m_pMultiplier->SetText(to_string(value) + "x");
}

void Game::Draw()
{
    //Draw Bet Box
    m_pBetBox->Draw();
    m_pBetBoxBorder->Draw();
    m_pBetBoxSeperator1->Draw();
    m_pBetBoxSeperator2->Draw();
    m_pBetBoxSeperator3->Draw();

    DrawPreviousBets();

    m_pShipPath->Draw();
    m_pShip->Draw();
    m_pPlayerBalance->Draw();

    m_pPlayerBet->Draw();
    m_pBetGuide->Draw();

    if (m_HasStarted)
    {
        m_pMultiplier->Draw();
    }

    if (m_HasCrashed)
    {
        m_pCrash->Draw();
    }
}

void Game::DrawPreviousBets()
{
    for (int i = 0; i < m_PreviousBets.size(); i++)
    {
        int bet = m_PreviousBets.at(i);
        if (bet > 0)
        {
            if (i == m_PreviousBets.size() - 1)
            {
                m_pPreviousBets->SetText("RECENT WIN: +" + to_string(bet));
            }
            else
            {
                m_pPreviousBets->SetText("WON: +" + to_string(bet));
            }
            m_pPreviousBets->SetPosition(Vector2(170.0f, WINDOW_HEIGHT - 400.0f - (i * 30.0f)));
            if (m_pPreviousBets->GetPosition().y <= 10.0f)
            {
                RemovePreviousBet();
                break;
            }

            m_pPreviousBets->Draw();
        }
        if (bet < 0)
        {
            if (i == m_PreviousBets.size() - 1)
            {
                m_pPreviousBets->SetText("RECENT LOST: " + to_string(bet));
            }
            else
            {
                m_pPreviousBets->SetText("LOST: " + to_string(bet));
            }
            m_pPreviousBets->SetPosition(Vector2(170.0f, WINDOW_HEIGHT - 400.0f - (i * 30.0f)));
            m_pPreviousBets->Draw();
            if (m_pPreviousBets->GetPosition().y <= 10.0f)
            {
                RemovePreviousBet();
                break;
            }
        }
    }
}

void Game::HandleLeftMouseClick(float aMouseX, float aMouseY)
{

}

void Game::HandleRightMouseClick(float aMouseX, float aMouseY)
{

}

void Game::HandleMouseMove(float aMouseX, float aMouseY, float aPreviousX, float aPreviousY)
{

}

void Game::HandleKeyPress(Keyboard::Key aKey)
{
    if( aKey == Keyboard::Key::Escape )
    {
        Services::GetApplication()->Shutdown();
    }
    if (aKey == Keyboard::Key::Return && m_PlayerBet >= 10 && m_PlayerBalance >= m_PlayerBet && m_HasStarted == false)
    {
        m_HasStarted = true;
        m_PlayerBalance -= m_PlayerBet;
    }
    if (aKey == Keyboard::Key::Up && m_HasStarted == false)
    {
        IncrementPlayerBet();
    }
    if (aKey == Keyboard::Key::Down && m_HasStarted == false)
    {
        DecrementPlayerBet();
    }
    if (aKey == Keyboard::Key::Spacebar && m_HasStarted && m_HasCrashed == false)
    {
        HandleStop();
    }
}

void Game::HandleKeyReleased(Keyboard::Key key)
{

}

void Game::Reset()
{
    m_HasCrashed = false;
    m_HasStarted = false;
    ResetShipPoints();
    ResetShip();
    m_CrashCounter = 0;
    m_PlayerBet = 0;
    CalculateMaxMultiplier();
    m_Multiplier = MIN_MULTIPLIER;
}

void Game::ResetShipPoints()
{
    m_MovingShipPoint = SECOND_VERTICE;
    m_pShipPath->ClearVertices();
    m_pShipPath->AddVertex(FIRST_VERTICE);
    m_pShipPath->AddVertex(SECOND_VERTICE);
}

void Game::ResetShip()
{
    m_ShipVector = Vector2(FIRST_VERTICE.x + 50.0f, FIRST_VERTICE.y);

    m_Speed = 0.0f;
    Vector2 tempVector = Vector2(m_MovingShipPoint.x - m_ShipVector.x, m_MovingShipPoint.y - m_ShipVector.y);
    m_Angle = atan2f(tempVector.y, tempVector.x);

    m_pShip->SetPosition(m_ShipVector);
    m_pShip->SetRadians(m_Angle);
}

void Game::IncrementPlayerBet()
{
    m_PlayerBet += 10;
}

void Game::DecrementPlayerBet()
{
    if (m_PlayerBet >= 10)
    {
        m_PlayerBet -= 10;
    }
}

void Game::HandleShipMovement(double delta)
{
    Vector2 tempVector = Vector2(m_MovingShipPoint.x - m_ShipVector.x, m_MovingShipPoint.y - m_ShipVector.y);
    m_Angle = atan2f(tempVector.y, tempVector.x);
    m_Speed += m_Multiplier * delta;
    Vector2 direction = Vector2((cosf(m_Angle)), (sinf(m_Angle)));
    Vector2 velocity = direction * m_Speed;
    Vector2 position = m_ShipVector + velocity;

    m_ShipVector = position;
    m_pShip->SetPosition(m_ShipVector);
    m_pShip->SetRadians(m_Angle);

    if (m_ShipVector.x >= WINDOW_WIDTH || m_ShipVector.y >= WINDOW_HEIGHT)
    {
        m_ShipVector = Vector2(FIRST_VERTICE.x + 50.0f, FIRST_VERTICE.y);
        m_Speed -= m_Multiplier * delta;
        m_pShip->SetPosition(m_ShipVector);
        m_pShip->SetRadians(m_Angle);
    }
}

void Game::HandleStop()
{
    if (CalculateComparison() == false)
    {
        m_PlayerBalance += m_PlayerBet * m_Multiplier;
        HandlePreviousBets();
        Reset();
    }
    else
    {
        HandleCrash();
    }
}

void Game::HandleCrash()
{
    m_HasCrashed = true;
    HandlePreviousBets();
}

void Game::HandleCrashCounter()
{
    m_CrashCounter++;
    if (m_CrashCounter == 150)
    {
        Reset();
    }
}

void Game::HandlePreviousBets()
{
    if (m_HasCrashed)
    {
        int bet = -m_PlayerBet;
        m_PreviousBets.push_back(bet);
    }
    else
    {
        int bet = m_PlayerBet * m_Multiplier;
        m_PreviousBets.push_back(bet);
    }
}

void Game::RemovePreviousBet()
{
    int indexA = 0;
    int indexB = m_PreviousBets.size() - 1;

    int tempElementA = m_PreviousBets.at(indexA);
    int tempElementB = m_PreviousBets.at(indexB);

    m_PreviousBets.at(indexA) = tempElementB;
    m_PreviousBets.at(indexB) = tempElementA;
    m_PreviousBets.pop_back();
    OrganizePreviousBets();
}

void Game::OrganizePreviousBets()
{
    for (int i = 0; i < m_PreviousBets.size() - 1; i++)
    {
        int indexA = i + 1;
        int indexB = i;

        int tempElementA = m_PreviousBets.at(indexA);
        int tempElementB = m_PreviousBets.at(indexB);

        m_PreviousBets.at(indexA) = tempElementB;
        m_PreviousBets.at(indexB) = tempElementA;
    }
}

void Game::HandleMovingShipPoint()
{
    m_pShipPath->ClearVertices();

    m_MovingShipPoint.y += m_Multiplier / MAX_MULTIPLIER;

    m_pShipPath->AddVertex(FIRST_VERTICE);
    m_pShipPath->AddVertex(m_MovingShipPoint);
}

void Game::CalculateMultiplier(double delta)
{
    m_Multiplier += delta / 2;

    HandleMovingShipPoint();

    HandleShipMovement(delta);

    if (CalculateComparison())
    {
        HandleCrash();
    }
}

void Game::CalculateMaxMultiplier()
{
    m_Maximum = CalculateRandom(MIN_MULTIPLIER, MAX_MULTIPLIER);
}

double Game::CalculateRandom(double min, double max)
{
    random_device seed;
    mt19937 mersenneTwister(seed());

    uniform_int_distribution<int> distribution(min, max);
    return distribution(mersenneTwister);
}

bool Game::CalculateComparison()
{
    if (m_Multiplier >= m_Maximum)
    {
        return true;
    }

    return false;
}
