#include "VehicleShape.h"
#include "PhysicsManager.h"
#include "ShapeComponent.h"
#include "ICollisionHandler.h"
#include "GameObject.h"
#include "PlayerBehaviour.h"
#include "../PetitMoteur3D/PetitMoteur3D/PhysX/Include/PxPhysicsAPI.h"

using namespace PirateSimulator;
using namespace physx;

class CollisionVehicleHandler : public ICollisionHandler
{
    void onContact(const physx::PxContactPair &aContactPair)
    {
        
    }

    void onTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape) override
    {

    }
};

void VehicleShape::setGameObject(GameObject* parent)
{
    auto position = parent->m_transform.getPosition();
    physx::PxVec3 newPos(position.vector4_f32[0], position.vector4_f32[1], position.vector4_f32[2]);


    m_actor = physx::unique_ptr<physx::PxRigidDynamic>(PhysicsManager::singleton.physics().createRigidDynamic(physx::PxTransform::createIdentity()));
    m_shape = m_actor->createShape(physx::PxSphereGeometry(11.f), *m_material);


    auto boatPose = pose();
    boatPose.p = newPos;
    setPose(boatPose);

    PxFilterData filterData;
    filterData.word0 = EACTORVEHICLE;
    filterData.word1 = EACTORPIECE | EACTORTERRAIN| EACTORTUNNEL;
    m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
    m_shape->setSimulationFilterData(filterData);

    setHandler(ICollisionHandlerRef(new CollisionVehicleHandler));
    m_actor->userData = parent;
    m_actor->setMass(1.0f);
    PhysicsManager::singleton.scene().addActor(*m_actor);
    PhysicsManager::singleton.registerNewComponent(this);
}

