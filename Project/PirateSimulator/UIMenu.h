#ifndef UIMENU_H_INCLUDED
#define UIMENU_H_INCLUDED

#include "UIElement.h"


namespace PM3D
{
    class CAfficheurSprite;
}

namespace PirateSimulator 
{
    class IUIButton;

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
        IUIButton                *m_buttons[2];
        unsigned int            m_currentButton;
    };
}

#endif // !UIMENU_H_INCLUDED


