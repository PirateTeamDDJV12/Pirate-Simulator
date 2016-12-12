#ifndef UIMENU_H_INCLUDED
#define UIMENU_H_INCLUDED

#include <memory>

#include "UIElement.h"
#include "UIButton.h"
#include "..\PetitMoteur3D\PetitMoteur3D\AfficheurSprite.h"

namespace PirateSimulator 
{
    class UIMenu : public UIElementBase
    {
    public:
        UIMenu();

    public:
        virtual void initialize();
        virtual bool update();
        virtual bool select();
        virtual bool cancel();

    private:
        PM3D::CAfficheurSprite  *m_mainMenuImages;
        UIButton                *m_buttons[4];
        unsigned int            m_currentButton;
    };
}

#endif // !UIMENU_H_INCLUDED


