#include "StdAfx.h"

#include "Plane.h"
#include "../PetitMoteur3D/PetitMoteur3D/Resource.h"
#include "../PetitMoteur3D/PetitMoteur3D/Singleton.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"
#include "../PetitMoteur3D/PetitMoteur3D/util.h"

#include <d3d11.h>
#include <winnt.h>
#include <d3dcompiler.h>
#include <algorithm>

using namespace PirateSimulator;
using namespace PM3D;
using namespace DirectX;
using namespace UtilitairesDX;


// Définir l'organisation de notre sommet
D3D11_INPUT_ELEMENT_DESC CSommetPlane::layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
UINT CSommetPlane::numElements = ARRAYSIZE(CSommetPlane::layout);


CPlane::CPlane(PM3D::CDispositifD3D11* pDispositif_) :
	Mesh<ShaderCPlane::ShadersParams>(ShaderCPlane::ShadersParams())
{
	pDispositif = pDispositif_;

	index.reserve(INDEX_COUNT);

	const size_t size = (NBPOINTSX - 1) * NBPOINTSZ;

	for (size_t iter = 0; iter < size; ++iter)
	{
		if (iter%NBPOINTSX == NBPOINTSX - 1)
		{
			continue;
		}

		index.push_back(iter);
		index.push_back(iter + NBPOINTSX);
		index.push_back(iter + NBPOINTSX + 1);

		index.push_back(iter);
		index.push_back(iter + NBPOINTSX + 1);
		index.push_back(iter + 1);
	}

	unsigned int a = 0;
	unsigned int b = 0;
	for (size_t iter = 0; iter < index.size(); ++iter)
	{
		if (index[iter] > a)
		{
			b = iter;
			a = index[iter];
		}
	}


	/*TODO: CREER POINTS*/
	sommets.reserve(NBPOINTS);
	for (int i = 0;i < NBPOINTSZ;++i)
	{
		for (int j = 0;j < NBPOINTSX;++j)
		{
			XMFLOAT3 pos{ j*(DX / static_cast<float>(NBPOINTSX - 1)),CPlane::DEFAULT_Y_LEVEL_WATER_PLANE,(i*DZ / static_cast<float>(NBPOINTSZ - 1)) };
			//point.push_back(pt);
			CSommetPlane pt = CSommetPlane(pos, XMFLOAT3{ 0.f,1.f,0.f });
			sommets.push_back(pt);
		}
	}


	//point[0] = XMFLOAT3(1.0f, 1.0f, 1.0f);


	// Création du vertex buffer et copie des sommets
	ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CSommetPlane) * sommets.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = sommets.data();
	pVertexBuffer = NULL;
	DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);

	// Création de l'index buffer et copie des indices
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * index.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = index.data();
	pIndexBuffer = NULL;
	DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer), DXE_CREATIONINDEXBUFFER);

	// Initialisation de l'effet
	InitEffet();

	m_matWorld = XMMatrixIdentity();
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

void CPlane::SetTexture(CTexture* pTexture)
{
	pTextureD3D = pTexture->GetD3DTexture();
}

CPlane::~CPlane(void)
{
	DXRelacher(pEffet);
	DXRelacher(pVertexLayout);
	DXRelacher(pIndexBuffer);
	DXRelacher(pVertexBuffer);
}

void CPlane::Draw()
{
	// Obtenir le contexte
	ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

	// Choisir la topologie des primitives
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Source des sommets
	UINT stride = sizeof(CSommetPlane);
	UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	// Source des index
	pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// input layout des sommets
	pImmediateContext->IASetInputLayout(pVertexLayout);


	// Initialiser et sélectionner les «constantes» de l'effet
	XMMATRIX viewProj = CMoteurWindows::GetInstance().GetMatViewProj();

	m_shaderParameter.matWorldViewProj = XMMatrixTranspose(m_matWorld * viewProj);
	pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &m_shaderParameter, 0, 0);
	ID3DX11EffectConstantBuffer* pCB = pEffet->GetConstantBufferByName("param"); // Nous n'avons qu'un seul CBuffer
	pCB->SetConstantBuffer(pConstantBuffer);


	// Activation de la texture
	/*ID3DX11EffectShaderResourceVariable* variableTexture;
	variableTexture = pEffet->GetVariableByName("textureEntree")->AsShaderResource();
	variableTexture->SetResource(pTextureD3D);*/


	// Le sampler state
	ID3DX11EffectSamplerVariable* variableSampler;
	variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
	variableSampler->SetSampler(0, pSampleState);





	// **** Rendu de l'objet
	pPasse->Apply(0, pImmediateContext);
	pImmediateContext->DrawIndexed(index.size(), 0, 0);

}