#include "StdAfx.h"

#include <iostream>
#include <sstream>

#include "FreeCameraBehaviour.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"

using namespace  PirateSimulator;
using namespace  cameraModule;
using namespace DirectX;


void FreeCameraBehaviour::move(Move::Translation::Direction direction)
{
    switch (direction)
    {
    case Move::Translation::FORWARD:
        m_gameObject->m_transform.m_position += m_gameObject->m_transform.m_forward * m_cameraComponent->getTranslationVelocity();
        break;

    case Move::Translation::BACKWARD:
        m_gameObject->m_transform.m_position -= m_gameObject->m_transform.m_forward * m_cameraComponent->getTranslationVelocity();
        break;

    case Move::Translation::LEFT:
        m_gameObject->m_transform.m_position -= m_gameObject->m_transform.m_right * m_cameraComponent->getTranslationVelocity();
        break;

    case Move::Translation::RIGHT:
        m_gameObject->m_transform.m_position += m_gameObject->m_transform.m_right * m_cameraComponent->getTranslationVelocity();
        break;

    case Move::Translation::UP:
        m_gameObject->m_transform.m_position += m_gameObject->m_transform.m_up * m_cameraComponent->getTranslationVelocity();
        break;

    case Move::Translation::DOWN:
        m_gameObject->m_transform.m_position -= m_gameObject->m_transform.m_up * m_cameraComponent->getTranslationVelocity();
        break;

    case Move::Translation::NONE:
    default:
        return;
    }

    m_cameraComponent->updateViewMatrix();
}

void FreeCameraBehaviour::rotate(Move::Rotation::Direction direction)
{
    using namespace std::chrono;

    time_point<system_clock> nowTime = std::chrono::system_clock::now();

    float elapsedTime = duration_cast<milliseconds>(nowTime - m_lastTime).count() / 1000.f;

    switch (direction)
    {
    case Move::Rotation::X_CLOCKWISE:
        m_rotationAroundX -= m_cameraComponent->getRotationVelocity();
        break;

    case Move::Rotation::X_INVERT_CLOCKWISE:
        m_rotationAroundX += m_cameraComponent->getRotationVelocity();
        break;

    case Move::Rotation::Y_CLOCKWISE:
        m_rotationAroundY -= m_cameraComponent->getRotationVelocity();
        break;

    case Move::Rotation::Y_INVERT_CLOCKWISE:
        m_rotationAroundY += m_cameraComponent->getRotationVelocity();
        break;

    case Move::Rotation::Z_CLOCKWISE:
    case Move::Rotation::Z_INVERT_CLOCKWISE:
    case Move::Rotation::NONE:
    default:
        return;
    }
    if (DirectX::XMConvertToDegrees(m_rotationAroundX.getAngle()) < -89.0f)
        m_rotationAroundX = DirectX::XMConvertToRadians(-89.0f);

    else if (DirectX::XMConvertToDegrees(m_rotationAroundX.getAngle()) > 89.0f)
        m_rotationAroundX = DirectX::XMConvertToRadians(89.0f);

    m_gameObject->m_transform.m_forward.vector4_f32[0] = sin(m_rotationAroundY.getAngle()) * cos(m_rotationAroundX.getAngle());
    m_gameObject->m_transform.m_forward.vector4_f32[1] = sin(m_rotationAroundX.getAngle());
    m_gameObject->m_transform.m_forward.vector4_f32[2] = cos(m_rotationAroundX.getAngle()) * cos(m_rotationAroundY.getAngle());

    // Update the rightDirection vector when rotating
    m_gameObject->m_transform.m_right = 
        DirectX::XMVector3Normalize(
            DirectX::XMVector3Cross(
                m_gameObject->m_transform.m_up,
                m_gameObject->m_transform.m_forward
            )
    );

    m_cameraComponent->updateViewMatrix();
}

void FreeCameraBehaviour::anime(float ellapsedTime)
{
    // Pour les mouvements, nous utilisons le gestionnaire de saisie
    PM3D::CMoteurWindows& rMoteur = PM3D::CMoteurWindows::GetInstance();
    CDIManipulateur& rGestionnaireDeSaisie = rMoteur.GetGestionnaireDeSaisie();
    

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_A))
    {
        move(Move::Translation::LEFT);
    }

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_D))
    {
        move(Move::Translation::RIGHT);
    }

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_W))
    {
        move(Move::Translation::FORWARD);
    }

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_S))
    {
        move(Move::Translation::BACKWARD);
    }

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_SPACE))
    {
        move(Move::Translation::UP);
    }

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_F))
    {
        move(Move::Translation::DOWN);
    }


    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_LEFT))
    {
        rotate(Move::Rotation::Y_CLOCKWISE);
    }

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_RIGHT))
    {
        rotate(Move::Rotation::Y_INVERT_CLOCKWISE);
    }

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_UP))
    {
        rotate(Move::Rotation::X_INVERT_CLOCKWISE);
    }

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_DOWN))
    {
        rotate(Move::Rotation::X_CLOCKWISE);
    }

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_CAPSLOCK))
    {
        m_cameraComponent->changeVelocity();
    }
}