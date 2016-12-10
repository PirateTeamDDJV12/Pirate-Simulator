#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
#include "PieceShape.h"
#include "PhysicsManager.h"
#include "ShapeComponent.h"
#include "Piece.h"

using namespace PirateSimulator;
using namespace physx;

void PieceShape::setGameObject(GameObject* parent, Piece* piece)
{
    m_gameObject = parent;
    m_piece = piece;
    //m_material = PhysicsManager::singleton.physics().createMaterial(0.5f, 0.5f, 0.1f);
    m_actor = PhysicsManager::singleton.physics().createRigidDynamic(physx::PxTransform::createIdentity());
    m_shape = m_actor->createShape(physx::PxBoxGeometry(10.f, 25.f, 20.f), *m_material);
    PxFilterData filterData;
    filterData.word0 = EACTORPIECE;
    filterData.word1 = EACTORVEHICLE | EACTORTERRAIN;
    PhysicsManager::singleton.registerNewComponent(this);
}
