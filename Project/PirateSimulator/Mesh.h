#pragma once
#include "../PetitMoteur3D/PetitMoteur3D/dispositif.h"

#include "Component.h"



namespace PirateSimulator
{
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
        virtual ~Mesh(void) {}


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
