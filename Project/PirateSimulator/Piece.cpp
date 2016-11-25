#include "Piece.h"

#include "IBehaviour.h"
#include "BlocMeshStructure.h"
#include "BlocMesh.h"

#include <algorithm>

using namespace PirateSimulator;


class PieceBehaviour : public IBehaviour
{
public:
    static constexpr const float PIECE_ANGULAR_SPEED = 2.f;

public:
    float m_xAngle;


private:
    static std::string typeId() noexcept { return "PieceBehaviour"; }
    virtual std::string getTypeId() const noexcept { return IBehaviour::typeId(); }

    virtual void anime(float ellapsedTime) 
    {
        m_xAngle = m_xAngle + (DirectX::XM_PI * ellapsedTime);

        // modifier la matrice de l'objet X
        m_gameObject->setWorldMatrix(DirectX::XMMatrixRotationY(m_xAngle) * DirectX::XMMatrixTranslationFromVector(m_gameObject->m_transform.m_position));
    };
};


GameObjectRef Piece::createPiece()
{
    if (this->isEmpty())
    {
        GameObjectManager::singleton.setSubscribingStrategy(GameObjectManager::PIECE);

        m_pieceInstance = GameObjectManager::singleton.subscribeAGameObject(new GameObject(m_transform, "Piece" + std::to_string(m_pieceID)));

        GameObjectManager::singleton.setSubscribingStrategy(GameObjectManager::NONE);

        m_pieceInstance->addComponent<IBehaviour>(new PieceBehaviour());
        m_pieceInstance->addComponent<IMesh>(new BlocMesh<BlocStructure>(1.f, 1.f, 0.1f, ShaderBloc::ShadersParams()));
    }

    return m_pieceInstance;
}

void Piece::destroyPiece()
{
    if (isInstanciated())
    {
        GameObjectManager::singleton.unspawnGameObject(m_pieceInstance->m_name);
        m_pieceInstance = GameObjectRef();
    }
}
