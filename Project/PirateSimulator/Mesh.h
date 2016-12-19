#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include "Component.h"


namespace PirateSimulator
{
    class GameObject;

    class IMesh : public Component 
    {
    protected:
        DirectX::XMMATRIX m_matWorld;				// Matrice de transformation dans le monde

    public:
        virtual void Draw() = 0;

        void setWorldMatrix(const DirectX::XMMATRIX& world) noexcept { m_matWorld = world; }
        const DirectX::XMMATRIX& getWorldMatrix() const noexcept { return m_matWorld; }

        static std::string typeId() noexcept { return "MeshComponent"; }
        virtual std::string getTypeId() const noexcept { return IMesh::typeId(); }

        virtual void setGameObject(GameObject* parent);
    };

    //  Classe : Mesh
    //
    //  BUT : 	Classe de base de tous nos objets 3D
    //
    template<class ShaderParameter>
    class Mesh : public IMesh
    {
    protected:
        ShaderParameter m_shaderParameter;

        using IMesh::m_matWorld;

    public:
        // Destructeur
        virtual ~Mesh() {}


    public:
        virtual void Draw() = 0;


    protected:
        // Constructeur par défaut
        Mesh(const ShaderParameter& shaderParameter) :
            m_shaderParameter{shaderParameter}
        {
            m_matWorld = DirectX::XMMatrixIdentity();
        }
    };

}

#endif //!MESH_H_INCLUDED