#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"

#include "GameObject.h"
#include "TimeManager.h"
//#include "../PetitMoteur3D/Moteur.h"
#include "SimulationManager.h"
#include <chrono>

using namespace physx;
using namespace std::chrono;

namespace PirateSimulator {

	//=============================================================================
	// PRIVATE FUNCTIONS
	//=============================================================================
	class SimulationEventCallback : public PxSimulationEventCallback
	{
	public:
		virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override
		{}

		virtual void onWake(PxActor** actors, PxU32 count) override
		{}

		virtual void onSleep(PxActor** actors, PxU32 count) override
		{}

	/*	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override
		{
			GameObject *actor0 = nullptr;
			if (!(pairHeader.flags & PxContactPairHeaderFlag::eDELETED_ACTOR_0))
				actor0 = static_cast<GameObject*>(pairHeader.actors[0]->userData);

			GameObject *actor1 = nullptr;
			if (!(pairHeader.flags & PxContactPairHeaderFlag::eDELETED_ACTOR_1))
				actor1 = static_cast<GameObject*>(pairHeader.actors[1]->userData);


			for (int i = 0; i < (int)nbPairs; ++i)
			{
				if (actor0)
					actor0->as<IColliderInterface>()->onContact(pairs[i]);

				if (actor1)
					actor1->as<IColliderInterface>()->onContact(pairs[i]);
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

				IGameObject *trigger = nullptr;
				if (!(pairs->flags & PxTriggerPairFlag::eDELETED_SHAPE_TRIGGER))
					trigger = static_cast<IGameObject*>(pairs[i].triggerShape->getActor()->userData);

				IGameObject *other = nullptr;
				if (!(pairs->flags & PxTriggerPairFlag::eDELETED_SHAPE_OTHER))
					other = static_cast<IGameObject*>(pairs[i].otherShape->getActor()->userData);

				if (trigger)
					trigger->as<IColliderInterface>()->onTrigger(triggerEnter, pairs[i].triggerShape, other ? pairs[i].otherShape : nullptr);

				if (other)
					other->as<IColliderInterface>()->onTrigger(triggerEnter, pairs[i].otherShape, trigger ? pairs[i].triggerShape : nullptr);
			}
		}

	};

	*/
	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;
	static SimulationEventCallback gDefaultSimulationCallback;

	class SimulationManager::SimulationManagerImp
	{
	public:
		SimulationManagerImp()
			: _foundation(nullptr)
			, _profileZoneManager(nullptr)
			, _physics(nullptr)
			, _scene(nullptr)
			, _cpuDispatcher(nullptr)
			, _cudaContextManager(nullptr)
			, _visualDebuggerConnection(nullptr)
		{}

	public:
		//-------------------------------------------------------------------------
		//
		physx::PxPhysics& physics()
		{
			return *_physics;
		}

		//-------------------------------------------------------------------------
		//
		physx::PxScene& scene()
		{
			return *_scene;
		}

		//-------------------------------------------------------------------------
		//
		void initialize()
		{
			_foundation = physx::unique_ptr<PxFoundation>(
				PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback));
			if (!_foundation)
				LOG_FATAL("PxCreateFoundation failed");

			bool recordMemoryAllocations = true;
			_profileZoneManager = physx::unique_ptr<PxProfileZoneManager>(
				&PxProfileZoneManager::createProfileZoneManager(_foundation.get())
				);
			if (!_profileZoneManager)
				LOG_FATAL("PxProfileZoneManager::createProfileZoneManager failed");

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
			if (!_physics)
				LOG_FATAL("PxCreatePhysics failed");


			if (!PxInitExtensions(*_physics))
				LOG_FATAL("PxInitExtensions failed");


			if (_physics->getPvdConnectionManager() != nullptr)
			{
				PxVisualDebuggerConnectionFlags connectionFlags(PxVisualDebuggerExt::getAllConnectionFlags());
				_visualDebuggerConnection = physx::unique_ptr<debugger::comm::PvdConnection>(
					PxVisualDebuggerExt::createConnection(_physics->getPvdConnectionManager(),
						"localhost", 5425, 100, connectionFlags));
				if (_visualDebuggerConnection == nullptr)
					LOG_INFO("    NOT CONNECTED!\n");
				else
					LOG_INFO("    CONNECTED!\n");
			}



			//-------------------------------------------------------------
			// create the scene
			PxSceneDesc sceneDesc(_physics->getTolerancesScale());
			customizeSceneDesc(&sceneDesc);

