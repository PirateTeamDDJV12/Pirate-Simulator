#include "Plane.h"
#include "../PetitMoteur3D/PetitMoteur3D/Resource.h"
#include "../PetitMoteur3D/PetitMoteur3D/Singleton.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"
#include "../PetitMoteur3D/PetitMoteur3D/util.h"

#include <d3d11.h>
#include <winnt.h>
#include <d3dcompiler.h>
#include <algorithm>
#include "TimeManager.h"
#include <chrono>
#include <comdef.h>
#include "CameraManager.h"
#include "../PetitMoteur3D/PetitMoteur3D/Config/Config.hpp"

using namespace PirateSimulator;
using namespace PM3D;
using namespace DirectX;
using namespace UtilitairesDX;


// Définir l'organisation de notre sommet
D3D11_INPUT_ELEMENT_DESC SommetPlane::layout[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
    { "TEXCOORD", 1, DXGI_FORMAT_R32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
UINT SommetPlane::numElements = ARRAYSIZE(SommetPlane::layout);


Plane::Plane(const std::string& textureFileName) :
    Mesh<ShaderPlane::ShadersParams>(ShaderPlane::ShadersParams()),
    pPixelShader{ nullptr }, pVertexBuffer{ nullptr }, pVertexLayout{ nullptr }, pVertexShader{ nullptr }, pIndexBuffer{ nullptr }, pConstantBuffer{ nullptr }
{
    pDispositif = RendererManager::singleton.getDispositif();

    float xWidth = Config::getInstance()->getWidth() * Config::getInstance()->getMapScale();
    float zHeight = Config::getInstance()->getHeight() * Config::getInstance()->getMapScale();

    m_index.reserve(INDEX_COUNT);

    constexpr const size_t size = (POINTS_X_COUNT - 1) * POINTS_Z_COUNT;

    for (size_t iter = 0; iter < size; ++iter)
    {
        if (iter % POINTS_X_COUNT == LAST_X_POINT_INDEX)
        {
            continue;
        }

        m_index.push_back(iter);
        m_index.push_back(iter + POINTS_X_COUNT);
        m_index.push_back(iter + POINTS_X_COUNT + 1);
        
        m_index.push_back(iter);
        m_index.push_back(iter + POINTS_X_COUNT + 1);
        m_index.push_back(iter + 1);
    }

    {
        m_sommets.reserve(NBPOINTS);

        XMFLOAT3 intermediaryPosition{ 0.f, Plane::DEFAULT_Y_LEVEL_WATER_PLANE, 0.f };
        XMFLOAT3 defaultNormals{ 0.f, 1.f, 0.f };

        float columnValue;
        float rowValue;
        const float XCoefficientPosition = xWidth / static_cast<float>(LAST_X_POINT_INDEX);
        const float ZCoefficientPosition = zHeight / static_cast<float>(LAST_Z_POINT_INDEX);

        for (int row = 0; row < POINTS_Z_COUNT; ++row)
        {
            for (int col = 0; col < POINTS_X_COUNT; ++col)
            {
                columnValue = static_cast<float>(col);
                rowValue = static_cast<float>(row);

                intermediaryPosition.x = X_MIN + columnValue * XCoefficientPosition;
                intermediaryPosition.z = Z_MIN + rowValue * ZCoefficientPosition;

                m_sommets.emplace_back(
                    intermediaryPosition, 
                    defaultNormals, 
                    XMFLOAT2{ columnValue, rowValue },
                    (columnValue + rowValue) / (XCoefficientPosition + ZCoefficientPosition)
                );
            }
        }
    }
    

    
    // Création du vertex buffer et copie des sommets
    ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SommetPlane) * m_sommets.size();
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = m_sommets.data();
    DXRelacher(pVertexBuffer);
    DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);

    // Création de l'index buffer et copie des indices
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(unsigned int) * m_index.size();
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = m_index.data();
    DXRelacher(pIndexBuffer);
    DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer), DXE_CREATIONINDEXBUFFER);

    // Initialisation de l'effet
    
    m_matWorld = XMMatrixIdentity();

    this->loadTexture(textureFileName);


    //InitSin();

    
    /*ID3DX11EffectShaderResourceVariable* sinTexture;
    sinTexture = m_textureEffect.m_effect->GetVariableByName("sinValue")->AsShaderResource();
    sinTexture->SetResource(pSinTex);*/


    ID3DX11EffectSamplerVariable* variableSampler;
    variableSampler = m_textureEffect.m_effect->GetVariableByName("SampleState")->AsSampler();
    variableSampler->SetSampler(0, m_textureEffect.m_sampleState);

    /*ID3DX11EffectSamplerVariable* sinSampler;
    sinSampler = m_textureEffect.m_effect->GetVariableByName("sinState")->AsSampler();
    sinSampler->SetSampler(0, pSinSampler);*/

    m_shaderParameter.waveAmplitude = Plane::WAVE_AMPLITUDE;
    m_shaderParameter.waveFrequency = Plane::WAVE_FREQUENCY;

    m_shaderParameter.vAMat = XMLoadFloat4(&m_material.m_property.ambientValue);
    m_shaderParameter.vDMat = XMLoadFloat4(&m_material.m_property.diffuseValue);
    m_shaderParameter.vSMat = { 0.12f, 0.12f, 0.12f, 1.f };
    m_shaderParameter.vSEcl = { 0.8f, 0.8f, 0.8f, 0.8f };

    // Activation de la texture ou non
    if (m_material.pTextureD3D != nullptr)
    {
        ID3DX11EffectShaderResourceVariable* variableTexture;
        variableTexture = m_textureEffect.m_effect->GetVariableByName("textureEntree")->AsShaderResource();
        variableTexture->SetResource(m_material.pTextureD3D);

        UtilitairesDX::DXRelacher(variableTexture);
    }

    UtilitairesDX::DXRelacher(variableSampler);
    UtilitairesDX::DXRelacher(pD3DDevice);
}


