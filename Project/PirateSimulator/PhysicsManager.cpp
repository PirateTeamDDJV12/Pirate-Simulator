#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
#include "PhysicsManager.h"


//#include  "../PetitMoteur3D/PetitMoteur3D/PhysX/Include/PxPhysicsAPI.h"
#include "GameObject.h"
using namespace physx;


PirateSimulator::PhysicsManager PirateSimulator::PhysicsManager::singleton;

namespace PirateSimulator {

	///////////////////////CALLBACKS//////////////////////////
	class SimulationEventCallback : public PxSimulationEventCallback
	{
	public:
		virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override
		{}

		virtual void onWake(PxActor** actors, PxU32 count) override
		{}

		virtual void onSleep(PxActor** actors, PxU32 count) override
		{}

		virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override
		{
			GameObject *actor0 = nullptr;
			if (!(pairHeader.flags & PxContactPairHeaderFlag::eDELETED_ACTOR_0))
				actor0 = static_cast<GameObject*>(pairHeader.actors[0]->userData);

			GameObject *actor1 = nullptr;
			if (!(pairHeader.flags & PxContactPairHeaderFlag::eDELETED_ACTOR_1))
				actor1 = static_cast<GameObject*>(pairHeader.actors[1]->userData);


			/*for (int i = 0; i < (int)nbPairs; ++i)
			{
				if (actor0)
					actor0->as<IColliderInterface>()->onContact(pairs[i]);

				if (actor1)
					actor1->as<IColliderInterface>()->onContact(pairs[i]);
			}*/
		}

		virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override
		{
			for (int i = 0; i < (int)count; ++i)
			{
				bool triggerEnter = false;
				if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
					triggerEnter = true;
				else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
					triggerEnter = false;
				else
					continue;

				GameObject *trigger = nullptr;
				if (!(pairs->flags & PxTriggerPairFlag::eDELETED_SHAPE_TRIGGER))
					trigger = static_cast<GameObject*>(pairs[i].triggerShape->getActor()->userData);

				GameObject *other = nullptr;
				if (!(pairs->flags & PxTriggerPairFlag::eDELETED_SHAPE_OTHER))
					other = static_cast<GameObject*>(pairs[i].otherShape->getActor()->userData);

				/*if (trigger)
					trigger->as<IColliderInterface>()->onTrigger(triggerEnter, pairs[i].triggerShape, other ? pairs[i].otherShape : nullptr);

				if (other)
					other->as<IColliderInterface>()->onTrigger(triggerEnter, pairs[i].otherShape, trigger ? pairs[i].triggerShape : nullptr);*/
			}
		}

	};
	static physx::PxDefaultErrorCallback gDefaultErrorCallback;
	static physx::PxDefaultAllocator gDefaultAllocatorCallback;
	static SimulationEventCallback gDefaultSimulationCallback;

	///////////////////////////////////////END/////////////////////////////


	void PhysicsManager::update()
	{

	}
	void PhysicsManager::initialize()
	{
		_foundation = physx::unique_ptr<PxFoundation>(
			PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback));

		bool recordMemoryAllocations = true;
		_profileZoneManager = physx::unique_ptr<PxProfileZoneManager>(
			&PxProfileZoneManager::createProfileZoneManager(_foundation.get())
			);

#if PX_SUPPORT_GPU_PHYSX
		PxCudaContextManagerDesc cudaContextManagerDesc;
		_cudaContextManager = physx::unique_ptr<PxCudaContextManager>(
			PxCreateCudaContextManager(*_foundation, cudaContextManagerDesc, _profileZoneManager.get()));
		if (_cudaContextManager)
		{
			if (!_cudaContextManager->contextIsValid())
			{
				_cudaContextManager.reset();
			}
		}
#endif

		_physics = physx::unique_ptr<PxPhysics>(
			PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation,
				PxTolerancesScale(), recordMemoryAllocations, _profileZoneManager.get()));



		if (_physics->getPvdConnectionManager() != nullptr)
		{
			PxVisualDebuggerConnectionFlags connectionFlags(PxVisualDebuggerExt::getAllConnectionFlags());
		}
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



