#ifndef UIMENU_H_INCLUDED
#define UIMENU_H_INCLUDED

#include "UIElement.h"

#include <memory>
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
        PM3D::CAfficheurSprite* pAfficheurSprite;
    };
}

#endif // !UIMENU_H_INCLUDED


