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

   /* physx::unique_ptr<physx::PxRigidStatic> ShapeComponent::createPxActor()
    {
        return physx::unique_ptr<physx::PxRigidStatic>(
            PhysicsManager::singleton.physics().createRigidStatic(physx::PxTransform::createIdentity())
            );
    }*/

    void ShapeComponent::cleanUp()
    {
        /*m_actor->release();
        m_material->release();
        m_shape->release();*/
    }
}