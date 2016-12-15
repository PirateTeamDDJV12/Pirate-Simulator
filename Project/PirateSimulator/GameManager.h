#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_


#include "UIMenu.h"
#include "UILoading.h"
#include "UIPause.h"
#include "TimeManager.h"

namespace PirateSimulator
{
    enum GameState
    {
        MainMenu,
        Loading,
        Options,
        Credits,
        Quitting,
        InGame,
        Pause,
        PartyFinished
    };

    class GameManager
    {
    private:
        GameManager();
        static GameManager *m_instance;
        GameState m_gameState;

        UIMenu      m_mainScreen;
        UILoading   m_loadingScreen;
        UIPause     m_pauseScreen;
        UIVictory     m_victoryScreen;

    public:
        GameManager(const GameManager &) = delete;
        GameManager &operator=(const GameManager&) = delete;

        static GameManager *getInstance()
        {
            if(m_instance == nullptr)
                m_instance = new GameManager();
            return m_instance;
        }

        void GameManager::initialize();
        void GameManager::setGameState(const GameState &state);
        const GameState &getGameState() const;
        void pause();
        void update();
    };
}

#endif // FILES_MANAGER_H