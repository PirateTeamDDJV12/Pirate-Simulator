#ifndef PLANE_H
#define PLANE_H

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
	class CSommetPlane
	{
	public:
		CSommetPlane() {}
		CSommetPlane(DirectX::XMFLOAT3 p, DirectX::XMFLOAT3 _normal, DirectX::XMFLOAT2 c = DirectX::XMFLOAT2(0.0f, 0.0f))
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
	namespace ShaderCPlane
	{
		struct ShadersParams
		{
			DirectX::XMMATRIX matWorldViewProj;
		};
	}

	class CPlane : public Mesh<ShaderCPlane::ShadersParams>
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
		std::vector<CSommetPlane> sommets;
		CPlane(PM3D::CDispositifD3D11* pDispositif);
		virtual ~CPlane(void);
		void Draw();
		void SetTexture(PM3D::CTexture* pTexture);

	
		CPlane(void) :
			Mesh<ShaderCPlane::ShadersParams>(ShaderCPlane::ShadersParams())
		{}

protected:
		void InitEffet();

		PM3D::CDispositifD3D11* pDispositif;
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;

		// Définitions des valeurs d'animation
		ID3D11Buffer* pConstantBuffer;

		// Pour les effets
		ID3DX11Effect* pEffet;
		ID3DX11EffectTechnique* pTechnique;
		ID3DX11EffectPass* pPasse;
		ID3D11InputLayout* pVertexLayout;
		ID3D11ShaderResourceView* pTextureD3D;
		ID3D11SamplerState* pSampleState;
	};
}
#endif