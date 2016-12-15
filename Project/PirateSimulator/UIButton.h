#ifndef _UIBUTTON_H_
#define _UIBUTTON_H_

#include <vector>
#include "..\PetitMoteur3D\PetitMoteur3D\AfficheurSprite.h"

namespace PirateSimulator
{
    class IUIButton
    {
    public:
        virtual void onSelect() = 0;
        virtual void onUnselect() = 0;
        virtual void onPressed() = 0;
        virtual void setDefaultSprites(PM3D::CAfficheurSprite *spriteDrawer) = 0;
        virtual void setSelectedSpriteDrawer(PM3D::CAfficheurSprite *spriteDrawer) = 0;
        virtual void draw() = 0;
    };

    template<class T>
    class UIButton : public IUIButton
    {
    public:
        UIButton(T &action) : m_sprites{nullptr}, m_selectedSprites{nullptr}, m_selected{false}, m_action{action}
        {

        }
        virtual ~UIButton() = default;

        void onSelect()
        {
            m_selected = true;
        }

        void onUnselect()
        {
            m_selected = false;
        }

        void onPressed()
        {
            m_action();
        }

        void setDefaultSprites(PM3D::CAfficheurSprite *spriteDrawer)
        {
            m_sprites = spriteDrawer;
        }

        void setSelectedSpriteDrawer(PM3D::CAfficheurSprite *spriteDrawer)
        {
            m_selectedSprites = spriteDrawer;
        }

        void draw()
        {
            m_sprites->Draw();
            if(m_selected)
                m_selectedSprites->Draw();
        }

    private:
        PM3D::CAfficheurSprite * m_sprites;
        PM3D::CAfficheurSprite * m_selectedSprites;
        bool m_selected;
        T   m_action;
    };

    template<class T>
    IUIButton* createButton(T action)
    {
        return new UIButton<T>(action);
    }
}

#endif // !_UIBUTTON_H_


