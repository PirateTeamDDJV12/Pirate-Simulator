#include "stdafx.h"

#include "TestBehaviour.h"

#include "../Chapitre 10/PetitMoteur3D/MoteurWindows.h"

using namespace PirateSimulator;
using namespace DirectX;

void TestBehaviour::anime(float ellapsedTime)
{
    PM3D::CMoteurWindows& rMoteur = PM3D::CMoteurWindows::GetInstance();
    CDIManipulateur& rGestionnaireDeSaisie = rMoteur.GetGestionnaireDeSaisie();

    // ******** POUR LA SOURIS ************
    // V�rifier si d�placement vers la gauche
    if ((rGestionnaireDeSaisie.EtatSouris().rgbButtons[0] & 0x80) &&
        (rGestionnaireDeSaisie.EtatSouris().lX < 0))
    {
        rotation = rotation + ((XM_PI * 2.0f) / 2.0f * ellapsedTime);


        // modifier la matrice de l'objet X
        m_gameObject->setWorldMatrix(XMMatrixRotationY(rotation) * DirectX::XMMatrixTranslationFromVector(m_gameObject->getTransform().m_position));
    }

    // V�rifier si d�placement vers la droite
    if ((rGestionnaireDeSaisie.EtatSouris().rgbButtons[0] & 0x80) &&
        (rGestionnaireDeSaisie.EtatSouris().lX > 0))
    {
        rotation = rotation - ((XM_PI * 2.0f) / 2.0f * ellapsedTime);


        // modifier la matrice de l'objet X
        m_gameObject->setWorldMatrix(XMMatrixRotationY(rotation) * DirectX::XMMatrixTranslationFromVector(m_gameObject->getTransform().m_position));
    }
}