#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
#include "VehicleShape.h"
#include "PhysicsManager.h"
#include "ShapeComponent.h"

using namespace PirateSimulator;
using namespace physx;

void VehicleShape::setGameObject(GameObject* parent)
{
	m_gameObject = parent;
	//m_material = PhysicsManager::singleton.physics().createMaterial(0.5f, 0.5f, 0.1f);
	m_actor = PhysicsManager::singleton.physics().createRigidDynamic(physx::PxTransform::createIdentity());
	m_shape = m_actor->createShape(physx::PxBoxGeometry(10.f, 15.f, 20.f), *m_material); 
    PxFilterData filterData;
    filterData.word0 = EACTORVEHICLE;
    filterData.word1 = EACTORPIECE | EACTORTERRAIN;
    PhysicsManager::singleton.registerNewComponent(this);
}
