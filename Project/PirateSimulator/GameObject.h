/*
Created by Sun-lay Gagneux
*/
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_


#include "Transform.h"
#include "IBehaviour.h"
#include "Mesh.h"

#include <memory>


namespace PirateSimulator
{
    using MeshRef = std::shared_ptr<Mesh>;
    using BehaviourRef = std::shared_ptr<IBehaviour>;

    class GameObject
    {
    private:
        Transform m_transform;
        BehaviourRef m_behaviour;
        MeshRef m_mesh;
        DirectX::XMMATRIX m_matWorld;				// Matrice de transformation dans le monde

    public:
        GameObject(const Transform& transform) :
            m_transform{ transform },
            m_mesh{},
            m_matWorld{ DirectX::XMMatrixIdentity() },
            m_behaviour{ new IBehaviour() }
        {}

    public:
        
        template<class T>
        void addComponent(T *component);

        template<>
        void addComponent<Mesh>(Mesh *component)
        {
            m_mesh = MeshRef(component);
            m_mesh->setGameObject(this);
        }

        template<>
        void addComponent<IBehaviour>(IBehaviour *component)
        {
            m_behaviour = BehaviourRef(component);
            m_behaviour->setGameObject(this);
        }


        virtual void anime(float ellapsedTime) { m_behaviour->anime(ellapsedTime); }
        void draw() { m_mesh->Draw(); }

        MeshRef getMesh() const { return m_mesh; }

        const Transform& getTransform() const noexcept { return m_transform; }
        void setTransform(const Transform& transform) noexcept { m_transform = transform; }

        const DirectX::XMMATRIX& getWorldMatrix() const noexcept { return m_matWorld; }
        void setWorldMatrix(const DirectX::XMMATRIX& world) noexcept { m_matWorld = world; }
    };
}


#endif //!_GAMEOBJECT_H_