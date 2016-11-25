#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
#include "PhysicsManager.h"

//#include  "../PetitMoteur3D/PetitMoteur3D/PhysX/Include/PxPhysicsAPI.h"
#include "GameObject.h"
using namespace physx;


PirateSimulator::PhysicsManager PirateSimulator::PhysicsManager::singleton;

namespace PirateSimulator {


	void PhysicsManager::update()
	{

	};
	void PhysicsManager::init()
	{

	}


	physx::PxPhysics& PhysicsManager::physics()
	{
		return  *_physics;
	}

	physx::PxScene& PhysicsManager::scene()
	{
		return *_scene;
	}

}



