#include "ShapeComponent.h"
#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
#include "PhysicsManager.h"
#include "GameObject.h"
#include "Piece.h"


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

    /*physx::unique_ptr<physx::PxRigidStatic> ShapeComponent::createPxActor()
    {
        
    }*/

    void ShapeComponent::cleanUp()
    {
        /*m_actor->release();
        m_material->release();
        m_shape->release();*/
    }

    Piece* ShapeComponent::getPiece()
    {
        return m_piece;
    }
}