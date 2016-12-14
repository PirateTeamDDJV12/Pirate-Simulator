#include "ShapeComponent.h"
#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
#include "PhysicsManager.h"
#include "GameObject.h"
#include "Piece.h"
#include "ICollisionHandler.h"


using namespace physx;
namespace PirateSimulator
{

    physx::PxRigidDynamic* ShapeComponent::pxActor()
    {
        return m_actor.get();
    }
    physx::PxTransform ShapeComponent::pose()
    {
        return m_actor->getGlobalPose();
    }
    void ShapeComponent::setPose(const physx::PxTransform &iPose)
    {
        m_actor->setGlobalPose(iPose);
    }



    void ShapeComponent::cleanUp()
    {
        m_actor.reset();
        PhysicsManager::singleton.removeComponent(this);
    }

    Piece* ShapeComponent::getPiece()
    {
        return m_piece;
    }

    void ShapeComponent::setHandler(ICollisionHandlerRef callback)
    {
        handler = callback;
    }
}