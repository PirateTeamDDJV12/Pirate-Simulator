#include <dinput.h>

#include "PlayerBehaviour.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "CameraManager.h"

#include <dinput.h>

using namespace PirateSimulator;
using namespace DirectX;

PlayerBehaviour::PlayerBehaviour() : m_speed{5000.0f}, m_cameraRef{CameraManager::singleton.getMainCameraGO()}, test{0.0f}
{}

void PlayerBehaviour::move(Move::Translation::Direction direction)
{
    Transform transform = m_gameObject->m_transform;

    // Change the boat forward to match camera forward 
    GameObjectRef camera = CameraManager::singleton.getMainCameraGO();
    XMVECTOR newDir{camera->m_transform.getForward().vector4_f32[0], 0.0f, camera->m_transform.getForward().vector4_f32[2]};
    m_gameObject->m_transform.setForward(newDir);

    //Get Actor shape to move it
    ShapeComponent* boatShape = PhysicsManager::singleton.getVehiculeShape();

    float ellapsedTime = TimeManager::GetInstance().getElapsedTimeFrame();

    switch(direction)
    {
        case Move::Translation::FORWARD:
        {
            m_gameObject->translate(m_gameObject->m_transform.getForward() * m_speed * ellapsedTime);
            DirectX::XMVECTOR position = m_gameObject->m_transform.getPosition();
            // 			_pxActor->addForce(frontVector);
            boatShape->setPose(transform.getPose());
        }
        break;

        case Move::Translation::BACKWARD:
        {
            m_gameObject->translate(-m_gameObject->m_transform.getForward() * m_speed * ellapsedTime);
            DirectX::XMVECTOR position = m_gameObject->m_transform.getPosition();
            // 			_pxActor->addForce(frontVector);
            boatShape->setPose(transform.getPose());
        }
        break;

        case Move::Translation::LEFT:
        {
            m_gameObject->translate(-transform.getRight() * m_speed * ellapsedTime);
            DirectX::XMVECTOR position = m_gameObject->m_transform.getPosition();
            // 			_pxActor->addForce(frontVector);
            boatShape->setPose(transform.getPose());

        }
        break;

        case Move::Translation::RIGHT:
        {
            m_gameObject->translate(transform.getRight() * m_speed * ellapsedTime);
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
    test += 0.05f;
    CDIManipulateur& rGestionnaireDeSaisie = InputManager::singleton.getManipulator();

    XMVECTOR position = m_gameObject->m_transform.getPosition();
    float angle = sinf(test);
    m_gameObject->setPosition(position.vector4_f32[0], angle * 0.5f, position.vector4_f32[2]);
    // Translate the boat
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