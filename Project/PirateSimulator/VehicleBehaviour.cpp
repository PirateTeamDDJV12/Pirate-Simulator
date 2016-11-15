#include "VehicleBehaviour.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"

using namespace PirateSimulator;
using namespace DirectX;

void VehicleBehaviour::anime(float ellapsedTime)
{
    PM3D::CMoteurWindows& rMoteur = PM3D::CMoteurWindows::GetInstance();
    CDIManipulateur& rGestionnaireDeSaisie = rMoteur.GetGestionnaireDeSaisie();

    // ******** POUR LA SOURIS ************

    // Vérifier si déplacement vers la gauche
    if ((rGestionnaireDeSaisie.EtatSouris().rgbButtons[0] & 0x80) &&
        (rGestionnaireDeSaisie.EtatSouris().lX < 0))
    {
        rotation = rotation + ((XM_PI * 2.0f) / 2.0f * ellapsedTime);


        // modifier la matrice de l'objet X
        m_gameObject->setWorldMatrix(XMMatrixRotationY(rotation) * DirectX::XMMatrixTranslationFromVector(m_gameObject->m_transform.m_position));
    }

    // Vérifier si déplacement vers la droite
    if ((rGestionnaireDeSaisie.EtatSouris().rgbButtons[0] & 0x80) &&
        (rGestionnaireDeSaisie.EtatSouris().lX > 0))
    {
        rotation = rotation - ((XM_PI * 2.0f) / 2.0f * ellapsedTime);


        // modifier la matrice de l'objet X
        m_gameObject->setWorldMatrix(XMMatrixRotationY(rotation) * DirectX::XMMatrixTranslationFromVector(m_gameObject->m_transform.m_position));
    }

    // Vérifier si déplacement vers la droite
    if ((rGestionnaireDeSaisie.ToucheAppuyee(DIK_T)))
    {
        Transform intermediary = m_gameObject->m_transform;
        intermediary.m_position += m_gameObject->m_transform.m_forward * velocity;
        m_gameObject->m_transform = intermediary;

        // modifier la matrice de l'objet X
        m_gameObject->setWorldMatrix(XMMatrixRotationY(rotation) * DirectX::XMMatrixTranslationFromVector(m_gameObject->m_transform.m_position));
    }
}
