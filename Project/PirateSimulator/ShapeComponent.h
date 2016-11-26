
#ifndef SHAPE_COMPONENT_H
#define SHAPE_COMPONENT_H
#include "Component.h"


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
        physx::PxMaterial* m_material;
        physx::PxShape* m_shape;
        physx::PxRigidDynamic* m_actor;

    public:
        ShapeComponent() : m_material(nullptr), m_actor(nullptr), m_shape(nullptr)
        {
        }


    public:
        static physx::unique_ptr<physx::PxRigidDynamic> createPxActor();

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
