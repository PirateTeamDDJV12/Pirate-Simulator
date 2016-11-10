#include "StdAfx.h"

#include "LevelCameraBehaviour.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"


using namespace PirateSimulator;
using namespace cameraModule;
using namespace DirectX;

void LevelCameraBehaviour::move(Move::Translation::Direction direction)
{
    switch(direction)
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

    auto temp = m_gameObject->m_transform.m_position;
    temp.vector4_f32[1] = m_terrain->getHeight(m_gameObject->m_transform.m_position) + m_offsetCam;

    m_gameObject->m_transform.m_position = XMVectorLerp(m_gameObject->m_transform.m_position, temp, .1f);

    m_cameraComponent->updateViewMatrix();
}

void LevelCameraBehaviour::rotate(Move::Rotation::Direction direction)
{
    switch(direction)
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
    if(XMConvertToDegrees(m_rotationAroundX.getAngle()) < -89.0f)
        m_rotationAroundX = XMConvertToRadians(-89.0f);

    else if(XMConvertToDegrees(m_rotationAroundX.getAngle()) > 89.0f)
        m_rotationAroundX = XMConvertToRadians(89.0f);

    m_gameObject->m_transform.m_forward.vector4_f32[0] = sin(m_rotationAroundY.getAngle()) * cos(m_rotationAroundX.getAngle());
    m_gameObject->m_transform.m_forward.vector4_f32[1] = sin(m_rotationAroundX.getAngle());
    m_gameObject->m_transform.m_forward.vector4_f32[2] = cos(m_rotationAroundX.getAngle()) * cos(m_rotationAroundY.getAngle());

    // Update the rightDirection vector when rotating
    m_gameObject->m_transform.m_right = XMVector3Normalize(XMVector3Cross(m_gameObject->m_transform.m_up, m_gameObject->m_transform.m_forward));


#ifdef MODIFY_UP_VECTOR_AT_ROTATE
    //y'' = (x * 0) + (y * cos(b)) + (- z * sin(b))
    m_up.vector4_f32[0] = 0.f;
    m_up.vector4_f32[1] = cosAngleAroundX;
    m_up.vector4_f32[2] = -sinAngleAroundX;
#endif //MODIFY_UP_VECTOR_AT_ROTATE

    m_cameraComponent->updateViewMatrix();
}

void LevelCameraBehaviour::anime(float ellapsedTime)
{
    // Pour les mouvements, nous utilisons le gestionnaire de saisie
    PM3D::CMoteurWindows& rMoteur = PM3D::CMoteurWindows::GetInstance();
    CDIManipulateur& rGestionnaireDeSaisie = rMoteur.GetGestionnaireDeSaisie();

    /*
     * Translation
     */
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

    /*
     * Rotation
     */
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
