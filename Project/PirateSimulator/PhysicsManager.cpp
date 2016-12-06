#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
#include "PhysicsManager.h"


//#include  "../PetitMoteur3D/PetitMoteur3D/PhysX/Include/PxPhysicsAPI.h"
#include "GameObject.h"
#include "TimeManager.h"
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
			ShapeComponent *actor0 = nullptr;
			if (!(pairHeader.flags & PxContactPairHeaderFlag::eDELETED_ACTOR_0))
				actor0 = static_cast<ShapeComponent*>(pairHeader.actors[0]->userData);

			ShapeComponent *actor1 = nullptr;
			if (!(pairHeader.flags & PxContactPairHeaderFlag::eDELETED_ACTOR_1))
				actor1 = static_cast<ShapeComponent*>(pairHeader.actors[1]->userData);


			for (int i = 0; i < (int)nbPairs; ++i)
			{
				if (actor0)
					actor0->onContact(pairs[i]);

				if (actor1)
					actor1->onContact(pairs[i]);
			}
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

				ShapeComponent *trigger = nullptr;
				if (!(pairs->flags & PxTriggerPairFlag::eDELETED_SHAPE_TRIGGER))
					trigger = static_cast<ShapeComponent*>(pairs[i].triggerShape->getActor()->userData);

				ShapeComponent *other = nullptr;
				if (!(pairs->flags & PxTriggerPairFlag::eDELETED_SHAPE_OTHER))
					other = static_cast<ShapeComponent*>(pairs[i].otherShape->getActor()->userData);

				if (trigger)
					trigger->onTrigger(triggerEnter, pairs[i].triggerShape, other ? pairs[i].otherShape : nullptr);

				if (other)
					other->onTrigger(triggerEnter, pairs[i].otherShape, trigger ? pairs[i].triggerShape : nullptr);
			}
		}

	};
	static physx::PxDefaultErrorCallback gDefaultErrorCallback;
	static physx::PxDefaultAllocator gDefaultAllocatorCallback;
	static SimulationEventCallback gDefaultSimulationCallback;

	///////////////////////////////////////END/////////////////////////////


	void PhysicsManager::update()
	{
        auto &timeManager = TimeManager::GetInstance();
        

        auto durationStep = 10ms;
        while (timeManager.isTimeToUpdate())
        {
            _scene->simulate(duration_cast<duration<PxReal>>(durationStep).count());
            _scene->fetchResults(true);
        }

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

    void PhysicsManager::reset()
    {
        for (auto shape : m_components)
        {
            shape->cleanUp();
            //shape->~ShapeComponent();
            //m_components.pop_back();
        }
        m_components.clear();
        /*specificRelease(_foundation);

        specificRelease(_physics);
        specificRelease(_scene);
        specificRelease(_cpuDispatcher);
        specificRelease(_cudaContextManager);
        specificRelease(_visualDebuggerConnection);
        _profileZoneManager->release();*/
    }
}

