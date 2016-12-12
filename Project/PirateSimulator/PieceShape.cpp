#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
#include "PieceShape.h"
#include "PhysicsManager.h"
#include "ShapeComponent.h"
#include "Piece.h"
#include "GameObjectManager.h"
#include "ICollisionHandler.h"

using namespace PirateSimulator;
using namespace physx;

class CollisionPieceHandler : public ICollisionHandler
{
    void onContact(const physx::PxContactPair &aContactPair)
    {
        ShapeComponent* actor0 = static_cast<ShapeComponent*>(aContactPair.shapes[0]->getActor()->userData);
        ShapeComponent* actor1 = static_cast<ShapeComponent*>(aContactPair.shapes[1]->getActor()->userData);
        if (actor1->getTypeId() == "PieceShape")
        {
            //unspawn the piece
            actor1->getPiece()->destroyPiece();
        }
        else //the piece is not actor1, so it is actor0
        {

            actor0->getPiece()->destroyPiece();
        }
    }
    
    void onTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape)
    {
        //auto actor0 = static_cast<ShapeComponent*>(contactShape->getActor()->userData);
        //auto actor1 = static_cast<ShapeComponent*>(actorShape->getActor()->userData);
        //TODO: Set Behaviour
    }
};

void PieceShape::setGameObject(GameObject* parent)
{
    m_gameObject = parent;
    //piece = piece;
    GameObjectManager::singleton.getGameObjectByName(m_gameObject->getName());
    //m_material = PhysicsManager::singleton.physics().createMaterial(0.5f, 0.5f, 0.1f);
    m_actor = PhysicsManager::singleton.physics().createRigidDynamic(physx::PxTransform(PxVec3(parent->m_transform.getPosition().vector4_f32[0], parent->m_transform.getPosition().vector4_f32[1], parent->m_transform.getPosition().vector4_f32[2])));
    m_shape = m_actor->createShape(physx::PxBoxGeometry(10.f, 25.f, 20.f), *m_material);
    PhysicsManager::singleton.scene().addActor(*m_actor);
    PxFilterData filterData;
    filterData.word0 = EACTORPIECE |EACTORVEHICLE;
    filterData.word1 = EACTORVEHICLE ;
    PhysicsManager::singleton.registerNewComponent(this);
   // PhysicsManager::singleton.setHandler(ICollisionHandlerRef(new CollisionCowHandler));


}

