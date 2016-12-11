#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
//=============================================================================
// EXTERNAL DECLARATIONS
//=============================================================================
#include "SimulationComponent.h"
#include "PhysicsManager.h"

namespace PirateSimulator {


    //=============================================================================
    // CLASS DynamicSimulationComponent
    //=============================================================================
    //-----------------------------------------------------------------------------
    //
    physx::PxRigidDynamic& DynamicSimulationComponent::pxActor()
    {
        return *_pxActor;
    }

    //-----------------------------------------------------------------------------
    //
    physx::PxTransform DynamicSimulationComponent::pose()
    {
        return pxActor().getGlobalPose();
    }

    //-----------------------------------------------------------------------------
    //
    void DynamicSimulationComponent::setPose(const physx::PxTransform &iPose)
    {
        pxActor().setGlobalPose(iPose);
    }

    //-----------------------------------------------------------------------------
    //
    physx::unique_ptr<physx::PxRigidDynamic> DynamicSimulationComponent::createPxActor()
    {
        return physx::unique_ptr<physx::PxRigidDynamic>(
            PhysicsManager::singleton.physics().createRigidDynamic(physx::PxTransform::createIdentity())
            );
    }


    //=============================================================================
    // CLASS StaticSimulationComponent
    //=============================================================================
    //-----------------------------------------------------------------------------
    //
    physx::PxTransform StaticSimulationComponent::pose()
    {
        return pxActor().getGlobalPose();
    }

    //-----------------------------------------------------------------------------
    //
    void StaticSimulationComponent::setPose(const physx::PxTransform &iPose)
    {
        pxActor().setGlobalPose(iPose);
    }

    //-----------------------------------------------------------------------------
    //
    physx::PxRigidStatic& StaticSimulationComponent::pxActor()
    {
        return *_pxActor;
    }

    //-----------------------------------------------------------------------------
    //
    physx::unique_ptr<physx::PxRigidStatic> StaticSimulationComponent::createPxActor()
    {
        return physx::unique_ptr<physx::PxRigidStatic>(
            PhysicsManager::singleton.physics().createRigidStatic(physx::PxTransform::createIdentity())
            );
    }

} // namespace engine

  //=============================================================================
  // COMPONENT REGISTRATION
  //=============================================================================
namespace PirateSimulator {
    std::string DynamicSimulationComponent::TypeId() { return "DynamicSimulationComponent"; }
    std::string StaticSimulationComponent::TypeId() { return "StaticSimulationComponent"; }

    //RegisterComponentType<DynamicSimulationComponent>* gRegisterPxRigidDynamicSimulationComponent = new RegisterComponentType<DynamicSimulationComponent>();
    //RegisterComponentType<StaticSimulationComponent>* gRegisterPxRigidStaticSimulationComponent = new RegisterComponentType<StaticSimulationComponent>();
}