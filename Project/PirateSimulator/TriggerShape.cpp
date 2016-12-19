#include "TriggerShape.h"
#include "PhysicsManager.h"
#include "ShapeComponent.h"
#include "ICollisionHandler.h"
#include "GameObject.h"

using namespace PirateSimulator;
using namespace physx;

class CollisionTriggerHandler : public ICollisionHandler
{
    void onContact(const physx::PxContactPair &aContactPair)
    {

    }

    void onTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape) override
    {

    }
};

void TriggerShape::setGameObject(GameObject* parent)
{
    PxTransform tunnelPose;
    DirectX::XMVECTOR position = parent->m_transform.getPosition();
    auto pose = parent->m_transform.getPose();
    pose.p.x += 80.f;
    pose.p.z += 1000.f;

    PxRigidStatic &m_actor = *PhysicsManager::singleton.physics().createRigidStatic(pose);
    m_shape = m_actor.createShape(PxBoxGeometry(250.f, 200.f,800.f), *m_material);

    m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
    m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
    PxFilterData filterDataCylindre;
    filterDataCylindre.word0 = EACTORTRIGGER;
    filterDataCylindre.word1 = EACTORVEHICLE;
    m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
    m_actor.userData = parent;
    m_shape->setSimulationFilterData(filterDataCylindre);
    PhysicsManager::singleton.scene().addActor(m_actor);

    //Register shape
    setHandler(ICollisionHandlerRef(new CollisionTriggerHandler));
    m_gameObject = parent;

}

