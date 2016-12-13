#ifndef _UIMAINMENU_H_
#define _UIMAINMENU_H_

#include <memory>
#include <map>

#include "UIElement.h"

namespace PirateSimulator
{
    enum GameState
    {
        MainMenu,
        Loading,
        Options,
        Credits,
        Quitting,
        Starting
    };


    class UIMainMenuLogic
    {
    private:
        UIRef m_mainScreen;
        UIRef m_optionsScreen;
        UIRef m_creditsScreen;
        UIRef m_loadingScreen;
        GameState m_gameState;
        std::map<GameState, UIRef> m_ptrTab;

    public:
        UIMainMenuLogic();
        
        void setGameState(GameState state)
        {
            m_gameState = state;
        }

        bool operator()();
    };
}

#endif // !_UIMAINMENU_H_


