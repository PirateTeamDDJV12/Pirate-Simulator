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
    // Move the camera to the target position
    m_gameObject->m_transform.setPosition(m_target->m_transform.getPosition());

    // Translate the camera back by the offset
    XMVECTOR dir = -m_gameObject->m_transform.getForward() * m_offset;
    m_gameObject->translate(dir);
}

void ObjectCameraBehaviour::rotate(Move::Rotation::Direction direction)
{
    using namespace std::chrono;

    const MouseState& mouseState = InputManager::singleton.getManipulator().EtatSouris();
    //time_point<system_clock> nowTime = std::chrono::system_clock::now();
    auto elapsedTime = TimeManager::GetInstance().getElapsedTimeFrame();

    m_rotationAroundX += mouseState.m_offsetMouseY * mouseState.m_sensibility;
    m_rotationAroundY += mouseState.m_offsetMouseX * mouseState.m_sensibility;
    // Prevent camera from flipping
    // You can change the values to block the camera before 90
    if (m_rotationAroundX < m_minAngleX)
        m_rotationAroundX = m_minAngleX;
    else if (m_rotationAroundX > m_maxAngleX)
        m_rotationAroundX = m_maxAngleX;

    float angleY;
    float angleX;

    // Angles in Radians
    angleY = DirectX::XMConvertToRadians(m_rotationAroundY);
    angleX = DirectX::XMConvertToRadians(m_rotationAroundX);



    float cosX = cosf(angleX);
    float cosY = cosf(angleY);
    float sinX = sinf(angleX);
    float sinY = sinf(angleY);

    // Look in the new direction
    m_gameObject->m_transform.setForward(XMVECTOR{ sinY * cosX, sinX, cosX * cosY });
}

void ObjectCameraBehaviour::anime(float ellapsedTime)
{
    // Pour les mouvements, nous utilisons le gestionnaire de saisie
    CDIManipulateur& rGestionnaireDeSaisie = InputManager::singleton.getManipulator();

    rotate(Move::Rotation::Y_CLOCKWISE);
    move(Move::Translation::FORWARD);

    // Update the view matrix
    m_cameraComponent->updateViewMatrix();
}
