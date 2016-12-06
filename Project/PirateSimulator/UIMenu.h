#ifndef UIMENU_H_INCLUDED
#define UIMENU_H_INCLUDED

#include "UIElement.h"

#include <memory>

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
    };
}

#endif // !UIMENU_H_INCLUDED


