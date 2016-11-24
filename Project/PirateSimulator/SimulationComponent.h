#pragma once
//=============================================================================
// EXTERNAL DECLARATIONS
//=============================================================================
//#include "Core/Components/IComponent.h"
//#include "Core/Components/ISimulationInterface.h"
//#include "Core/Components/IDynamicSimulationInterface.h"
//#include "Core/Components/IStaticSimulationComponent.h"
//#include "Core/Components/ICollisionHandler.h"
//#include "Core/Components/IColliderInterface.h"
//#include "Engine/GameObjects/GameObjectComponent.h"
//#include "Core/GameObjects/IGameObject.h"

#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
#include "Component.h"
#include "GameObject.h"
#include <memory>

#pragma warning( disable : 4250 )

namespace PirateSimulator {

	//=============================================================================
	// CLASS SimulationComponentBase
	//=============================================================================
	template<class TDerived, class PXACTORTYPE>
	class SimulationComponentBase :
		public GameObjectComponent<TDerived>,
		public IRigidSimulationInterface,
		public IColliderInterface,
		public IPoseInterface,
		virtual public IComponentInterface
	{
	public:
		virtual void onAttached(const GameObjectRef &iGameObject) override;
		virtual void onDetached(const GameObjectRef &iGameObject) override;
		virtual void onContact(const physx::PxContactPair &aContactPair) override;
		virtual void onTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape) override;
		virtual void setHandler(ICollisionHandlerRef iHandler) override;

	protected:
		GameObjectWeakRef _go;
		physx::unique_ptr<PXACTORTYPE> _pxActor;
		ICollisionHandlerRef _collisionHandler;
	};

	//=============================================================================
	// CLASS DynamicSimulationComponent
	//=============================================================================
	class DynamicSimulationComponent :
		public SimulationComponentBase<DynamicSimulationComponent, physx::PxRigidDynamic>, Component/*,
		public IDynamicSimulationInterface*/
	{
	private:
		using base = SimulationComponentBase;

	public:
		static std::string getTypeId();

		/*static std::string* Interfaces()
		{
			static IComponentInterface::IdType sInterfaces[] = {
				IPoseInterface::TypeId(),
				IColliderInterface::TypeId(),
				ISimulationInterface::TypeId(),
				IRigidSimulationInterface::TypeId(),
				IDynamicSimulationInterface::TypeId(),
				0
			};

			return sInterfaces;
		}
		*/
	public:
		static physx::PxRigidDynamic createPxActor();

		virtual physx::PxTransform pose() ;
		virtual void setPose(const physx::PxTransform &iPose) ;

	public:
		virtual physx::PxRigidDynamic& pxActor() ;
	};

	//=============================================================================
	// CLASS StaticSimulationComponent
	//=============================================================================
	class StaticSimulationComponent :
		public SimulationComponentBase<StaticSimulationComponent, physx::PxRigidStatic>/*,
		public IStaticSimulationInterface*/
	{
	private:
		using Base = SimulationComponentBase;
		
	public:
		static std::string TypeId()
		{
			return "StaticSimulationComponent";
		}

	/*	static IComponentInterface::IdType* Interfaces()
		{
			static IComponentInterface::IdType sInterfaces[] = {
				IPoseInterface::TypeId(),
				IColliderInterface::TypeId(),
				ISimulationInterface::TypeId(),
				IRigidSimulationInterface::TypeId(),
				IStaticSimulationInterface::TypeId(),
				0
			};

			return sInterfaces;
		}
*/
	public:
		static physx::PxRigidStatic createPxActor();

	public: // IStaticSimulationInterface
		virtual physx::PxRigidStatic& pxActor();

	public: // IPoseInterface
		virtual physx::PxTransform pose() ;
		virtual void setPose(const physx::PxTransform &iPose);
	};

} // namespace engine

//#include "SimulationComponent.inline.h"
