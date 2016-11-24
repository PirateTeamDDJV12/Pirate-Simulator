#pragma once
#include "GameObject.h"
#include <memory>

namespace PirateSimulator
{
	
		class PhysicsManager
		{
		public:
			static PhysicsManager singleton;
	
		private:
			physx::unique_ptr<physx::PxFoundation> _foundation=nullptr;
			physx::unique_ptr<physx::PxProfileZoneManager> _profileZoneManager = nullptr;
			physx::unique_ptr<physx::PxPhysics> _physics = nullptr;
			physx::unique_ptr<physx::PxScene> _scene = nullptr;
			

		public:
				PhysicsManager(const PhysicsManager&) = delete;
				PhysicsManager& operator=(const PhysicsManager&) = delete;

		public: // IComponent
			//virtual void onAttached(const GameEngineRef &iGameEngine) override;
			//virtual void onDetached(const GameEngineRef &iGameEngine) override;

		public: // ISimulationManager
			virtual physx::PxPhysics& physics();
			virtual physx::PxScene& scene();

		public:
			void update();
			void init();
			
			
		};
	}



