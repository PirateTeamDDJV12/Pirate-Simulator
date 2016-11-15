#include "ObjectCameraBehaviour.h"
#include "PlayerBehaviour.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"
#include <DirectXMath.h>

using namespace PirateSimulator;
using namespace cameraModule;
using namespace DirectX;


void ObjectCameraBehaviour::move(Move::Translation::Direction direction)
{
}

void ObjectCameraBehaviour::rotate(Move::Rotation::Direction direction)
{
    using namespace std::chrono;

    time_point<system_clock> nowTime = std::chrono::system_clock::now();

    switch (direction)
    {
    case Move::Rotation::X_CLOCKWISE:
        m_rotationAroundX -= 1.0f;
        break;

    case Move::Rotation::X_INVERT_CLOCKWISE:
        m_rotationAroundX += 1.0f;
        break;

    case Move::Rotation::Y_CLOCKWISE:
        m_rotationAroundY -= 1.0f;
        break;

    case Move::Rotation::Y_INVERT_CLOCKWISE:
        m_rotationAroundY += 1.0f;
        break;

    default:
        return;
    }

    float angleY;
    float angleX;

    // Angles in Radians
    angleY = DirectX::XMConvertToRadians(m_rotationAroundY);
    angleX = DirectX::XMConvertToRadians(m_rotationAroundX);


    // Prevent camera from flipping
    // You can change the values to block the camera before 90
    if (m_rotationAroundX < -85.0f)
        m_rotationAroundX = -85.0f;
    else if (m_rotationAroundX > 85.0f)
        m_rotationAroundX = 85.0f;

    // Look in the new direction
    m_gameObject->m_transform.m_forward.vector4_f32[0] = sin(angleY) * cos(angleX);
    m_gameObject->m_transform.m_forward.vector4_f32[1] = sin(angleX);
    m_gameObject->m_transform.m_forward.vector4_f32[2] = cos(angleX) * cos(angleY);
    // Update the rightDirection vector when rotating
    m_gameObject->m_transform.m_right =
        DirectX::XMVector3Normalize(
            DirectX::XMVector3Cross(
                m_gameObject->m_transform.m_up,
                m_gameObject->m_transform.m_forward
            )
        );
}

void ObjectCameraBehaviour::anime(float ellapsedTime)
{
    // Pour les mouvements, nous utilisons le gestionnaire de saisie
    PM3D::CMoteurWindows& rMoteur = PM3D::CMoteurWindows::GetInstance();
    CDIManipulateur& rGestionnaireDeSaisie = rMoteur.GetGestionnaireDeSaisie();

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

    // Move the camera to the target position
    m_gameObject->m_transform.m_position = m_target->m_transform.m_position;

    // Translate the camera back by the offset
    XMVECTOR dir = -m_gameObject->m_transform.m_forward * m_offset;
    m_gameObject->translate(dir);

    // Update the view matrix
    m_cameraComponent->updateViewMatrix();
}
