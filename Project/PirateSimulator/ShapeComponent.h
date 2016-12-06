
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

    class GameObject;
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




        virtual void onTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape) =0;


    public:
        //static physx::unique_ptr<physx::PxRigidDynamic> createPxActor();

        physx::PxRigidDynamic& pxActor();

        virtual physx::PxTransform pose();
        void setPose(const physx::PxTransform &iPose);
        void cleanUp()
        {
            /*m_actor->release();
            m_material->release();
            m_shape->release();*/
        }

    };



}
#endif // 
