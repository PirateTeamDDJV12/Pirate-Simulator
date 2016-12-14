#include "Skybox.h"
#include "../PetitMoteur3D/PetitMoteur3D/Resource.h"
#include "../PetitMoteur3D/PetitMoteur3D/Singleton.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"
#include "../PetitMoteur3D/PetitMoteur3D/util.h"
#include "LightManager.h"
#include "CameraManager.h"

#include <d3d11.h>
#include <winnt.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

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

CSkybox::CSkybox() :
    Mesh<ShaderCSkyBox::ShadersParams>(ShaderCSkyBox::ShadersParams()),
    pTextureD3D{ nullptr }, pIndexBuffer{ nullptr }, pVertexBuffer{ nullptr }, m_effect{}
{
    pDispositif = PirateSimulator::RendererManager::singleton.getDispositif();

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

    DXRelacher(pD3DDevice);
}

void CSkybox::InitEffet()
{
    // Compilation et chargement du vertex shader
    ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

    // Création d'un tampon pour les constantes du VS
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ShaderCSkyBox::ShadersParams);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;

    DXRelacher(m_effect.m_constantBuffer);
    pD3DDevice->CreateBuffer(&bd, NULL, &m_effect.m_constantBuffer);

    // Pour l'effet
    ID3DBlob* pFXBlob = NULL;
    DXEssayer(D3DCompileFromFile(L"MiniPhongSky.fx", 0, 0, 0,
        "fx_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0,
        &pFXBlob, NULL),
        DXE_ERREURCREATION_FX);

    DXRelacher(m_effect.m_effect);
    D3DX11CreateEffectFromMemory(pFXBlob->GetBufferPointer(), pFXBlob->GetBufferSize(), 0, pD3DDevice, &m_effect.m_effect);
    pFXBlob->Release();

    DXRelacher(m_effect.m_technique);
    m_effect.m_technique = m_effect.m_effect->GetTechniqueByIndex(0);

    DXRelacher(m_effect.m_pass);
    m_effect.m_pass = m_effect.m_technique->GetPassByIndex(0);

    // Créer l'organisation des sommets pour le VS de notre effet
    D3DX11_PASS_SHADER_DESC effectVSDesc;
    m_effect.m_pass->GetVertexShaderDesc(&effectVSDesc);
    D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
    effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex, &effectVSDesc2);
    const void *vsCodePtr = effectVSDesc2.pBytecode;
    unsigned int vsCodeLen = effectVSDesc2.BytecodeLength;
    
    DXRelacher(m_effect.m_vertexLayout);
    DXEssayer(pD3DDevice->CreateInputLayout(CSommetSky::layout,
        CSommetSky::numElements,
        vsCodePtr,
        vsCodeLen,
        &m_effect.m_vertexLayout),
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
    DXRelacher(m_effect.m_sampleState);
    pD3DDevice->CreateSamplerState(&samplerDesc, &m_effect.m_sampleState);

    DXRelacher(pD3DDevice);
}

void CSkybox::setTexture(const std::wstring& fileName)
{
    PM3D::CGestionnaireDeTextures& TexturesManager = PM3D::CMoteurWindows::GetInstance().GetTextureManager();
    auto texture = TexturesManager.GetNewTexture(fileName.c_str());

    if (texture)
    {
        DXRelacher(pTextureD3D);
        pTextureD3D = texture->GetD3DTexture();
    }
}

