#ifndef UIMENU_H_INCLUDED
#define UIMENU_H_INCLUDED

#include <memory>

#include "UIElement.h"
#include "UIMainMenuLogic.h"
#include "UIButton.h"
#include "..\PetitMoteur3D\PetitMoteur3D\AfficheurSprite.h"

namespace PirateSimulator 
{
    class UIMainMenuLogic;

    class UIMenu : public UIElementBase
    {
    public:
        UIMenu(UIMainMenuLogic *parent);

    public:
        virtual void initialize();
        virtual bool update();
        virtual bool select();
        virtual bool cancel();

    private:
        UIMainMenuLogic *m_parent;
        PM3D::CAfficheurSprite  *m_mainMenuImages;
        IUIButton                *m_buttons[4];
        unsigned int            m_currentButton;
    };
}

#endif // !UIMENU_H_INCLUDED


