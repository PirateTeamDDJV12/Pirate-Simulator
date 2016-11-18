#ifndef PLANE_H
#define PLANE_H

#include "Material.h"
#include "Effect.h"
#include "Mesh.h"
#include "../PetitMoteur3D/PetitMoteur3D/dispositifD3D11.h"
#include "../PetitMoteur3D/PetitMoteur3D/Texture.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include "../PetitMoteur3D/PetitMoteur3D/d3dx11effect.h"

#include <vector>
#include <DirectXMath.h>
#include <math.h>
namespace PirateSimulator
{
    class SommetPlane
    {
    public:
        static UINT numElements;
        static D3D11_INPUT_ELEMENT_DESC layout[];


    public:
        DirectX::XMFLOAT3 m_position;
        DirectX::XMFLOAT3 m_normale;
        DirectX::XMFLOAT2 m_coordTex;
        float m_originAngle;


    public:
        SommetPlane() {}
        SommetPlane(
            const DirectX::XMFLOAT3& position, 
            const DirectX::XMFLOAT3& normal, 
            const DirectX::XMFLOAT2& coordinate = DirectX::XMFLOAT2(0.0f, 0.0f),
            float originAngle = 0.f
        ) :
            m_originAngle{ originAngle }
        {
            m_normale = normal;
            m_position = position;
            m_coordTex = coordinate;
        }
    };


    namespace ShaderPlane
    {
        struct ShadersParams
        {
        public:
            DirectX::XMMATRIX matWorldViewProj;	// la matrice totale 
            DirectX::XMMATRIX matWorld;			// matrice de transformation dans le monde 

            DirectX::XMVECTOR vLumiere; 			// la position de la source d'éclairage (Point)
            DirectX::XMVECTOR vCamera; 			// la position de la caméra

            DirectX::XMVECTOR vAEcl; 			// la valeur ambiante de l'éclairage
            DirectX::XMVECTOR vAMat; 			// la valeur ambiante du matériau
            DirectX::XMVECTOR vDEcl; 			// la valeur diffuse de l'éclairage 
            DirectX::XMVECTOR vDMat; 			// la valeur diffuse du matériau 
            DirectX::XMVECTOR vSEcl; 			// la valeur spéculaire de l'éclairage 
            DirectX::XMVECTOR vSMat; 			// la valeur spéculaire du matériau 

            float puissance;
            int bTex;					// Texture ou materiau 
            float tick;

            float remplissage;


        public:
            ShadersParams() :
                bTex{ 1 },
                tick{ 0.f }
            {
                vLumiere = DirectX::XMVectorSet(130.0f, 130.0f, -50.0f, 1.0f);
                vAEcl = DirectX::XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
                vAMat = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
                vDEcl = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
                vDMat = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);

            }

            ShadersParams(
                const DirectX::XMVECTOR& lumiere,
                const DirectX::XMVECTOR& AEcl,
                const DirectX::XMVECTOR& AMat,
                const DirectX::XMVECTOR& DEcl,
                const DirectX::XMVECTOR& DMat
            ) :
                bTex{ 1 },
                tick{ 0.f }
            {
                vLumiere = lumiere;
                vAEcl = AEcl;
                vAMat = AMat;
                vDEcl = DEcl;
                vDMat = DMat;
            }

            ShadersParams(
                const DirectX::XMVECTOR& lumiere,
                const DirectX::XMVECTOR& AEcl,
                const DirectX::XMVECTOR& AMat,
                const DirectX::XMVECTOR& DEcl,
                const DirectX::XMVECTOR& DMat,
                const DirectX::XMVECTOR& SEcl,
                const DirectX::XMVECTOR& SMat
            ) :
                bTex{ 1 },
                tick{ 0.f }
            {
                vLumiere = lumiere;
                vAEcl = AEcl;
                vAMat = AMat;
                vDEcl = DEcl;
                vDMat = DMat;
                vSEcl = SEcl;
                vSMat = SMat;
            }
        };
    }

    class Plane : public Mesh<ShaderPlane::ShadersParams>
    {
    public:
        enum
        {
            X_MIN = 0,
            X_MAX = 1024,
            Z_MIN = 0,
            Z_MAX = 1024,
            
            POINTS_X_COUNT = 257,
            POINTS_Z_COUNT = 257,

            LAST_X_POINT_INDEX = POINTS_X_COUNT - 1,
            LAST_Z_POINT_INDEX = POINTS_Z_COUNT - 1,
            NBPOINTS = POINTS_X_COUNT * POINTS_Z_COUNT,
            TRIANGLE_COUNT = LAST_X_POINT_INDEX * LAST_Z_POINT_INDEX * 2,
            INDEX_COUNT = TRIANGLE_COUNT * 3
        };

        enum
        {
            SIN_ARRAY_ELEMENTS_COUNT = 360
        };

        static constexpr const float DEFAULT_Y_LEVEL_WATER_PLANE = 0.f;
        static constexpr const float Z_DELTA = static_cast<float>(Z_MAX - Z_MIN);
        static constexpr const float X_DELTA = static_cast<float>(X_MAX - X_MIN);
        static constexpr const float TICK_INCREMENT = 1.f;


    private:
        std::vector<unsigned int> index;
        std::vector<SommetPlane> sommets;


    protected:
        PM3D::CDispositifD3D11* pDispositif;

        ID3D11Buffer* pVertexBuffer;
        ID3D11Buffer* pIndexBuffer;

        ID3D11VertexShader*  pVertexShader;
        ID3D11PixelShader*  pPixelShader;
        ID3D11InputLayout* pVertexLayout;

        // Définitions des valeurs d'animation
        ID3D11Buffer* pConstantBuffer;

        //Pour texture
        Material m_material;
        Effect m_textureEffect;

        ID3D11ShaderResourceView* pSinTex;
        ID3D11Texture1D* pSinText1D;
        ID3D11SamplerState* pSinSampler;


    public:
        Plane(const std::string& textureFileName) :
            Mesh<ShaderPlane::ShadersParams>(ShaderPlane::ShadersParams())
        {
            m_material.m_textureFileName = textureFileName;
        }

        Plane(PM3D::CDispositifD3D11* pDispositif, const std::string& textureFileName);

        virtual ~Plane(void);

        
    public:
        void Draw();
        void SetTexture(PM3D::CTexture* pTexture);


    protected:
        void InitEffet();

        void InitShaders();
        void setTexture(PM3D::CTexture* texture) { m_material.pTextureD3D = texture->GetD3DTexture(); }
        void loadTexture(const std::string& filename);    

        void InitSin();
    };
}
#endif