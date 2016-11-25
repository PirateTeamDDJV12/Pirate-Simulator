#include "ShapeComponent.h"

#include "PhysicsManager.h"
#include "GameObject.h"


using namespace physx;
namespace PirateSimulator
{

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