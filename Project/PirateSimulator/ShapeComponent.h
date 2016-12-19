
#ifndef SHAPE_COMPONENT_H
#define SHAPE_COMPONENT_H
#include "Component.h"
#include "PhysicsManager.h"
#include "ICollisionHandler.h"
#include <memory>


namespace physx
{
    class PxMaterial;
    class PxShape;
}


namespace PirateSimulator
{
    class Piece;
    class ShapeComponent : public Component
    {
        
    protected:
        const physx::PxMaterial* m_material = PhysicsManager::singleton.physics().createMaterial(0.0f, 0.0f, 0.0f);
        physx::PxShape* m_shape;
        physx::unique_ptr<physx::PxRigidDynamic> m_actor;
        Piece* m_piece;
        ICollisionHandlerRef handler;
    public:
        ShapeComponent() : m_actor(nullptr), m_shape(nullptr)
        {
        }
        static std::string typeId() noexcept { return "ShapeComponent"; }
        virtual std::string getTypeId() const noexcept override = 0;
        enum ACTOR
        {
            EACTORTERRAIN = 1<<0,
            EACTORVEHICLE = 1<<1,
            EACTORPIECE = 1<<2,
            EACTORTUNNEL=1<<3,
            EACTORTRIGGER=1<<4 
        };





    public:
        physx::PxRigidDynamic* ShapeComponent::pxActor();

        virtual bool isBoat() = 0;
        virtual bool isPiece() = 0;
        virtual bool isTrigger() = 0;
        virtual physx::PxTransform pose();
        void setPose(const physx::PxTransform &iPose);
        void cleanUp();

        Piece* getPiece();
        void setHandler(ICollisionHandlerRef callback);
        ICollisionHandler *getHandler()
        {
            return handler.get();
        };

        void onContact(const physx::PxContactPair &aContactPair)
        {
            if (handler)
            {
                handler->onContact(aContactPair);
            }
        }
    };
}

#endif //ifndef SHAPE_COMPONENT_H
