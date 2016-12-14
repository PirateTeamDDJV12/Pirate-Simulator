#ifndef UIPAUSELOGIC_H_INCLUDED
#define UIPAUSELOGIC_H_INCLUDED

#include "UIElement.h"
#include "UIButton.h"
#include "UIMainMenuLogic.h"


namespace PirateSimulator
{
    class UIPauseLogic
    {
    public:
        enum PauseState
        {
            Paused,
            Resuming,
            Leaving
        };

    private:
        UIRef m_pauseScreen;
        PauseState m_gameState;

    public:
        UIPauseLogic();

    public:
        void setPauseState(PauseState state);
        PauseState getPauseState() const noexcept
        {
            return m_gameState;
        }

    public:
        bool operator()();
    };
} 

#endif // !UIPAUSELOGIC_H_INCLUDED