CSkybox::~CSkybox(void)
{
    DXRelacher(pTextureD3D);
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
    pImmediateContext->IASetInputLayout(m_effect.m_vertexLayout);


    // Initialiser et sélectionner les «constantes» de l'effet
    XMVECTOR cameraPos = PirateSimulator::CameraManager::singleton.getMainCameraGO()->m_transform.getPosition();

    m_matWorld = XMMatrixTranslation(cameraPos.vector4_f32[0], cameraPos.vector4_f32[1], cameraPos.vector4_f32[2]);
    XMMATRIX viewProj = PirateSimulator::CameraManager::singleton.getMatViewProj();

    m_shaderParameter.matWorldViewProj = XMMatrixTranspose(m_matWorld * viewProj);

    this->setSunAngleCoeff(LightManager::singleton.getSunAngle());

    pImmediateContext->UpdateSubresource(m_effect.m_constantBuffer, 0, NULL, &m_shaderParameter, 0, 0);
    ID3DX11EffectConstantBuffer* pCB = m_effect.m_effect->GetConstantBufferByName("param"); // Nous n'avons qu'un seul CBuffer
    pCB->SetConstantBuffer(m_effect.m_constantBuffer);
    DXRelacher(pCB);

    // Activation de la texture
    ID3DX11EffectShaderResourceVariable* variableTexture;
    variableTexture = m_effect.m_effect->GetVariableByName("textureEntree")->AsShaderResource();
    variableTexture->SetResource(pTextureD3D);
    DXRelacher(variableTexture);

    // Le sampler state
    ID3DX11EffectSamplerVariable* variableSampler;
    variableSampler = m_effect.m_effect->GetVariableByName("SampleState")->AsSampler();
    variableSampler->SetSampler(0, m_effect.m_sampleState);
    DXRelacher(variableSampler);

    // Désactiver Culling et ZBuffer
    pDispositif->DesactiverCulling();
    pDispositif->DesactiverZBuffer();


    // **** Rendu de l'objet
    m_effect.m_pass->Apply(0, pImmediateContext);
    pImmediateContext->DrawIndexed(36, 0, 0);


    // Réactiver Culling et ZBuffer
    pDispositif->ActiverCulling();
    pDispositif->ActiverZBuffer();
}

void CSkybox::setSunAngleCoeff(float sunAngle)
{
    //Do not change those values unless you know what you're doing. (obtained with some twiking)
    const float MIN_ANGLE_SIN = XM_PI / 32.f;
    const float ZENITH_FRONTIER = XM_PI / 6.f;
    const float SUN_RISE_FRONTIER = 11 * XM_PI / 6.f;

    if (sunAngle > (7.f * XM_PI / 6.f) && sunAngle < SUN_RISE_FRONTIER) // night time
    {
        float sinus = sinf(MIN_ANGLE_SIN);

        m_shaderParameter.sunCoefficient.x = sinus;
        m_shaderParameter.sunCoefficient.y = sinus;
        m_shaderParameter.sunCoefficient.z = sinus;
    }
    else if (sunAngle >= SUN_RISE_FRONTIER || sunAngle < ZENITH_FRONTIER) // rising sun
    {
        float sinus = sinf((sunAngle - SUN_RISE_FRONTIER) + MIN_ANGLE_SIN);

        m_shaderParameter.sunCoefficient.x = sinus;
        m_shaderParameter.sunCoefficient.y = sinus;
        m_shaderParameter.sunCoefficient.z = sinus;
    }
    else if (sunAngle >= ZENITH_FRONTIER && sunAngle < XM_PIDIV2) // morning zenith
    {
        float sinus = sinf(XM_PIDIV2 - (sunAngle / 3.65f));

        m_shaderParameter.sunCoefficient.x = sinus;
        m_shaderParameter.sunCoefficient.y = sinus;
        m_shaderParameter.sunCoefficient.z = sinus;
    }
    else if (sunAngle >= XM_PIDIV2 && sunAngle < (XM_PI * 5.f / 6.f)) // afternoon zenith
    {
        float sinus = sinf(XM_PIDIV2 + (sunAngle / 5.f));

        m_shaderParameter.sunCoefficient.x = sinus;
        m_shaderParameter.sunCoefficient.y = sinus;
        m_shaderParameter.sunCoefficient.z = sinus;
    }
    else // twilight
    {
        float sinus = sinf((29.f / 32.f * (sunAngle - 5 * ZENITH_FRONTIER)) + (XM_2PI / 3.f)); //LERP
        float opposedSinus = (1.f - sinus);

        m_shaderParameter.sunCoefficient.x = sinus + opposedSinus * opposedSinus * sinus * sinus;
        m_shaderParameter.sunCoefficient.y = sinus;
        m_shaderParameter.sunCoefficient.z = sinus;
    }
}