#include "StdAfx.h"

#include "Skybox.h"
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

// Définir l'organisation de notre sommet
D3D11_INPUT_ELEMENT_DESC CSommetSky::layout[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
UINT CSommetSky::numElements = ARRAYSIZE(CSommetSky::layout);

CSkybox::CSkybox(CDispositifD3D11* pDispositif_)
{
    pDispositif = pDispositif_;

    XMFLOAT3 point[8];
    point[0] = XMFLOAT3(1.0f, 1.0f, 1.0f);
    point[1] = XMFLOAT3(-1.0f, 1.0f, 1.0f);
    point[2] = XMFLOAT3(-1.0f, -1.0f, 1.0f);
    point[3] = XMFLOAT3(1.0f, -1.0f, 1.0f);
    point[4] = XMFLOAT3(-1.0f, 1.0f, -1.0f);
    point[5] = XMFLOAT3(1.0f, 1.0f, -1.0f);
    point[6] = XMFLOAT3(1.0f, -1.0f, -1.0f);
    point[7] = XMFLOAT3(-1.0f, -1.0f, -1.0f);
    CSommetSky sommets[24];
    float m = 0.00001f;

    // Le devant du bloc
    sommets[0] = CSommetSky(point[0], XMFLOAT2(0.25f, 1.0f / 3.0f));
    sommets[1] = CSommetSky(point[1], XMFLOAT2(0.50f, 1.0f / 3.0f));
    sommets[2] = CSommetSky(point[2], XMFLOAT2(0.50f, 2.0f / 3.0f));
    sommets[3] = CSommetSky(point[3], XMFLOAT2(0.25f, 2.0f / 3.0f));

    // L'arrière du bloc
    sommets[4] = CSommetSky(point[4], XMFLOAT2(0.75f, 1.0f / 3.0f));
    sommets[5] = CSommetSky(point[5], XMFLOAT2(1.00f, 1.0f / 3.0f));
    sommets[6] = CSommetSky(point[6], XMFLOAT2(1.00f, 2.0f / 3.0f));
    sommets[7] = CSommetSky(point[7], XMFLOAT2(0.75f, 2.0f / 3.0f));

    // Le dessous du bloc
    sommets[8] = CSommetSky(point[3], XMFLOAT2(0.25f, 2.0f / 3.0f));
    sommets[9] = CSommetSky(point[2], XMFLOAT2(0.50f, 2.0f / 3.0f));
    sommets[10] = CSommetSky(point[7], XMFLOAT2(0.50f, 1.00f));
    sommets[11] = CSommetSky(point[6], XMFLOAT2(0.25f, 1.00f));

    // Le dessus du bloc
    sommets[12] = CSommetSky(point[5], XMFLOAT2(0.25f, 0.00f));
    sommets[13] = CSommetSky(point[4], XMFLOAT2(0.50f, 0.00f));
    sommets[14] = CSommetSky(point[1], XMFLOAT2(0.50f, 1.0f / 3.0f));
    sommets[15] = CSommetSky(point[0], XMFLOAT2(0.25f, 1.0f / 3.0f));

    // La face gauche
    sommets[16] = CSommetSky(point[5], XMFLOAT2(0.00f, 1.0f / 3.0f));
    sommets[17] = CSommetSky(point[0], XMFLOAT2(0.25f, 1.0f / 3.0f));
    sommets[18] = CSommetSky(point[3], XMFLOAT2(0.25f, 2.0f / 3.0f));
    sommets[19] = CSommetSky(point[6], XMFLOAT2(0.00f, 2.0f / 3.0f));

    // La face droite
    sommets[20] = CSommetSky(point[1], XMFLOAT2(0.50f, 1.0f / 3.0f));
    sommets[21] = CSommetSky(point[4], XMFLOAT2(0.75f, 1.0f / 3.0f));
    sommets[22] = CSommetSky(point[7], XMFLOAT2(0.75f, 2.0f / 3.0f));
    sommets[23] = CSommetSky(point[2], XMFLOAT2(0.50f, 2.0f / 3.0f));

    // Création du vertex buffer et copie des sommets
    ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(CSommetSky) * 24;
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
    bd.ByteWidth = sizeof(unsigned int) * 36;
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

void CSkybox::InitEffet()
{
    // Compilation et chargement du vertex shader
    ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

    // Création d'un tampon pour les constantes du VS
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ShadersParams);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    pD3DDevice->CreateBuffer(&bd, NULL, &pConstantBuffer);

    // Pour l'effet
    ID3DBlob* pFXBlob = NULL;
    DXEssayer(D3DCompileFromFile(L"MiniPhongSky.fx", 0, 0, 0,
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
    
    DXEssayer(pD3DDevice->CreateInputLayout(CSommetSky::layout,
        CSommetSky::numElements,
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

void CSkybox::SetTexture(CTexture* pTexture)
{
    pTextureD3D = pTexture->GetD3DTexture();
}

CSkybox::~CSkybox(void)
{
    DXRelacher(pEffet);
    DXRelacher(pVertexLayout);
    DXRelacher(pIndexBuffer);
    DXRelacher(pVertexBuffer);
}

void CSkybox::Draw()
{
    // Obtenir le contexte
    ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

    // Choisir la topologie des primitives
    pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Source des sommets
    UINT stride = sizeof(CSommetSky);
    UINT offset = 0;
    pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

    // Source des index
    pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // input layout des sommets
    pImmediateContext->IASetInputLayout(pVertexLayout);

    // Initialiser et sélectionner les «constantes» de l'effet
    ShadersParams sp;
    XMVECTOR v = CMoteurWindows::GetInstance().GetCameraPosition();
    XMMATRIX matPos = XMMatrixTranslation(v.vector4_f32[0], v.vector4_f32[1], v.vector4_f32[2]);
    XMMATRIX viewProj = CMoteurWindows::GetInstance().GetMatViewProj();
    sp.matWorldViewProj = XMMatrixTranspose(matPos * viewProj);
    pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &sp, 0, 0);
    ID3DX11EffectConstantBuffer* pCB = pEffet->GetConstantBufferByName("param"); // Nous n'avons qu'un seul CBuffer
        pCB->SetConstantBuffer(pConstantBuffer);

    // Activation de la texture
    ID3DX11EffectShaderResourceVariable* variableTexture;
    variableTexture = pEffet->GetVariableByName("textureEntree")->AsShaderResource();
    variableTexture->SetResource(pTextureD3D);

    // Le sampler state
    ID3DX11EffectSamplerVariable* variableSampler;
    variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
    variableSampler->SetSampler(0, pSampleState);

    // Désactiver Culling et ZBuffer
    pDispositif->DesactiverCulling();
    pDispositif->DesactiverZBuffer();

    // **** Rendu de l'objet
    pPasse->Apply(0, pImmediateContext);
    pImmediateContext->DrawIndexed(36, 0, 0);

    // Réactiver Culling et ZBuffer
    pDispositif->ActiverCulling();
    pDispositif->ActiverZBuffer();
}
