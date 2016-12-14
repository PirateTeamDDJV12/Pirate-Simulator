#include "UIPauseLogic.h"
#include "UIPause.h"


using namespace PirateSimulator;


UIPauseLogic::UIPauseLogic()
{
    m_pauseScreen = UIRef(new UIPause(this));
    m_gameState = PauseState::Resuming;
}

void UIPauseLogic::setPauseState(UIPauseLogic::PauseState state)
{
    m_gameState = state;
}

bool UIPauseLogic::operator()()
{
    return m_pauseScreen->update();
}