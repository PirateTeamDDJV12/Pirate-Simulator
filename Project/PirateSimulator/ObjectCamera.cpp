#include "stdafx.h"

#include "ObjectCamera.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"

using namespace PirateSimulator;
using namespace cameraModule;
using namespace DirectX;

void ObjectCamera::setTarget(GameObject *target) noexcept
{
    m_target = target;

    //position(target->m_position - m_transform.m_forward * m_offset);

    m_transform.m_position = m_target->getTransform().m_position - m_transform.m_forward * m_offset;

    setMatrixView(XMMatrixLookAtLH(m_transform.m_position,
        m_target->getTransform().m_position,
        m_transform.m_up));
}

void ObjectCamera::move(Move::Translation::Direction direction)
{

    m_target->translate(0.1f, 0.f, 0.f);

    //m_transform.m_position = m_target->getTransform().m_position - m_target->getTransform().m_forward * m_offset;

    setMatrixView(XMMatrixLookAtLH(m_transform.m_position,
        m_target->getTransform().m_position,
        m_transform.m_up));
}

void ObjectCamera::rotate(Move::Rotation::Direction direction)
{

}

void ObjectCamera::listenInput()
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
