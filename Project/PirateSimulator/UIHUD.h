#ifndef UIHUD_H_INCLUDED
#define UIHUD_H_INCLUDED

#include "GameObject.h"

namespace PM3D
{
    class CAfficheurSprite;
    class CAfficheurTexte;
}

namespace Gdiplus
{
    class Font;
}

namespace PirateSimulator
{
    class UIHUD : public GameObject
    {
    public:
        UIHUD();

        void anime(float elapsedTime) override;
    private:
        PM3D::CAfficheurSprite* m_afficheurSprite;
        PM3D::CAfficheurTexte* m_speed;
        PM3D::CAfficheurTexte* m_score;
        PM3D::CAfficheurTexte* m_time;
        Gdiplus::Font* m_police;
    };
}

#endif // !UIHUD_H_INCLUDED