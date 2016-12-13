#include "PlayerBehaviour.h"
#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
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
    XMVECTOR newDir{ camera->m_transform.getForward().vector4_f32[0], 0.0f, camera->m_transform.getForward().vector4_f32[2] };
    m_gameObject->m_transform.setForward(newDir);

    //Get Actor shape to move it
    ShapeComponent* boatShape = PhysicsManager::singleton.getVehiculeShape();

    switch(direction)
    {
        case Move::Translation::FORWARD:
        {
            m_gameObject->translate(m_gameObject->m_transform.getForward() * m_speed);
            DirectX::XMVECTOR position = m_gameObject->m_transform.getPosition();
            // 			_pxActor->addForce(frontVector);
            
            boatShape->setPose(transform.getPose());
        }
        break;

        case Move::Translation::BACKWARD:
        {
            m_gameObject->translate(-m_gameObject->m_transform.getForward() * m_speed);
            DirectX::XMVECTOR position = m_gameObject->m_transform.getPosition();
            // 			_pxActor->addForce(frontVector);
            boatShape->setPose(transform.getPose());
        }
        break;

        case Move::Translation::LEFT:
        {
            m_gameObject->translate(-transform.getRight() * m_speed);
            DirectX::XMVECTOR position = m_gameObject->m_transform.getPosition();
            // 			_pxActor->addForce(frontVector);
            boatShape->setPose(transform.getPose());

        }
        break;

        case Move::Translation::RIGHT:
        {
            m_gameObject->translate(transform.getRight() * m_speed);
            DirectX::XMVECTOR position = m_gameObject->m_transform.getPosition();
            // 			_pxActor->addForce(frontVector);
            boatShape->setPose(transform.getPose());
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

    if(rGestionnaireDeSaisie.getKey(DIK_CAPSLOCK))
    {
        m_speed = 5.0f;
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
}