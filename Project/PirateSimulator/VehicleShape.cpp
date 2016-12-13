#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
#include "VehicleShape.h"
#include "PhysicsManager.h"
#include "ShapeComponent.h"
#include "ICollisionHandler.h"
#include "GameObject.h"

using namespace PirateSimulator;
using namespace physx;

class CollisionVehicleHandler : public ICollisionHandler
{
    void onContact(const physx::PxContactPair &aContactPair)
    {
        auto actor0 = static_cast<GameObject*>(aContactPair.shapes[0]->getActor()->userData);
        auto actor1 = static_cast<GameObject*>(aContactPair.shapes[1]->getActor()->userData);
        //Set Behaviour in piece handler;
    }

    void onTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape) override
    {
        // auto actor0 = static_cast<ShapeComponent*>(contactShape->getActor()->userData);
        //auto actor1 = static_cast<ShapeComponent*>(actorShape->getActor()->userData);
        
    }
};


void VehicleShape::setGameObject(GameObject* parent)
{
    DirectX::XMVECTOR position = parent->m_transform.getPosition();
    physx::PxVec3 newPos(position.vector4_f32[0], position.vector4_f32[1], position.vector4_f32[2]);
    // 			_pxActor->addForce(frontVector);
	m_actor = PhysicsManager::singleton.physics().createRigidDynamic(physx::PxTransform::createIdentity());
	m_shape = m_actor->createShape(physx::PxBoxGeometry(10.f, 15.f, 20.f), *m_material); 
    auto boatPose = pose();
    boatPose.p = newPos;
    setPose(boatPose);
   
    PxFilterData filterData;
    filterData.word0 = EACTORVEHICLE ;
    filterData.word1 = EACTORPIECE | EACTORTERRAIN;
    m_shape->setSimulationFilterData(filterData);
    setHandler(ICollisionHandlerRef(new CollisionVehicleHandler));
    m_actor->userData = parent;
    m_actor->setMass(99999.f);
    PhysicsManager::singleton.scene().addActor(*m_actor);
    PhysicsManager::singleton.registerNewComponent(this);
}

