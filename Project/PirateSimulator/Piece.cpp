#include "Piece.h"

#include "IBehaviour.h"
#include "BlocMeshStructure.h"
#include "BlocMesh.h"

#include <algorithm>
#include "TimeManager.h"

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
        m_gameObject->setWorldMatrix(DirectX::XMMatrixRotationY(m_xAngle) * DirectX::XMMatrixTranslationFromVector(m_gameObject->m_transform.m_position));
    };
};


Piece::Piece(const Transform& spawnPosition, size_t pieceID) :
    GameObject(spawnPosition, "Piece" + to_string(pieceID)),
    m_pieceID{pieceID},
    m_unspawnedTime{TimeManager::msNow().count()}
{}


void PieceFabrik::createPiece(const Transform& pos, size_t pieceID)
{
    auto pieceInstance = GameObjectManager::singleton.subscribeAGameObject(new Piece(pos, pieceID));

    auto pieceMesh = new BlocMesh<BlocStructure>(10.f, 10.f, 1.f, ShaderBloc::ShadersParams(), L"MiniPhong.vhl", L"PieceShader.phl");

    pieceInstance->addComponent<IBehaviour>(new PieceBehaviour());
    pieceInstance->addComponent<IMesh>(pieceMesh);

    RendererManager::singleton.addAStaticSortableMesh(pieceMesh);
}

long long Piece::getUnspawnedTime() const noexcept
{
    return TimeManager::msNow().count() - m_unspawnedTime;
}