void Plane::SetTexture(CTexture* pTexture)
{
    DXRelacher(m_material.pTextureD3D);
    m_material.pTextureD3D = pTexture->GetD3DTexture();
}

Plane::~Plane(void)
{
    DXRelacher(m_textureEffect.m_effect);
    DXRelacher(m_textureEffect.m_constantBuffer);
    DXRelacher(m_textureEffect.m_pass);
    DXRelacher(m_textureEffect.m_sampleState);
    DXRelacher(m_textureEffect.m_sampleState);
    DXRelacher(pVertexLayout);
    DXRelacher(pIndexBuffer);
    DXRelacher(pVertexBuffer);

    DXRelacher(pVertexShader);
    DXRelacher(pPixelShader);

    DXRelacher(pConstantBuffer);
}

void Plane::Draw()
{
    pDispositif->DesactiverCulling();

    // Obtenir le contexte
    ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

    // Choisir la topologie des primitives
    pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Source des sommets
    UINT stride = sizeof(SommetPlane);
    UINT offset = 0;
    pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

    // Source des index
    pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // input layout des sommets
    pImmediateContext->IASetInputLayout(pVertexLayout);


    // Initialiser et sélectionner les «constantes» de l'effet
    XMMATRIX viewProj = CameraManager::singleton.getMatViewProj();

    m_shaderParameter.matWorldViewProj = XMMatrixTranspose(m_matWorld * viewProj);
    m_shaderParameter.matWorld = m_matWorld;
    m_shaderParameter.vCamera = CameraManager::singleton.getMainCameraGO()->m_transform.m_position;


    // Dessiner les subsets non-transparents    
    //m_material = Material(MaterialProperties());

    m_shaderParameter.puissance = m_material.m_property.power;
    m_shaderParameter.tick += TICK_INCREMENT;

    float waterVelocity = DirectX::XMScalarCos(m_shaderParameter.tick) * Plane::WAVE_SPEED_COEFFICIENT;
    m_shaderParameter.undertow = { waterVelocity , waterVelocity };


    // IMPORTANT pour ajuster les param.
    m_textureEffect.m_pass->Apply(0, pImmediateContext);


    ID3DX11EffectConstantBuffer* pCB = m_textureEffect.m_effect->GetConstantBufferByName("param");  // Nous n'avons qu'un seul CBuffer
    pCB->SetConstantBuffer(pConstantBuffer);
    pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &m_shaderParameter, 0, 0);


    // **** Rendu de l'objet	   
    pImmediateContext->DrawIndexed(m_index.size(), 0, 0);

    pDispositif->ActiverCulling();
    DXRelacher(pCB);
}

