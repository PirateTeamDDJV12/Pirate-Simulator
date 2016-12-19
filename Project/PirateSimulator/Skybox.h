#ifndef SKYBOX_H
#define SKYBOX_H

#include "Mesh.h"
#include "Effect.h"

#include <DirectXMath.h>

namespace PM3D
{
    class CTexture;
    class CDispositifD3D11;
}

namespace PirateSimulator
{
    class CSommetSky
    {
    public:
        static UINT numElements;
        static D3D11_INPUT_ELEMENT_DESC layout[];


    public:
        DirectX::XMFLOAT3 m_position;
        DirectX::XMFLOAT2 m_coordTex;
        

    public:
        CSommetSky() {}
        CSommetSky(DirectX::XMFLOAT3 vertexPosition, DirectX::XMFLOAT2 textureCoordinate = DirectX::XMFLOAT2(0.0f, 0.0f)) :
            m_position{ vertexPosition },
            m_coordTex{ textureCoordinate }
        {}
    };

    namespace ShaderCSkyBox
    {
        struct ShadersParams
        {
            DirectX::XMMATRIX matWorldViewProj;

            DirectX::XMFLOAT3 sunCoefficient;

            float uselessFill;
        };
    }

    class CSkybox : public Mesh<ShaderCSkyBox::ShadersParams>
    {
    private:
        PM3D::CDispositifD3D11* pDispositif;
        ID3D11Buffer* pVertexBuffer;
        ID3D11Buffer* pIndexBuffer;

        // Définitions des valeurs d'animation
        DirectX::XMMATRIX matWorld;

        // Pour les effets
        ID3D11ShaderResourceView* pTextureD3D;
        Effect m_effect;


    public:
        CSkybox();
        virtual ~CSkybox(void);


    public:
        void Draw();
        void setTexture(const std::wstring& fileName);


    protected:
        void InitEffet();

        //Set the sun angle coefficient for the skybox in regard to its position (does the day/night cycle on the skybox)
        void setSunAngleCoeff(float sunAngle);
    };
}

#endif //!SKYBOX_H