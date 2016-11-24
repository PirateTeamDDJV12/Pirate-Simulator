#pragma once
//=============================================================================
// EXTERNAL DECLARATIONS
//=============================================================================
#include "Component.h"
#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
//#include "Engine/Game/GameComponent.h"

namespace PirateSimulator {

	//=============================================================================
	// CLASS SimulationManager
	//=============================================================================
	class SimulationManager
		: public Component
		//, public ISimulationManager
	{
	public:
		static std::string getTypeId();
		/*static ComponentInterface::IdType* Interfaces()
		{
			static IComponentInterface::IdType sInterfaces[] = {
				ISimulationManager::TypeId(),
				0
			};

			return sInterfaces;
		}*/

	public:
		SimulationManager();
		virtual ~SimulationManager();

	public: // IComponen
	/*	virtual void onAttached(const GameEngineRef &iGameEngine);
		virtual void onDetached(const GameEngineRef &iGameEngine);*/

	public: // ISimulationManager
		virtual physx::PxPhysics& physics();
		virtual physx::PxScene& scene();

	public:
		void update();

	private:
		class SimulationManagerImp;
		SimulationManagerImp *_imp;
	};
} // namespace engine
