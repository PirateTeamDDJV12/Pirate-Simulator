#ifndef ICOLLISION_HANDLER_H
#define ICOLLISION_HANDLER_H

//=============================================================================
// EXTERNAL DECLARATIONS
//=============================================================================
#include "../PetitMoteur3D/PetitMoteur3D/PhysX/Include/PxPhysicsAPI.forward.h"
#include <memory>

namespace PirateSimulator
{

    //=============================================================================
    // INTERFACE ICollisionHandler
    //=============================================================================
    class ICollisionHandler
    {
    public:
        virtual void onContact(const physx::PxContactPair &aContactPair) = 0;
        virtual void onTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape) = 0;
    };

    using ICollisionHandlerRef = std::shared_ptr<ICollisionHandler>;
}

#endif // ICOLLISION_HANDLER_H