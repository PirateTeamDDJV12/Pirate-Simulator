#include "PlayerBehaviour.h"
#include "PhysicsManager.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"
#include <dinput.h>
#include "InputManager.h"

using namespace PirateSimulator;
using namespace DirectX;

PlayerBehaviour::PlayerBehaviour() : m_speed{1.0f}, m_cameraRef{CameraManager::singleton.getMainCameraGO()}
{}

void PlayerBehaviour::move(Move::Translation::Direction direction)
{
    Transform transform = m_gameObject->m_transform;

    // Change the boat forward to match camera forward 
    GameObjectRef camera = CameraManager::singleton.getMainCameraGO();
    m_gameObject->m_transform.m_forward.vector4_f32[0] = camera->m_transform.m_forward.vector4_f32[0];
    m_gameObject->m_transform.m_forward.vector4_f32[1] = 0.f;
    m_gameObject->m_transform.m_forward.vector4_f32[2] = camera->m_transform.m_forward.vector4_f32[2];

    m_gameObject->m_transform.m_forward = XMVector3Normalize(m_gameObject->m_transform.m_forward);

    //Get Actor shape to move it
    //VehicleShape boatShape = PhysicsManager::singleton.

    switch(direction)
    {
        case Move::Translation::FORWARD:
        {
            m_gameObject->translate(m_gameObject->m_transform.m_forward * m_speed);
            
        }
        break;

        case Move::Translation::BACKWARD:
        {
            m_gameObject->translate(-m_gameObject->m_transform.m_forward * m_speed);
        }
        break;

        case Move::Translation::LEFT:
        {
            m_gameObject->translate(-transform.m_right * m_speed);

        }
        break;

        case Move::Translation::RIGHT:
        {
            m_gameObject->translate(transform.m_right * m_speed);
        }
        break;

        default:
            return;
    }
}

void PlayerBehaviour::rotate(Move::Rotation::Direction direction)
{

}

void PlayerBehaviour::anime(float ellapsedTime)
{
    // Pour les mouvements, nous utilisons le gestionnaire de saisie
    CDIManipulateur& rGestionnaireDeSaisie = InputManager::singleton.getManipulator();

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_CAPSLOCK))
    {
        m_speed = 5.0f;
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
}