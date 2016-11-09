#ifndef MATERIAL_H
#define MATERIAL_H


#include "../PetitMoteur3D/PetitMoteur3D/objet3d.h"
#include "../PetitMoteur3D/PetitMoteur3D/dispositifD3D11.h"
#include "../PetitMoteur3D/PetitMoteur3D/d3dx11effect.h"

#include <string>



namespace PirateSimulator
{
    struct MaterialProperties
    {
        DirectX::XMFLOAT4 ambientValue;
        DirectX::XMFLOAT4 diffuseValue;
        DirectX::XMFLOAT4 specularValue;
        float power;
        bool transparency;

        MaterialProperties() :
            ambientValue{ 1.0f, 1.0f, 1.0f, 1.0f },
            diffuseValue{ 1.0f, 1.0f, 1.0f, 1.0f },
            specularValue{ 0.0f, 0.0f, 0.0f, 1.0f },
            transparency{ false },
            power{ 1.f }
        {}

        MaterialProperties(
            const DirectX::XMFLOAT4& ambientValue, 
            const DirectX::XMFLOAT4& diffuseValue, 
            const DirectX::XMFLOAT4& specularValue, 
            float power, 
            bool transparency
        ) :
            ambientValue{ ambientValue },
            diffuseValue{ diffuseValue },
            specularValue{ specularValue },
            transparency{ transparency },
            power{ power }
        {}

        static DirectX::XMFLOAT4 defaultAmbientValue() noexcept
        {
            return{ 1.0f, 1.0f, 1.0f, 1.0f };
        }

        static DirectX::XMFLOAT4 defaultDiffuseValue() noexcept
        {
            return{ 1.0f, 1.0f, 1.0f, 1.0f };
        }

        static DirectX::XMFLOAT4 defaultSpecularValue() noexcept
        {
            return{ 0.0f, 0.0f, 0.0f, 1.0f };
        }
    };

    class Material
    {
    public:
        std::string m_textureFileName;
        std::string m_materialName;

        ID3D11ShaderResourceView* pTextureD3D;

        MaterialProperties m_property;


    public:
        Material() :
            pTextureD3D{ nullptr },
            m_textureFileName{ "" },
            m_materialName{ "" },
            m_property{}
        {}

        Material(const MaterialProperties& properties) :
            pTextureD3D{ nullptr },
            m_textureFileName{ "" },
            m_materialName{ "" },
            m_property{ properties }
        {}

        Material(ID3D11ShaderResourceView* pTextureD3D, const MaterialProperties& properties) :
            pTextureD3D{ pTextureD3D },
            m_textureFileName{ "" },
            m_materialName{ "" },
            m_property{ properties }
        {}
    };
}


#endif //!MATERIAL_H