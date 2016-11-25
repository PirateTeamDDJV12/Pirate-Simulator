#include "ShapeComponent.h"
#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
#include "PhysicsManager.h"
#include "GameObject.h"


using namespace physx;
namespace PirateSimulator
{
	void ShapeComponent::setGameObject(PirateSimulator::GameObject* parent)
	{
		m_gameObject = parent;
		m_material = PhysicsManager::singleton.physics().createMaterial(0.5f, 0.5f, 0.1f);
		m_actor = PhysicsManager::singleton.physics().createRigidDynamic(physx::PxTransform::createIdentity());
		m_shape = m_actor->createShape(physx::PxSphereGeometry(0.5f), *m_material); //TODO, Change the Float value to fit parent size

	}


/*	 physx::unique_ptr<physx::PxRigidDynamic> ShapeComponent::createPxActor()
	{
		PhysicsManager::singleton.physics().createRigidDynamic(physx::PxTransform::createIdentity());
	}*/
	physx::PxRigidDynamic& ShapeComponent::pxActor()
	{
		return *m_actor;
	}
	physx::PxTransform ShapeComponent::pose()
	{
		return pxActor().getGlobalPose();
	}
	void ShapeComponent::setPose(const physx::PxTransform &iPose)
	{
		pxActor().setGlobalPose(iPose);
	}
}