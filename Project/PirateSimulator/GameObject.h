/*
Created by Sun-lay Gagneux
*/
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_


#include "Transform.h"
#include "IBehaviour.h"
#include "Mesh.h"


#include <vector>
#include <memory>


namespace PirateSimulator
{
    using ComponentRef = std::shared_ptr<Component>;


    class GameObject
    {
    private:
        void (GameObject::* m_pSetMatrix)(const DirectX::XMMATRIX& world);
        void (GameObject::* m_pAnim)(float elapsedTime);


    protected:
        Transform m_transform;

        std::vector<ComponentRef> m_attachedComponent;

        IBehaviour* m_behaviour;
        IMesh* m_mesh;
        

    public:
        GameObject(const Transform& transform) :
            m_transform{ transform },
            m_pSetMatrix{ &GameObject::setWorldMatrixWhenNotHavingAMesh },
            m_pAnim{ &GameObject::animNothing }
        {
            m_attachedComponent.push_back(ComponentRef(new IBehaviour()));
        }


    public:
        template<class ComponentAttribute>
        void addComponent(ComponentAttribute* component)
        {
            static_assert(std::is_convertible<ComponentAttribute*, Component*>::value &&
                          !std::is_null_pointer<ComponentAttribute*>::value, "You want to attach a component that is not!");

            m_attachedComponent.push_back(ComponentRef(component));
            m_attachedComponent[m_attachedComponent.size() - 1]->setGameObject(this);
        }

        template<>
        void addComponent<IBehaviour>(IBehaviour* component)
        {
            if (component)
            {
                m_attachedComponent[0] = ComponentRef(component);
                m_behaviour = component;
                m_pAnim = &GameObject::animSomething;
            }
        }

        template<>
        void addComponent<IMesh>(IMesh* component)
        {
            if (component)
            {
                m_attachedComponent.push_back(ComponentRef(component));
                m_mesh = component;
                m_pSetMatrix = &GameObject::setWorldMatrixWhenHaving;
            }
        }

        template<class ComponentAttribute>
        ComponentAttribute* getComponent()
        {
            static_assert(std::is_convertible<ComponentAttribute*, Component*>::value, "You want to get something that is not component!");

            for (auto iter = m_attachedComponent.begin(); iter != m_attachedComponent.end(); ++iter)
            {
                if (Component::sameTypeIdAs((*iter)->getTypeId(), ComponentAttribute::typeId()))
                {
                    return (*iter)->as<ComponentAttribute>();
                }
            }

            return nullptr;
        }

        template<>
        IBehaviour* getComponent<IBehaviour>() { return m_behaviour; }

        template<>
        IMesh* getComponent<IMesh>() { return m_mesh; }


        virtual void anime(float elapsedTime) { (this->*m_pAnim)(elapsedTime); }
        void draw() 
        { 
            m_mesh->Draw(); 
        }

        const Transform& getTransform() const noexcept { return m_transform; }
        void setTransform(const Transform& transform) noexcept { m_transform = transform; }

        void translate(float x, float y, float z);
        void rotate(float angleX, float angleY);

        void setWorldMatrix(const DirectX::XMMATRIX& world)
        {
            (this->*m_pSetMatrix)(world);
        }

    private:
        void setWorldMatrixWhenHaving(const DirectX::XMMATRIX& world) { m_mesh->setWorldMatrix(world); }
        void setWorldMatrixWhenNotHavingAMesh(const DirectX::XMMATRIX& world) {}

        void animSomething(float elapsedTime) { m_behaviour->anime(elapsedTime); }
        void animNothing(float elapsedTime) {}
    };
}


#endif //!_GAMEOBJECT_H_