void Plane::loadTexture(const std::string& filename)
{
    PM3D::CGestionnaireDeTextures& TexturesManager = PM3D::CMoteurWindows::GetInstance().GetTextureManager();

    wstring ws;
    ws.assign(filename.begin(), filename.end());

    this->setTexture(TexturesManager.GetNewTexture(ws.c_str()));

    ID3D11Resource* pResource;
    ID3D11Texture2D *pTextureInterface = 0;
    m_material.pTextureD3D->GetResource(&pResource);
    pResource->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
    D3D11_TEXTURE2D_DESC desc;
    pTextureInterface->GetDesc(&desc);

    // Compilation et chargement du vertex shader
    ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

    // Création d'un tampon pour les constantes du VS
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ShaderPlane::ShadersParams);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    HRESULT hr = pD3DDevice->CreateBuffer(&bd, NULL, &pConstantBuffer);

    // Pour l'effet
    ID3DBlob* pFXBlob = NULL;

    UtilitairesDX::DXEssayer(D3DCompileFromFile(L"MiniPhongWater.fx", 0, 0, 0,
        "fx_5_0", 0, 0, &pFXBlob, 0),
        DXE_ERREURCREATION_FX);

    UtilitairesDX::DXRelacher(m_textureEffect.m_effect);
    D3DX11CreateEffectFromMemory(pFXBlob->GetBufferPointer(), pFXBlob->GetBufferSize(), 0, pD3DDevice, &m_textureEffect.m_effect);

    pFXBlob->Release();

    UtilitairesDX::DXRelacher(m_textureEffect.m_technique);
    m_textureEffect.m_technique = m_textureEffect.m_effect->GetTechniqueByIndex(0);
    UtilitairesDX::DXRelacher(m_textureEffect.m_pass);
    m_textureEffect.m_pass = m_textureEffect.m_technique->GetPassByIndex(0);

    // Créer l'organisation des sommets pour le VS de notre effet
    D3DX11_PASS_SHADER_DESC effectVSDesc;
    m_textureEffect.m_pass->GetVertexShaderDesc(&effectVSDesc);

    D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
    effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex, &effectVSDesc2);

    const void *vsCodePtr = effectVSDesc2.pBytecode;
    unsigned vsCodeLen = effectVSDesc2.BytecodeLength;

    UtilitairesDX::DXRelacher(pVertexLayout);
    UtilitairesDX::DXEssayer(pD3DDevice->CreateInputLayout(SommetPlane::layout,
        SommetPlane::numElements,
        vsCodePtr,
        vsCodeLen,
        &pVertexLayout),
        DXE_CREATIONLAYOUT);

    // Initialisation des paramètres de sampling de la texture
    D3D11_SAMPLER_DESC samplerDesc;

    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 4;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Création de l'état de sampling
    pD3DDevice->CreateSamplerState(&samplerDesc, &m_textureEffect.m_sampleState);

    UtilitairesDX::DXRelacher(pResource);
    UtilitairesDX::DXRelacher(pTextureInterface);
    UtilitairesDX::DXRelacher(pD3DDevice);
}


//void Plane::InitSin()
//{
//    // Création d'une texture de même dimension sur la carte graphique
//    D3D11_TEXTURE1D_DESC texDesc;
//    texDesc.Width = SIN_ARRAY_ELEMENTS_COUNT;
//    texDesc.MipLevels = 1;
//    texDesc.ArraySize = 1;
//    texDesc.Format = DXGI_FORMAT_R32_FLOAT;
//    texDesc.Usage = D3D11_USAGE_DEFAULT;
//    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//    texDesc.CPUAccessFlags = 0;
//    texDesc.MiscFlags = 0;
//    
//
//    // Create the sinus Array;
//    const float angleCoefficient = XM_2PI / static_cast<float>(SIN_ARRAY_ELEMENTS_COUNT);
//
//    for (int iter = 0; iter < SIN_ARRAY_ELEMENTS_COUNT; ++iter)
//    {
//        m_sinArray[iter] = sinf(static_cast<float>(iter) * angleCoefficient);
//    }
//
//    //Put the sin array into the GPU Memory
//    D3D11_SUBRESOURCE_DATA data;
//    data.pSysMem = m_sinArray;
//    data.SysMemPitch = SIN_ARRAY_ELEMENTS_COUNT * sizeof(float);
//    data.SysMemSlicePitch = 0;
//
//
//
//    /*pSinTex->QueryInterface<ID3D11Texture1D>(&pSinText1D);
//    pSinText1D->GetDesc(&texDesc);*/
//
//    //// Création de la texture à partir des données du bitmap
//    ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();
//    HRESULT hr = pD3DDevice->CreateTexture1D(&texDesc, &data, &pSinText1D);
//
//    /*ID3D11Resource* pResource;
//    pSinTex->GetResource(&pResource);
//    pResource->QueryInterface<ID3D11Texture1D>(&pSinText1D);*/
//    pSinText1D->GetDesc(&texDesc);
//    pSinText1D->QueryInterface<ID3D11ShaderResourceView>(&pSinTex);
//
//
//    // Initialisation des paramètres de sampling de la texture
//    D3D11_SAMPLER_DESC samplerDesc;
//
//    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
//    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//    samplerDesc.MipLODBias = 0.0f;
//    samplerDesc.MaxAnisotropy = 4;
//    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
//    samplerDesc.BorderColor[0] = 0;
//    samplerDesc.BorderColor[1] = 0;
//    samplerDesc.BorderColor[2] = 0;
//    samplerDesc.BorderColor[3] = 0;
//    samplerDesc.MinLOD = 0;
//    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
//
//    // Création de l'état de sampling
//    pD3DDevice->CreateSamplerState(&samplerDesc, &pSinSampler);
//}