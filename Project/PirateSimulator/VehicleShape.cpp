#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
#include "VehicleShape.h"
#include "PhysicsManager.h"
#include "ShapeComponent.h"
#include "ICollisionHandler.h"

using namespace PirateSimulator;
using namespace physx;
using SimulationEventCallbackRef = std::shared_ptr<SimulationEventCallback>;

class CollisionVehicleHandler : public ICollisionHandler
{
    void onContact(const physx::PxContactPair &aContactPair)
    {
        auto actor0 = static_cast<ShapeComponent*>(aContactPair.shapes[0]->getActor()->userData);
        auto actor1 = static_cast<ShapeComponent*>(aContactPair.shapes[1]->getActor()->userData);
        //Todo Set Behaviour
    }

    void onTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape) override
    {
        // auto actor0 = static_cast<ShapeComponent*>(contactShape->getActor()->userData);
        //auto actor1 = static_cast<ShapeComponent*>(actorShape->getActor()->userData);
        //TODO: Set Behaviour// Or not, there is no trigger Zone
    }
};


void VehicleShape::setGameObject(GameObject* parent)
{
	m_gameObject = parent;
	//m_material = PhysicsManager::singleton.physics().createMaterial(0.5f, 0.5f, 0.1f);
	m_actor = PhysicsManager::singleton.physics().createRigidDynamic(physx::PxTransform::createIdentity());
	m_shape = m_actor->createShape(physx::PxBoxGeometry(10.f, 15.f, 20.f), *m_material); 
    PxFilterData filterData;
    filterData.word0 = EACTORVEHICLE |EACTORPIECE;
    filterData.word1 = EACTORPIECE | EACTORTERRAIN;
    setHandler(ICollisionHandlerRef(new CollisionVehicleHandler));
    PhysicsManager::singleton.scene().addActor(*m_actor);
    PhysicsManager::singleton.registerNewComponent(this);
}

