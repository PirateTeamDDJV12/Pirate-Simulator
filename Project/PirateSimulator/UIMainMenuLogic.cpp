#include "UIMainMenuLogic.h"

#include "UIMenu.h"
#include "UILoading.h"

using namespace PirateSimulator;

UIMainMenuLogic::UIMainMenuLogic() : m_optionsScreen{nullptr}, m_creditsScreen{nullptr}, m_loadingScreen{new UILoading()}
{
    m_mainScreen = UIRef(new UIMenu(this));
    m_ptrTab[GameState::MainMenu] = m_mainScreen;
    m_ptrTab[GameState::Options] = m_optionsScreen;
    m_ptrTab[GameState::Credits] = m_creditsScreen;
    m_ptrTab[GameState::Loading] = m_loadingScreen;

    m_gameState = GameState::MainMenu;
}

bool UIMainMenuLogic::operator()()
{
    // todo
    if(m_gameState == GameState::Starting || m_gameState == GameState::Quitting)
        return true;
    for(std::map<GameState, UIRef>::const_iterator it = m_ptrTab.begin(); it != m_ptrTab.end(); ++it)
    {
        if(it->first == m_gameState)
            return it->second->update();
    }
    return false;
}
