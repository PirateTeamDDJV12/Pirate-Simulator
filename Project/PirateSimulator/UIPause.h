#ifndef UIPAUSE_H_INCLUDED
#define UIPAUSE_H_INCLUDED

#include "UIElement.h"
#include "UIButton.h"


namespace PirateSimulator
{
    class UIPauseLogic;

    class UIPause : public UIElementBase
    {
    private:
        UIPauseLogic* m_parent;
        PM3D::CAfficheurSprite* m_mainMenuImages;
        IUIButton* m_buttons[2];
        unsigned int m_currentButton;

    public:
        UIPause(UIPauseLogic* parent);

    public:
        virtual void initialize();
        virtual bool update();
        virtual bool select();
        virtual bool cancel();
    };
}

#endif // !UIPAUSE_H_INCLUDED