#ifndef UIHUD_H_INCLUDED
#define UIHUD_H_INCLUDED

#include "../PetitMoteur3D/PetitMoteur3D/AfficheurSprite.h"
#include "../PetitMoteur3D/PetitMoteur3D/AfficheurTexte.h"
#include <Gdiplus.h>
#include "GameObject.h"
#pragma comment(lib, "gdiplus.lib")

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