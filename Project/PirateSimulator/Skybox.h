#ifndef SKYBOX_H
#define SKYBOX_H

#include "Mesh.h"
#include "../PetitMoteur3D/PetitMoteur3D/dispositifD3D11.h"
#include "../PetitMoteur3D/PetitMoteur3D/Texture.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include "Effect.h"

namespace PirateSimulator
{
    class CSommetSky
    {
    public:
        CSommetSky() {}
        CSommetSky(DirectX::XMFLOAT3 p, DirectX::XMFLOAT2 c = DirectX::XMFLOAT2(0.0f, 0.0f))
        {
            position = p;
            coordTex = c;
        }
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 coordTex;
        static UINT numElements;
        static D3D11_INPUT_ELEMENT_DESC layout[];
    };

    namespace ShaderCSkyBox
    {
        struct ShadersParams
        {
            DirectX::XMMATRIX matWorldViewProj;
        };
    }

    class CSkybox : public Mesh<ShaderCSkyBox::ShadersParams>
    {
    public:
        CSkybox();
        virtual ~CSkybox(void);
        void Draw();
        void SetTexture(PM3D::CTexture* pTexture);

    protected:
        //CSkybox() :
        //    Mesh<ShaderCSkyBox::ShadersParams>(ShaderCSkyBox::ShadersParams()),
        //    pDispositif { PirateSimulator::RendererManager::singleton.getDispositif() }
        //{}

        void InitEffet();
        PM3D::CDispositifD3D11* pDispositif;
        ID3D11Buffer* pVertexBuffer;
        ID3D11Buffer* pIndexBuffer;

        // Définitions des valeurs d'animation
        DirectX::XMMATRIX matWorld;

        // Pour les effets
        ID3D11ShaderResourceView* pTextureD3D;
        Effect m_effect;
    };
}

#endif //!SKYBOX_H