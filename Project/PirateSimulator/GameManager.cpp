#include "GameManager.h"

using namespace PirateSimulator;

GameManager *GameManager::m_instance;

GameManager::GameManager()
{
    m_gameState = GameState::MainMenu;

}
void GameManager::initialize()
{
    m_mainScreen.initialize();
}

void GameManager::setGameState(const GameState &state)
{
    m_gameState = state;
    if(state == GameState::Loading)
        m_loadingScreen.initialize();
    if(state == GameState::InGame)
        TimeManager::GetInstance().startGameTime();
}

const GameState &GameManager::getGameState() const
{
    return m_gameState;
}

void GameManager::update()
{
    switch(m_gameState)
    {
        case GameState::MainMenu :
            m_mainScreen.update();
            break;
        case GameState::Loading:
            m_loadingScreen.update();
            break;
        case GameState::Pause:
            m_pauseScreen.update();
            break;
        case GameState::InGame:
        {
        }
        default:
            break;
    }
}

void GameManager::pause()
{
    if(m_gameState == GameState::InGame)
        m_gameState = GameState::Pause;
    else
    {
        m_gameState = GameState::InGame;
        TimeManager::GetInstance().startGameTime();
    }
}