			if (!sceneDesc.cpuDispatcher)
			{
				_cpuDispatcher = physx::unique_ptr<PxDefaultCpuDispatcher>(PxDefaultCpuDispatcherCreate(1));
				if (!_cpuDispatcher)
					LOG_FATAL("PxDefaultCpuDispatcherCreate failed!");
				sceneDesc.cpuDispatcher = _cpuDispatcher.get();
			}
			if (!sceneDesc.filterShader)
				sceneDesc.filterShader = PxDefaultSimulationFilterShader;

#if PX_SUPPORT_GPU_PHYSX
			if (!sceneDesc.gpuDispatcher && _cudaContextManager)
			{
				sceneDesc.gpuDispatcher = _cudaContextManager->getGpuDispatcher();
			}
#endif

			_scene = physx::unique_ptr<PxScene>(_physics->createScene(sceneDesc));
			if (!_scene)
				LOG_FATAL("createScene failed!");

			_scene->setSimulationEventCallback(&gDefaultSimulationCallback);
		}

		//-------------------------------------------------------------------------
		//
		void customizeSceneDesc(PxSceneDesc *aSceneDesc)
		{
			aSceneDesc->gravity = PxVec3(0.0f, -9.81f, 0.0f);
			//aSceneDesc->gravity = PxVec3(0.0f, 0.0f, 0.0f);
			aSceneDesc->filterShader = &SimulationManagerImp::SimulationFilterShader;
		}

		//-------------------------------------------------------------------------
		//
		static PxFilterFlags SimulationFilterShader(
			PxFilterObjectAttributes attributes0, PxFilterData filterData0,
			PxFilterObjectAttributes attributes1, PxFilterData filterData1,
			PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
		{
			// let triggers through
			if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
			{
				pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
				return PxFilterFlag::eDEFAULT;
			}
			// generate contacts for all that were not filtered above
			pairFlags = PxPairFlag::eCONTACT_DEFAULT;

			// trigger the contact callback for pairs (A,B) where 
			// the filtermask of A contains the ID of B and vice versa.
			if ((filterData0.word0 & filterData1.word1) || (filterData1.word0 & filterData0.word1))
			{
				pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
				return PxFilterFlag::eDEFAULT;
			}

			return PxFilterFlag::eSUPPRESS;
		}

		//-------------------------------------------------------------------------
		//
		void cleanup()
		{
			_scene.reset();
			_cpuDispatcher.reset();
			_visualDebuggerConnection.reset();

			if (_physics)
			{
				PxCloseExtensions();
				_physics.reset();
			}

			_cudaContextManager.reset();

			_profileZoneManager.reset();
			_foundation.reset();
		}

		void update()
		{
			GameClock::duration remainingToSimulate = Game<ITimeManager>()->currentTime() - Game<ITimeManager>()->lastFrameTime();

			auto durationStep = 100ms;
			while (remainingToSimulate > 0ms)
			{
				if (remainingToSimulate > durationStep)
				{
					_scene->simulate(duration_cast<duration<PxReal>>(durationStep).count());
					remainingToSimulate -= durationStep;
				}
				else
				{
					_scene->simulate(duration_cast<duration<PxReal>>(remainingToSimulate).count());
					remainingToSimulate = 0ms;
				}
				_scene->fetchResults(true);
			}
		}
	private:
		physx::unique_ptr<PxFoundation> _foundation;
		physx::unique_ptr<PxProfileZoneManager> _profileZoneManager;
		physx::unique_ptr<PxPhysics> _physics;
		physx::unique_ptr<PxScene> _scene;
		physx::unique_ptr<PxDefaultCpuDispatcher> _cpuDispatcher;
		physx::unique_ptr<PxCudaContextManager> _cudaContextManager;
		physx::unique_ptr<debugger::comm::PvdConnection> _visualDebuggerConnection;
	};

	SimulationManager::SimulationManager()
		: _imp(new SimulationManagerImp)
	{}

	SimulationManager::~SimulationManager()
	{
		delete _imp;
	}

	physx::PxPhysics& SimulationManager::physics()
	{
		return _imp->physics();
	}

	physx::PxScene& SimulationManager::scene()
	{
		return _imp->scene();
	}



	void SimulationManager::update()
	{
		_imp->update();
	}

	void SimulationManager::onAttached(const GameEngineRef &iGameEngine)
	{
		_imp->initialize();
	}

	void SimulationManager::onDetached(const GameEngineRef &iGameEngine)
	{
		_imp->cleanup();
	}
} // namespace engine

  //=============================================================================
  // COMPONENT REGISTRATION
  //=============================================================================
#include "Engine/Managers/GameManagerFactory.h"


namespace engine {

	IComponent::IdType SimulationManager::TypeId() { return "SimulationManager"; }
	RegisterGameManagerType<SimulationManager> gManagerRegistration;
} // namespace engine


