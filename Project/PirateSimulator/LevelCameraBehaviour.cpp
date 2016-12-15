#include "LevelCameraBehaviour.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"
#include "InputManager.h"


using namespace PirateSimulator;
using namespace cameraModule;
using namespace DirectX;

void LevelCameraBehaviour::move(Move::Translation::Direction direction)
{
    switch(direction)
    {
        case Move::Translation::FORWARD:
            m_gameObject->m_transform.translate(m_gameObject->m_transform.getForward());
            break;

        case Move::Translation::BACKWARD:
            m_gameObject->m_transform.translate(-m_gameObject->m_transform.getForward());
            break;

        case Move::Translation::LEFT:
            m_gameObject->m_transform.translate(-m_gameObject->m_transform.getRight());
            break;

        case Move::Translation::RIGHT:
            m_gameObject->m_transform.translate(m_gameObject->m_transform.getRight());
            break;

        case Move::Translation::UP:
            m_gameObject->m_transform.translate(m_gameObject->m_transform.getUp());
            break;

        case Move::Translation::DOWN:
            m_gameObject->m_transform.translate(-m_gameObject->m_transform.getUp());
            break;

        case Move::Translation::NONE:
        default:
            return;
    }

    auto temp = m_gameObject->m_transform.getPosition();
    temp.vector4_f32[1] = m_terrain->getHeight(m_gameObject->m_transform.getPosition()) + m_offsetCam;

    m_gameObject->m_transform.setPosition(XMVectorLerp(m_gameObject->m_transform.getPosition(), temp, .1f));

    m_cameraComponent->updateViewMatrix();
}

void LevelCameraBehaviour::rotate(Move::Rotation::Direction direction)
{
    switch(direction)
    {
        case Move::Rotation::X_CLOCKWISE:
            m_rotationAroundX -= m_speed;
            break;

        case Move::Rotation::X_INVERT_CLOCKWISE:
            m_rotationAroundX += m_speed;
            break;

        case Move::Rotation::Y_CLOCKWISE:
            m_rotationAroundY -= m_speed;
            break;

        case Move::Rotation::Y_INVERT_CLOCKWISE:
            m_rotationAroundY += m_speed;
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

    float cosX = cosf(m_rotationAroundX.getAngle());
    float cosY = cosf(m_rotationAroundY.getAngle());
    float sinX = sinf(m_rotationAroundX.getAngle());
    float sinY = sinf(m_rotationAroundY.getAngle());

    m_gameObject->m_transform.setForward(XMVECTOR{ sinY * cosX, sinX, cosX * cosY });

#ifdef MODIFY_UP_VECTOR_AT_ROTATE
    //y'' = (x * 0) + (y * cos(b)) + (- z * sin(b))
    m_up.vector4_f32[0] = 0.f;
    m_up.vector4_f32[1] = cosAngleAroundX;
    m_up.vector4_f32[2] = -sinAngleAroundX;
#endif //MODIFY_UP_VECTOR_AT_ROTATE

    m_cameraComponent->updateViewMatrix();
}

void LevelCameraBehaviour::anime(float elapsedTime)
{
    // Pour les mouvements, nous utilisons le gestionnaire de saisie
    CDIManipulateur& rGestionnaireDeSaisie = InputManager::singleton.getManipulator();

    /*
     * Translation
     */
    if(rGestionnaireDeSaisie.getKey(DIK_A))
    {
        move(Move::Translation::LEFT);
    }

    if(rGestionnaireDeSaisie.getKey(DIK_D))
    {
        move(Move::Translation::RIGHT);
    }

    if(rGestionnaireDeSaisie.getKey(DIK_W))
    {
        move(Move::Translation::FORWARD);
    }

    if(rGestionnaireDeSaisie.getKey(DIK_S))
    {
        move(Move::Translation::BACKWARD);
    }

    /*
     * Rotation
     */
    if(rGestionnaireDeSaisie.getKey(DIK_LEFT))
    {
        rotate(Move::Rotation::Y_CLOCKWISE);
    }

    if(rGestionnaireDeSaisie.getKey(DIK_RIGHT))
    {
        rotate(Move::Rotation::Y_INVERT_CLOCKWISE);
    }

    if(rGestionnaireDeSaisie.getKey(DIK_UP))
    {
        rotate(Move::Rotation::X_INVERT_CLOCKWISE);
    }

    if(rGestionnaireDeSaisie.getKey(DIK_DOWN))
    {
        rotate(Move::Rotation::X_CLOCKWISE);
    }
}
