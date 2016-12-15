#include "Piece.h"
#include "IBehaviour.h"
#include "PieceMesh.h"
#include "TimeManager.h"
#include "GameObjectManager.h"
#include "RendererManager.h"
#include "PieceShape.h"
#include "../PetitMoteur3D/PetitMoteur3D/PhysX/Include/PxPhysicsAPI.h"
#include "../PetitMoteur3D/PetitMoteur3D/PhysX/Include/PxRigidDynamic.h"
#include "../PetitMoteur3D/PetitMoteur3D/ChargeurAssimp.h"
#include "../PetitMoteur3D/PetitMoteur3D/ObjetMesh.h"
#include <algorithm>
using namespace PirateSimulator;


class PieceBehaviour : public IBehaviour
{
public:
    static constexpr const float PIECE_ANGULAR_SPEED = 50.f;

public:
    float m_xAngle;


private:
    static std::string typeId() noexcept
    {
        return "PieceBehaviour";
    }
    virtual std::string getTypeId() const noexcept
    {
        return IBehaviour::typeId();
    }

    virtual void anime(float elapsedTime)
    {
        m_xAngle = m_xAngle + (elapsedTime * PIECE_ANGULAR_SPEED);

        // modifier la matrice de l'objet X
        m_gameObject->setWorldMatrix(DirectX::XMMatrixRotationY(m_xAngle) * DirectX::XMMatrixTranslationFromVector(m_gameObject->m_transform.getPosition()));
    };
};


Piece::Piece(const Transform& spawnPosition, size_t pieceID) :
    m_transform{ spawnPosition },
    m_pieceID{ pieceID },
    m_unspawnedTime{ TimeManager::msNow().count() }
{}



GameObjectRef Piece::createPiece()
{
    if (this->isEmpty())
    {
        GameObjectManager::singleton.setSubscribingStrategy(GameObjectManager::PIECE);


        m_pieceInstance = GameObjectManager::singleton.subscribeAGameObject(new GameObject(m_transform, "Piece" + std::to_string(m_pieceID)));

        GameObjectManager::singleton.setSubscribingStrategy(GameObjectManager::NONE);

        PM3D::CChargeurAssimp chargeur;

        // Création du mesh du boat à partir d'un fichier .OBJ
        PM3D::CParametresChargement paramPiece(".\\PirateSimulator\\", "PieceCoin.obj", false, false);
        chargeur.Chargement(paramPiece);

        auto pieceMesh = new PieceMesh(PirateSimulator::ShaderPieceMesh::ShadersParams(), L"PieceShader.fx", chargeur);
        auto pieceShape = new PirateSimulator::PieceShape();

        m_pieceInstance->addComponent<IBehaviour>(new PieceBehaviour());
        m_pieceInstance->addComponent<IMesh>(pieceMesh);
        m_pieceInstance->addComponent<ShapeComponent>(pieceShape);
        pieceShape->setPiece(this);
        //m_pieceInstance->addComponent<DynamicSimulationComponent>();


     //   std::shared_ptr<DynamicSimulationComponent> simulationComponent;


        RendererManager::singleton.addAStaticSortableMesh(pieceMesh);
    }


    return m_pieceInstance;
}


void Piece::destroyPiece()
{
    if (isInstanciated())
    {
        RendererManager::singleton.removeAStaticSortableMesh(m_pieceInstance->getComponent<IMesh>());
        m_pieceInstance->getComponent<ShapeComponent>()->cleanUp();
        //PhysicsManager::singleton.scene().removeActor(actor);
        GameObjectManager::singleton.unspawnGameObject(m_pieceInstance->m_name);
        m_pieceInstance = GameObjectRef();
        m_unspawnedTime = TimeManager::msNow().count();
    }

}

long long Piece::getUnspawnedTime() const noexcept
{
    if (isInstanciated())
    {
        return 0;
    }

    return TimeManager::msNow().count() - m_unspawnedTime;
}

void Piece::anim(float elapsedTime)
{
    m_pieceInstance->getComponent<IBehaviour>()->anime(elapsedTime);
}