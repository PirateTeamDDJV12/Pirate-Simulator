#include "PlayerBehaviour.h"
#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
#include "PhysicsManager.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"
#include <dinput.h>
#include "InputManager.h"

using namespace PirateSimulator;
using namespace DirectX;
using namespace physx;
PlayerBehaviour::PlayerBehaviour() : m_speed{0.5f}, m_cameraRef{CameraManager::singleton.getMainCameraGO()}
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
    auto boatPose = boatShape->pose();

    switch(direction)
    {
    case Move::Translation::FORWARD:
    {
        //m_gameObject->translate(m_gameObject->m_transform.getForward() * m_speed);
        //DirectX::XMVECTOR position = m_gameObject->m_transform.getPosition();

        //boatShape->setPose(transform.getPose());

        PxVec3 frontVector = PxVec3{ m_gameObject->m_transform.getForward().vector4_f32[0],m_gameObject->m_transform.getForward().vector4_f32[1],m_gameObject->m_transform.getForward().vector4_f32[2] };

        boatPose.p += frontVector *m_speed;
        boatPose.q = PxQuat::createIdentity();
        boatShape->setPose(boatPose);
        XMVECTOR vecToTranslate = XMVECTOR{ boatPose.p.x,boatPose.p.y ,boatPose.p.z }-m_gameObject->m_transform.getPosition();
        vecToTranslate.vector4_f32[1] = 0;
        m_gameObject->translate(vecToTranslate);
    }
    break;

    case Move::Translation::BACKWARD:
    {
        
        
        PxVec3 frontVector = PxVec3{ m_gameObject->m_transform.getForward().vector4_f32[0],m_gameObject->m_transform.getForward().vector4_f32[1],m_gameObject->m_transform.getForward().vector4_f32[2] };
        boatPose.p -= frontVector *m_speed;
        boatPose.q = PxQuat::createIdentity();
        boatShape->setPose(boatPose);

        //Get move vector
        XMVECTOR vecToTranslate = XMVECTOR{boatPose.p.x,boatPose.p.y ,boatPose.p.z }- m_gameObject->m_transform.getPosition();
        vecToTranslate.vector4_f32[1] = 0;
        m_gameObject->translate(vecToTranslate);

    }
    break;

    case Move::Translation::LEFT:
    {
        
        PxVec3 leftVector = -PxVec3{ m_gameObject->m_transform.getRight().vector4_f32[0],m_gameObject->m_transform.getRight().vector4_f32[1],m_gameObject->m_transform.getRight().vector4_f32[2] };
        boatPose.p += leftVector *m_speed;
        boatPose.q = PxQuat::createIdentity();
        boatShape->setPose(boatPose);

        //Get move vector
        XMVECTOR vecToTranslate = XMVECTOR{ boatPose.p.x,boatPose.p.y ,boatPose.p.z }-m_gameObject->m_transform.getPosition();
        vecToTranslate.vector4_f32[1] = 0;
        m_gameObject->translate(vecToTranslate);

    }
    break;

    case Move::Translation::RIGHT:
    {
        //m_gameObject->translate(transform.getRight() * m_speed);
        //DirectX::XMVECTOR position = m_gameObject->m_transform.getPosition();
        //// 			_pxActor->addForce(frontVector);
        //boatShape->setPose(transform.getPose());

        PxVec3 leftVector = -PxVec3{ m_gameObject->m_transform.getRight().vector4_f32[0],m_gameObject->m_transform.getRight().vector4_f32[1],m_gameObject->m_transform.getRight().vector4_f32[2] };
        // 			_pxActor->addForce(frontVector);
        boatPose.p -= leftVector *m_speed;
        boatPose.q = PxQuat::createIdentity();
        boatShape->setPose(boatPose);

        //Get move vector
        XMVECTOR vecToTranslate = XMVECTOR{ boatPose.p.x,boatPose.p.y ,boatPose.p.z }-m_gameObject->m_transform.getPosition();
        
        vecToTranslate.vector4_f32[1] = 0;
        m_gameObject->translate(vecToTranslate);
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