#include "stdafx.h"

#include "ObjectCameraBehaviour.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"

using namespace PirateSimulator;
using namespace cameraModule;
using namespace DirectX;


void ObjectCameraBehaviour::move(Move::Translation::Direction direction)
{

    m_cameraComponent->getTarget()->translate(0.1f, 0.f, 0.f);

    //m_transform.m_position = m_target->getTransform().m_position - m_target->getTransform().m_forward * m_offset;

    m_cameraComponent->updateViewMatrix();
}

void ObjectCameraBehaviour::rotate(Move::Rotation::Direction direction)
{

}

void ObjectCameraBehaviour::anime(float ellapsedTime)
{
    // Pour les mouvements, nous utilisons le gestionnaire de saisie
    PM3D::CMoteurWindows& rMoteur = PM3D::CMoteurWindows::GetInstance();
    CDIManipulateur& rGestionnaireDeSaisie = rMoteur.GetGestionnaireDeSaisie();

    /*
    * Translation
    */
    if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_A))
    {
        move(Move::Translation::LEFT);
    }
}
