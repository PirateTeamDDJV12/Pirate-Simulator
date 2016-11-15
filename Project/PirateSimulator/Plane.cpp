#include "StdAfx.h"
#include "Plane.h"
#include "../PetitMoteur3D/PetitMoteur3D/Resource.h"
#include "../PetitMoteur3D/PetitMoteur3D/Singleton.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"
#include "../PetitMoteur3D/PetitMoteur3D/util.h"

#include <d3d11.h>
#include <winnt.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>

using namespace PirateSimulator;
using namespace PM3D;
using namespace DirectX;
using namespace UtilitairesDX;


// Définir l'organisation de notre sommet
D3D11_INPUT_ELEMENT_DESC CSommetPlane::layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
UINT CSommetPlane::numElements = ARRAYSIZE(CSommetPlane::layout);

CPlane::CPlane(CDispositifD3D11* pDispositif_) :
	Mesh<ShaderCPlane::ShadersParams>(ShaderCPlane::ShadersParams())
{

	const int xMax=256;
	const int xMin=0;
	const int zMax=256;
	const int zMin=0;
	float y=0.0f;
	const unsigned int nbPointsX = xMax - xMin;
	const unsigned int nbPointsZ = zMax - zMin;
	//CHANGETHIS
	const unsigned int index[36] =
	{
		0, 1, 2,    // devant
		0, 2, 3,    // devant
		4, 5, 6,    // arrière
		4, 6, 7,    // arrière
		8, 9, 10,   // dessous
		8, 10, 11,  // dessous
		12, 13, 14, // dessus
		12, 14, 15, // dessus
		16, 17, 18, // gauche
		16, 18, 19, // gauche
		20, 21, 22, // droite
		20, 22, 23  // droite
	};

	CSommetPlane sommets[nbPointsX*nbPointsZ];
	float m = 0.00001f;
	/*TODO: CREER POINTS*/
	XMFLOAT3 point[nbPointsX*nbPointsZ];
	for (int i=0;i<nbPointsZ;i++)
	{
		for (int j = 0;j < nbPointsX;j++)
		{
			
			point[i*nbPointsX + j] = XMFLOAT3{ (j*xMax + (nbPointsX - j)*xMin) / static_cast<float>(nbPointsX),y,(i*zMax + (nbPointsZ - i)*zMin) / static_cast<float>(nbPointsZ) };
		}
	}

	//point[0] = XMFLOAT3(1.0f, 1.0f, 1.0f);

	// Création du vertex buffer et copie des sommets
	ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CSommetPlane) * nbPointsX*nbPointsZ; 
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = sommets;
	pVertexBuffer = NULL;
	DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);
	// Création de l'index buffer et copie des indices
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * 36; //CHANGE
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = index;
	pIndexBuffer = NULL;
	DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer), DXE_CREATIONINDEXBUFFER);

	// Initialisation de l'effet
	InitEffet();
	matWorld = XMMatrixIdentity();
}

void CPlane::InitEffet()
{
	// Compilation et chargement du vertex shader
	ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

	// Création d'un tampon pour les constantes du VS
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ShaderCPlane::ShadersParams);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	pD3DDevice->CreateBuffer(&bd, NULL, &pConstantBuffer);

	// Pour l'effet
	ID3DBlob* pFXBlob = NULL;
	DXEssayer(D3DCompileFromFile(L"MiniPhong.fx", 0, 0, 0,
		"fx_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0,
		&pFXBlob, NULL),
		DXE_ERREURCREATION_FX);

	D3DX11CreateEffectFromMemory(pFXBlob->GetBufferPointer(), pFXBlob->GetBufferSize(), 0, pD3DDevice, &pEffet);
	pFXBlob->Release();
	pTechnique = pEffet->GetTechniqueByIndex(0);
	pPasse = pTechnique->GetPassByIndex(0);

	// Créer l'organisation des sommets pour le VS de notre effet
	D3DX11_PASS_SHADER_DESC effectVSDesc;
	pPasse->GetVertexShaderDesc(&effectVSDesc);
	D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
	effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex, &effectVSDesc2);
	const void *vsCodePtr = effectVSDesc2.pBytecode;
	unsigned int vsCodeLen = effectVSDesc2.BytecodeLength;
	pVertexLayout = NULL;

	DXEssayer(pD3DDevice->CreateInputLayout(CSommetPlane::layout,
		CSommetPlane::numElements,
		vsCodePtr,
		vsCodeLen,
		&pVertexLayout),
		DXE_CREATIONLAYOUT);

	// Initialisation des paramètres de sampling de la texture
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Création de l'état de sampling
	pD3DDevice->CreateSamplerState(&samplerDesc, &pSampleState);
}