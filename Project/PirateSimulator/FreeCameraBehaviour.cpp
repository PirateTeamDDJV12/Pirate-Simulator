#include <iostream>
#include <sstream>

#include "FreeCameraBehaviour.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"
#include "InputManager.h"

using namespace  PirateSimulator;
using namespace  cameraModule;
using namespace DirectX;


void FreeCameraBehaviour::move(Move::Translation::Direction direction)
{
    switch (direction)
    {
    case Move::Translation::FORWARD:
        m_gameObject->m_transform.translate(m_gameObject->m_transform.getForward() * m_cameraComponent->getTranslationVelocity());
        break;

    case Move::Translation::BACKWARD:
        m_gameObject->m_transform.translate(-m_gameObject->m_transform.getForward() * m_cameraComponent->getTranslationVelocity());
        break;

    case Move::Translation::LEFT:
        m_gameObject->m_transform.translate(-m_gameObject->m_transform.getRight() * m_cameraComponent->getTranslationVelocity());
        break;

    case Move::Translation::RIGHT:
        m_gameObject->m_transform.translate(m_gameObject->m_transform.getRight() * m_cameraComponent->getTranslationVelocity());
        break;

    case Move::Translation::UP:
        m_gameObject->m_transform.translate(m_gameObject->m_transform.getUp() * m_cameraComponent->getTranslationVelocity());
        break;

    case Move::Translation::DOWN:
        m_gameObject->m_transform.translate(-m_gameObject->m_transform.getUp() * m_cameraComponent->getTranslationVelocity());
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

    float cosX = cosf(m_rotationAroundX.getAngle());
    float cosY = cosf(m_rotationAroundY.getAngle());
    float sinX = sinf(m_rotationAroundX.getAngle());
    float sinY = sinf(m_rotationAroundY.getAngle());

    m_gameObject->m_transform.setForward(XMVECTOR{ sinY * cosX, sinX, cosX * cosY });

    m_cameraComponent->updateViewMatrix();
}

void FreeCameraBehaviour::anime(float ellapsedTime)
{
    // Pour les mouvements, nous utilisons le gestionnaire de saisie
    CDIManipulateur& rGestionnaireDeSaisie = InputManager::singleton.getManipulator();
    

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

    if(rGestionnaireDeSaisie.getKey(DIK_SPACE))
    {
        move(Move::Translation::UP);
    }

    if(rGestionnaireDeSaisie.getKey(DIK_F))
    {
        move(Move::Translation::DOWN);
    }


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

    if(rGestionnaireDeSaisie.getKey(DIK_CAPSLOCK))
    {
        m_cameraComponent->changeVelocity();
    }
}