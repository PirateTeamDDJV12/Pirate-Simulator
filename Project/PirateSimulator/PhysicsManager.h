#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H
#include "../PetitMoteur3D/PetitMoteur3D/PhysX/Include/PxPhysicsAPI.forward.h"
#include "GameObject.h"
#include <memory>

namespace physx
{
	class PxFoundation;
	class PxProfileZoneManager;
}
namespace PirateSimulator
{

	class PhysicsManager
	{
	public:
		static PhysicsManager singleton;
	private:
		physx::unique_ptr<physx::PxFoundation> _foundation = nullptr;
		physx::unique_ptr<physx::PxProfileZoneManager> _profileZoneManager = nullptr;
		physx::unique_ptr<physx::PxPhysics> _physics = nullptr;
		physx::unique_ptr<physx::PxScene> _scene = nullptr;
		
	private:
		PhysicsManager()
		{
			_foundation = nullptr;
			_profileZoneManager = nullptr;
			_physics = nullptr;
			_scene = nullptr;
		}
		PhysicsManager(const PhysicsManager&) = delete;
		PhysicsManager& operator=(const PhysicsManager&) = delete;

	public: // IComponent
		
	public: // ISimulationManager
		virtual physx::PxPhysics& physics();
		virtual physx::PxScene& scene();

	public:
		void update();
		void initialize();


	};
}

#endif

