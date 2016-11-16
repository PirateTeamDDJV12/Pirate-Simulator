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
		SommetPlane() {}
		SommetPlane(DirectX::XMFLOAT3 p, DirectX::XMFLOAT3 _normal, DirectX::XMFLOAT2 c = DirectX::XMFLOAT2(0.0f, 0.0f))
		{
			normale = _normal;
			position = p;
			coordTex = c;
		}
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normale;
		DirectX::XMFLOAT2 coordTex;

		static UINT numElements;
		static D3D11_INPUT_ELEMENT_DESC layout[];
	};
	namespace ShaderPlane
	{
        struct ShadersParams
        {
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
            DirectX::XMFLOAT2 remplissage;


            ShadersParams() :
                bTex{ 1 }
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
                bTex{ 1 }
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
                bTex{ 1 }
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

		
	private:

	public:
		enum SIZEPLANE {
			XMIN = 0,
			XMAX = 1024,
			ZMIN = 0,
			ZMAX = 1024,
			DZ = ZMAX - ZMIN,
			DX = XMAX - XMIN,
			NBPOINTSX = 257,
			NBPOINTSZ= 257,
			NBPOINTS = NBPOINTSX *NBPOINTSZ,

			TRIANGLE_COUNT = ((NBPOINTSX - 1) * (NBPOINTSZ - 1)) * 2,
			INDEX_COUNT = TRIANGLE_COUNT * 3
		};

		static constexpr float DEFAULT_Y_LEVEL_WATER_PLANE = 0.f;

		std::vector<unsigned int> index;
		std::vector<SommetPlane> sommets;
		Plane(PM3D::CDispositifD3D11* pDispositif, const std::string& textureFileName);
		virtual ~Plane(void);
		void Draw();
		void SetTexture(PM3D::CTexture* pTexture);

	
		Plane(const std::string& textureFileName) :
			Mesh<ShaderPlane::ShadersParams>(ShaderPlane::ShadersParams())
		{ 
            m_material.m_textureFileName = textureFileName;
        }

protected:
		void InitEffet();

        void InitShaders();
        void setTexture(PM3D::CTexture* texture) { m_material.pTextureD3D = texture->GetD3DTexture(); }
        void loadTexture(const std::string& filename);

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
	};
}
#endif