#include "PieceShape.h"
#include "PhysicsManager.h"
#include "GameObjectManager.h"
#include "SoundManager.h"
#include "ShapeComponent.h"
#include "Piece.h"
#include "ICollisionHandler.h"
#include "../PetitMoteur3D/PetitMoteur3D/PhysX/Include/PxPhysicsAPI.h"

using namespace PirateSimulator;
using namespace physx;

class CollisionPieceHandler : public ICollisionHandler
{
    void onContact(const physx::PxContactPair &aContactPair) override
    {}

    void onTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape) override
    {
        if(actorShape == nullptr || contactShape == nullptr)
        {
            return;
        }

        auto actor0 = static_cast<GameObject*>(contactShape->getActor()->userData);
        auto actor1 = static_cast<GameObject*>(actorShape->getActor()->userData);

        if(triggerEnter)
        {
            if(actor1->getComponent<ShapeComponent>()->isPiece() && actor0->getComponent<ShapeComponent>()->isBoat())
            {
                GameObjectManager::singleton.getPieceAdministrator()->addScore();
                GameObjectManager::singleton.destroyCoin(actor1->getComponent<ShapeComponent>()->getPiece());
            }
            else if(actor0->getComponent<ShapeComponent>()->isPiece() && actor1->getComponent<ShapeComponent>()->isBoat())
            {
                GameObjectManager::singleton.getPieceAdministrator()->addScore();
                GameObjectManager::singleton.destroyCoin(actor0->getComponent<ShapeComponent>()->getPiece());
            }

            PirateSimulator::SoundManager::singleton.playMusic("PirateSimulator/JackpotPieceNoise.wav");
        }
    }
};


void PieceShape::setGameObject(GameObject* parent)
{
    m_gameObject = parent;

    PxTransform parentTransform = physx::PxTransform(PxVec3(
        parent->m_transform.getPosition().vector4_f32[0],
        parent->m_transform.getPosition().vector4_f32[1],
        parent->m_transform.getPosition().vector4_f32[2]));
    m_actor = physx::unique_ptr<physx::PxRigidDynamic>(PhysicsManager::singleton.physics().createRigidDynamic(parentTransform));

    m_shape = m_actor->createShape(physx::PxBoxGeometry(10.f, 25.f, 20.f), *m_material);
    PhysicsManager::singleton.scene().addActor(*m_actor);
    m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
    m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

    PxFilterData filterData;
    filterData.word0 = EACTORPIECE;
    filterData.word1 = EACTORVEHICLE;
    m_shape->setSimulationFilterData(filterData);

    PhysicsManager::singleton.registerNewComponent(this);

    setHandler(ICollisionHandlerRef(new CollisionPieceHandler));

    m_actor->setMass(0.0001f);
    m_actor->userData = parent;
}

