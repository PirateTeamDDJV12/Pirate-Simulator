#include <DirectXMath.h>

#include "ObjectCameraBehaviour.h"
#include "PlayerBehaviour.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"
#include "InputManager.h"

using namespace PirateSimulator;
using namespace cameraModule;
using namespace DirectX;


void ObjectCameraBehaviour::move(Move::Translation::Direction direction)
{
    if(m_state == CameraState::ThirdPersonCamera)
    {
        XMVECTOR targetPos = m_target->m_transform.getPosition();

        // Move the camera to the target position
        m_desiredPosition = XMVECTOR{targetPos.vector4_f32[0], 10.0f, targetPos.vector4_f32[2]};

        // Translate the camera back by the offset
        XMVECTOR dir = -m_gameObject->m_transform.getForward() * m_distanceFromTarget;

        m_desiredPosition = m_desiredPosition + dir;
        XMVECTOR currentPosition = m_gameObject->m_transform.getPosition();

        float newX = currentPosition.vector4_f32[0] + (m_desiredPosition.vector4_f32[0] - currentPosition.vector4_f32[0]) * m_translationSmooth;
        float newY = currentPosition.vector4_f32[1] + (m_desiredPosition.vector4_f32[1] - currentPosition.vector4_f32[1]) * m_translationSmooth;
        float newZ = currentPosition.vector4_f32[2] + (m_desiredPosition.vector4_f32[2] - currentPosition.vector4_f32[2]) * m_translationSmooth;

        m_gameObject->setPosition(newX, newY, newZ);
    }
    else
    {
        // Move the camera to the target position
        m_desiredPosition = m_target->m_transform.getPosition() + m_target->m_transform.getForward() * -4.0f + m_target->m_transform.getRight() * -1.0f + m_target->m_transform.getUp() * 8.0f;
        if(!m_firstPersonPositionOk)
        {
            XMVECTOR diff = m_desiredPosition - m_gameObject->m_transform.getPosition();
            if(fabs(diff.vector4_f32[0]) < 0.2f &&  fabs(diff.vector4_f32[1]) < 0.2f && fabs(diff.vector4_f32[2] < 0.2f))
            {
                m_firstPersonPositionOk = true;
                m_rotationSmooth = 0.5f;
                m_translationSmooth = 1.0f;
            }
            XMVECTOR currentPosition = m_gameObject->m_transform.getPosition();

            float newX = currentPosition.vector4_f32[0] + (m_desiredPosition.vector4_f32[0] - currentPosition.vector4_f32[0]) * m_translationSmooth;
            float newY = currentPosition.vector4_f32[1] + (m_desiredPosition.vector4_f32[1] - currentPosition.vector4_f32[1]) * m_translationSmooth;
            float newZ = currentPosition.vector4_f32[2] + (m_desiredPosition.vector4_f32[2] - currentPosition.vector4_f32[2]) * m_translationSmooth;

            m_gameObject->setPosition(newX, newY, newZ);
        }
        else
        m_gameObject->setPosition(m_desiredPosition);
    }
}

void ObjectCameraBehaviour::rotate(Move::Rotation::Direction direction)
{
    using namespace std::chrono;

    const MouseState& mouseState = InputManager::singleton.getManipulator().EtatSouris();
    //time_point<system_clock> nowTime = std::chrono::system_clock::now();
    auto elapsedTime = TimeManager::GetInstance().getElapsedTimeFrame();

    m_newAngleX -= mouseState.m_offsetMouseY * mouseState.m_sensibility;
    m_newAngleY += mouseState.m_offsetMouseX * mouseState.m_sensibility;

    // Prevent camera from flipping
    // You can change the values to block the camera before 90
    if(m_rotationAroundX < m_minAngleX)
        m_rotationAroundX = m_minAngleX;
    else if(m_rotationAroundX > m_maxAngleX)
        m_rotationAroundX = m_maxAngleX;

    float angleXFinal;
    float angleYFinal;

    angleXFinal = m_rotationAroundX + (m_newAngleX - m_rotationAroundX) * m_rotationSmooth;
    angleYFinal = m_rotationAroundY + (m_newAngleY - m_rotationAroundY) * m_rotationSmooth;

    m_rotationAroundX = angleXFinal;
    m_rotationAroundY = angleYFinal;
    // Angles in Radians
    angleYFinal = DirectX::XMConvertToRadians(m_rotationAroundY);
    angleXFinal = DirectX::XMConvertToRadians(m_rotationAroundX);

    float cosX = cosf(angleXFinal);
    float cosY = cosf(angleYFinal);
    float sinX = sinf(angleXFinal);
    float sinY = sinf(angleYFinal);

    // Look in the new direction
    m_gameObject->m_transform.setForward(XMVECTOR{sinY * cosX, sinX, cosX * cosY});
}

void ObjectCameraBehaviour::anime(float ellapsedTime)
{
    // Pour les mouvements, nous utilisons le gestionnaire de saisie
    CDIManipulateur& rGestionnaireDeSaisie = InputManager::singleton.getManipulator();

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_F))
    {
        if(m_state == CameraState::ThirdPersonCamera)
        {
            m_state = CameraState::FirstPersonCamera;
            m_maxAngleX = -85.0f;
            m_maxAngleX = 85.0f;
            m_rotationSmooth = 0.5f;
            m_translationSmooth = 0.5f;
        }
        else
        {
            m_state = CameraState::ThirdPersonCamera;
            m_maxAngleX = -85.0f;
            m_maxAngleX = -10.0f;
            m_rotationSmooth = 0.01f;
            m_translationSmooth = 0.05f;
            m_firstPersonPositionOk = false;
        }
    }

    rotate(Move::Rotation::Y_CLOCKWISE);
    move(Move::Translation::FORWARD);

    // Update the view matrix
    m_cameraComponent->updateViewMatrix();
}
