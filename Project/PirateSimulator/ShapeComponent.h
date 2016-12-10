
#ifndef SHAPE_COMPONENT_H
#define SHAPE_COMPONENT_H
#include "Component.h"
#include "PhysicsManager.h"


namespace physx
{
    class PxMaterial;
    class PxShape;
}


namespace PirateSimulator
{
    class Piece;

    //class GameObject;
    class ShapeComponent : public Component
    {
        
    protected:
        const physx::PxMaterial* m_material = PhysicsManager::singleton.physics().createMaterial(0.5f, 0.5f, 0.1f);
        physx::PxShape* m_shape;
        physx::PxRigidDynamic* m_actor;
        

    public:
        ShapeComponent() : m_actor(nullptr), m_shape(nullptr)
        {
        }
        virtual std::string getTypeId() const noexcept override = 0;
        virtual void onContact(const physx::PxContactPair &aContactPair) =0;
        enum ACTOR
        {
            EACTORTERRAIN = 1<<0,
            EACTORVEHICLE = 1<<1,
            EACTORPIECE = 1<<2
        };



        virtual void onTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape) =0;


    public:
        physx::unique_ptr<physx::PxRigidStatic> ShapeComponent::createPxActor()
        {
        return physx::unique_ptr<physx::PxRigidStatic>(
            PhysicsManager::singleton.physics().createRigidStatic(physx::PxTransform::createIdentity())
            );
        }
        physx::PxRigidDynamic& pxActor();

        virtual physx::PxTransform pose();
        void setPose(const physx::PxTransform &iPose);
        void cleanUp();


    };



}
#endif // 
