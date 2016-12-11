#pragma once
#include <string>
#include "PhysicsManager.h"
#include "ICollisionHandler.h"
//=============================================================================
// EXTERNAL DECLARATIONS
//=============================================================================

namespace PirateSimulator
{

    //=============================================================================
    // INTERFACE IColliderInterface
    //=============================================================================
    class IColliderInterface 
    {
    public:
        static std::string TypeId();

    public:
        virtual void onContact(const physx::PxContactPair &aContactPair) = 0;
        virtual void onTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape) = 0;
        virtual void setHandler(ICollisionHandlerRef ICollisionHandler) = 0;
    };

} // namespace engine
