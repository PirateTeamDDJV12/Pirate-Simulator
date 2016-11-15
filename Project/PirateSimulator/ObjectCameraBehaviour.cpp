#include "ObjectCameraBehaviour.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"

using namespace PirateSimulator;
using namespace cameraModule;
using namespace DirectX;


void ObjectCameraBehaviour::move(Move::Translation::Direction direction)
{
    switch (direction)
    {
    case Move::Translation::FORWARD:
    {
        DirectX::XMVECTOR trMove = m_gameObject->m_transform.m_forward * m_cameraComponent->getTranslationVelocity();
        m_cameraComponent->getTarget()->translate(trMove.vector4_f32[0], trMove.vector4_f32[1], trMove.vector4_f32[2]);
    }
        break;

    case Move::Translation::BACKWARD:
    {
        DirectX::XMVECTOR trMove = m_gameObject->m_transform.m_forward * -m_cameraComponent->getTranslationVelocity();
        m_cameraComponent->getTarget()->translate(trMove.vector4_f32[0], trMove.vector4_f32[1], trMove.vector4_f32[2]);
    }
        break;

    case Move::Translation::LEFT:
    {
        DirectX::XMVECTOR trMove = m_gameObject->m_transform.m_right * -m_cameraComponent->getTranslationVelocity();
        m_cameraComponent->getTarget()->translate(trMove.vector4_f32[0], trMove.vector4_f32[1], trMove.vector4_f32[2]);
    }
        break;

    case Move::Translation::RIGHT:
    {
        DirectX::XMVECTOR trMove = m_gameObject->m_transform.m_right * m_cameraComponent->getTranslationVelocity();
        m_cameraComponent->getTarget()->translate(trMove.vector4_f32[0], trMove.vector4_f32[1], trMove.vector4_f32[2]);
    }
        break;

    case Move::Translation::UP:
    case Move::Translation::DOWN:
    case Move::Translation::NONE:
    default:
        return;
    }

    m_cameraComponent->updateViewMatrix();

    
    //m_cameraComponent->getTarget()->translate(0.1f, 0.f, 0.f);
    //m_transform.m_position = m_target->getTransform().m_position - m_target->getTransform().m_forward * m_offset;

    //m_cameraComponent->updateViewMatrix();
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

    if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_D))
    {
        move(Move::Translation::RIGHT);
    }

    if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_W))
    {
        move(Move::Translation::FORWARD);
    }

    if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_S))
    {
        move(Move::Translation::BACKWARD);
    }

    /*
    * Rotation
    */
    if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_LEFT))
    {
        rotate(Move::Rotation::Y_CLOCKWISE);
    }

    if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_RIGHT))
    {
        rotate(Move::Rotation::Y_INVERT_CLOCKWISE);
    }

    if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_UP))
    {
        rotate(Move::Rotation::X_INVERT_CLOCKWISE);
    }

    if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_DOWN))
    {
        rotate(Move::Rotation::X_CLOCKWISE);
    }

    if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_CAPSLOCK))
    {
        m_cameraComponent->changeVelocity();
    }
}
