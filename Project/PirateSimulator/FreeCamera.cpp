#include "stdafx.h"

#include <iostream>
#include <sstream>

#include "FreeCamera.h"
#include "../Chapitre 10/PetitMoteur3D/MoteurWindows.h"

using namespace  PirateSimulator;
using namespace  cameraModule;


void FreeCamera::move(Move::Translation::Direction direction)
{
    switch (direction)
    {
    case Move::Translation::FORWARD:
        m_transform.m_position += m_transform.m_forward * m_moveParams.translationVelocity;
        break;

    case Move::Translation::BACKWARD:
        m_transform.m_position -= m_transform.m_forward * m_moveParams.translationVelocity;
        break;

    case Move::Translation::LEFT:
        m_transform.m_position -= m_transform.m_right * m_moveParams.translationVelocity;
        break;

    case Move::Translation::RIGHT:
        m_transform.m_position += m_transform.m_right * m_moveParams.translationVelocity;
        break;

    case Move::Translation::UP:
        m_transform.m_position += m_transform.m_up * m_moveParams.translationVelocity;
        break;

    case Move::Translation::DOWN:
        m_transform.m_position -= m_transform.m_up * m_moveParams.translationVelocity;
        break;

    case Move::Translation::NONE:
    default:
        return;
    }
    setMatrixView(XMMatrixLookToLH(m_transform.m_position, m_transform.m_forward, m_transform.m_up));
}

void FreeCamera::rotate(Move::Rotation::Direction direction)
{
    using namespace std::chrono;

    time_point<system_clock> nowTime = std::chrono::system_clock::now();

    float elapsedTime = duration_cast<milliseconds>(nowTime - m_lastTime).count() / 1000.f;

    switch (direction)
    {
    case Move::Rotation::X_CLOCKWISE:
        m_rotationAroundX -= m_moveParams.rotationVelocity;
        break;

    case Move::Rotation::X_INVERT_CLOCKWISE:
        m_rotationAroundX += m_moveParams.rotationVelocity;
        break;

    case Move::Rotation::Y_CLOCKWISE:
        m_rotationAroundY -= m_moveParams.rotationVelocity;
        break;

    case Move::Rotation::Y_INVERT_CLOCKWISE:
        m_rotationAroundY += m_moveParams.rotationVelocity;
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

    m_transform.m_forward.vector4_f32[0] = sin(m_rotationAroundY.getAngle()) * cos(m_rotationAroundX.getAngle());
    m_transform.m_forward.vector4_f32[1] = sin(m_rotationAroundX.getAngle());
    m_transform.m_forward.vector4_f32[2] = cos(m_rotationAroundX.getAngle()) * cos(m_rotationAroundY.getAngle());

    // Update the rightDirection vector when rotating
    m_transform.m_right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(m_transform.m_up, m_transform.m_forward));


#ifdef MODIFY_UP_VECTOR_AT_ROTATE
    //y'' = (x * 0) + (y * cos(b)) + (- z * sin(b))
    m_up.vector4_f32[0] = 0.f;
    m_up.vector4_f32[1] = cosAngleAroundX;
    m_up.vector4_f32[2] = -sinAngleAroundX;
#endif //MODIFY_UP_VECTOR_AT_ROTATE

    setMatrixView(XMMatrixLookToLH(m_transform.m_position, m_transform.m_forward, m_transform.m_up));
}

void FreeCamera::listenInput()
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
        changeVelocity();
    }
}