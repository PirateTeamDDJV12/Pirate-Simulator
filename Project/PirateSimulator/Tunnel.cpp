/*
#include "Tunnel.h"

#include "CameraManager.h"
#include "LightManager.h"
#include "RendererManager.h"

#include "..\PetitMoteur3D\PetitMoteur3D\util.h"
#include "..\PetitMoteur3D\PetitMoteur3D\sommetbloc.h"
#include "..\PetitMoteur3D\PetitMoteur3D\ObjetMesh.h"
#include <d3dcompiler.h>
#include <d3dcompiler.h>
#include "..\PetitMoteur3D\PetitMoteur3D\MoteurWindows.h"
#include <string>
#include "Light.h"


using namespace PirateSimulator;


Tunnel::Tunnel(const PM3D::ShaderCObjectMesh::ShadersParams& shaderParameter,
               const std::wstring& shaderName, 
               PM3D::IChargeur& chargeur, 
               const std::string& textureFileName) : 
    PM3D::CObjetMesh(shaderParameter, shaderName, chargeur)
{
    Init(textureFileName);
}

void Tunnel::Init(const std::string& textureFileName)
{
    // Création du vertex buffer et copie des sommets
    ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(CSommetBloc) * m_csommetsArray.size();
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = m_csommetsArray.data();

    UtilitairesDX::DXRelacher(pVertexBuffer);
    UtilitairesDX::DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);

    // Création de l'index buffer et copie des indices
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(unsigned int) * m_index_bloc.size();
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = m_index_bloc.data();

    UtilitairesDX::DXRelacher(pIndexBuffer);
    UtilitairesDX::DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer),
        DXE_CREATIONINDEXBUFFER);

    // Initialisation des shaders
    InitShaders();

    m_matWorld = XMMatrixIdentity();

    // Chargement des textures
    this->loadTexture(textureFileName);

    LightRef sun = LightManager::singleton.getBrightSun();

    m_shaderParameter.vLumiere = DirectX::XMLoadFloat3(&sun->m_vector);
    m_shaderParameter.sunPower = sun->m_power;

    m_shaderParameter.vAMat = XMLoadFloat4(&m_material.m_property.ambientValue);
    m_shaderParameter.vDMat = XMLoadFloat4(&m_material.m_property.diffuseValue);
    m_shaderParameter.vSMat = XMLoadFloat4(&m_material.m_property.specularValue);
    m_shaderParameter.puissance = m_material.m_property.power;

    UtilitairesDX::DXRelacher(pD3DDevice);
}

void Tunnel::InitShaders()
{
    // Compilation et chargement du vertex shader
    ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

    ID3DBlob* pVSBlob = NULL;
    UtilitairesDX::DXEssayer(D3DCompileFromFile(L"MiniPhong.vhl",
        NULL, NULL,
        "MiniPhongVS",
        "vs_4_0",
        D3DCOMPILE_ENABLE_STRICTNESS,
        0,
        &pVSBlob, NULL), DXE_FICHIER_VS);

    UtilitairesDX::DXRelacher(pVertexShader);
    UtilitairesDX::DXEssayer(pD3DDevice->CreateVertexShader(pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(),
        NULL,
        &pVertexShader),
        DXE_CREATION_VS);

    // Créer l'organisation des sommets
    UtilitairesDX::DXRelacher(pVertexLayout);
    UtilitairesDX::DXEssayer(pD3DDevice->CreateInputLayout(Terrain::layout,
        Terrain::numElements,
        pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(),
        &pVertexLayout),
        DXE_CREATIONLAYOUT);

    pVSBlob->Release(); //  On n'a plus besoin du blob

                        // Création d'un tampon pour les constantes du VS
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ShaderTerrain::ShadersParams);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    pD3DDevice->CreateBuffer(&bd, NULL, &pConstantBuffer);

    // Compilation et chargement du pixel shader
    ID3DBlob* pPSBlob = NULL;
    UtilitairesDX::DXEssayer(D3DCompileFromFile(L"MiniPhong.phl",
        NULL, NULL,
        "MiniPhongPS",
        "ps_4_0",
        D3DCOMPILE_ENABLE_STRICTNESS,
        0,
        &pPSBlob,
        NULL), DXE_FICHIER_PS);

    UtilitairesDX::DXRelacher(pPixelShader);

    UtilitairesDX::DXEssayer(pD3DDevice->CreatePixelShader(pPSBlob->GetBufferPointer(),
        pPSBlob->GetBufferSize(),
        NULL,
        &pPixelShader),
        DXE_CREATION_PS);

    UtilitairesDX::DXRelacher(pD3DDevice);
    pPSBlob->Release(); //  On n'a plus besoin du blob
}

void Tunnel::loadTexture(const std::string& filename)
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
    bd.ByteWidth = sizeof(ShaderTerrain::ShadersParams);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;

    UtilitairesDX::DXRelacher(pConstantBuffer);
    HRESULT hr = pD3DDevice->CreateBuffer(&bd, NULL, &pConstantBuffer);

    // Pour l'effet
    ID3DBlob* pFXBlob = NULL;

    UtilitairesDX::DXEssayer(D3DCompileFromFile(L"MiniPhongField.fx", 0, 0, 0,
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

    UtilitairesDX::DXEssayer(pD3DDevice->CreateInputLayout(Terrain::layout,
        Terrain::numElements,
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

    // Activation de la texture ou non
    if (m_material.pTextureD3D != nullptr)
    {
        ID3DX11EffectShaderResourceVariable* variableTexture;
        variableTexture = m_textureEffect.m_effect->GetVariableByName("textureEntree")->AsShaderResource();
        variableTexture->SetResource(m_material.pTextureD3D);

        UtilitairesDX::DXRelacher(variableTexture);
    }

    // Création de l'état de sampling
    UtilitairesDX::DXRelacher(m_textureEffect.m_sampleState);
    pD3DDevice->CreateSamplerState(&samplerDesc, &m_textureEffect.m_sampleState);

    //Texture
    ID3DX11EffectSamplerVariable* variableSampler;
    variableSampler = m_textureEffect.m_effect->GetVariableByName("SampleState")->AsSampler();
    variableSampler->SetSampler(0, m_textureEffect.m_sampleState);
    UtilitairesDX::DXRelacher(variableSampler);

    UtilitairesDX::DXRelacher(pTextureInterface);
    UtilitairesDX::DXRelacher(pResource);
    UtilitairesDX::DXRelacher(pD3DDevice);
}

void Tunnel::setTexture(PM3D::CTexture* texture)
{
    UtilitairesDX::DXRelacher(m_material.pTextureD3D);
    m_material.pTextureD3D = texture->GetD3DTexture();
}

void Tunnel::Draw()
{
    // Obtenir le contexte
    ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

    // Choisir la topologie des primitives
    pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // input layout des sommets
    pImmediateContext->IASetInputLayout(pVertexLayout);

    // Activer le VS
    pImmediateContext->VSSetShader(pVertexShader, NULL, 0);

    // Activer le PS
    pImmediateContext->PSSetShader(pPixelShader, NULL, 0);

    // Index buffer
    pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Vertex buffer
    UINT stride = sizeof(CSommetMesh);
    UINT offset = 0;
    pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

    // Initialiser et sélectionner les «constantes» de l'effet
    XMMATRIX viewProj = PirateSimulator::CameraManager::singleton.getMatViewProj();

    m_shaderParameter.matWorldViewProj = XMMatrixTranspose(m_matWorld * viewProj);
    m_shaderParameter.matWorld = XMMatrixTranspose(m_matWorld);

    // Le sampler state
    ID3DX11EffectSamplerVariable* variableSampler;
    variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
    variableSampler->SetSampler(0, pSampleState);
    UtilitairesDX::DXRelacher(variableSampler);

    m_shaderParameter.matWorldViewProj = XMMatrixTranspose(m_matWorld * viewProj);
    m_shaderParameter.matWorld = XMMatrixTranspose(m_matWorld);

    m_shaderParameter.vCamera = PirateSimulator::CameraManager::singleton.getMainCameraGO()->m_transform.m_position;

    LightManager& lightManager = LightManager::singleton;
    m_shaderParameter.vLumiere = DirectX::XMLoadFloat3(&lightManager.getBrightSun()->m_vector);

    float ambientLightVal = lightManager.getAmbientLightCoefficient();

    m_shaderParameter.vAEcl.vector4_f32[0] = ambientLightVal;
    m_shaderParameter.vAEcl.vector4_f32[1] = ambientLightVal;
    m_shaderParameter.vAEcl.vector4_f32[2] = ambientLightVal;


    // Désactiver Culling et ZBuffer
    pDispositif->DesactiverCulling();

    // Dessiner les subsets non-transparents
    for (int i = 0; i < NombreSubset; ++i)
    {
        int indexStart = SubsetIndex[i];
        int indexDrawAmount = SubsetIndex[i + 1] - indexStart;
        if (indexDrawAmount)
        {
            m_shaderParameter.vAMat = XMLoadFloat4(&m_materials[SubsetMaterialIndex[i]].Ambient);
            m_shaderParameter.vDMat = XMLoadFloat4(&m_materials[SubsetMaterialIndex[i]].Diffuse);
            m_shaderParameter.vSMat = XMLoadFloat4(&m_materials[SubsetMaterialIndex[i]].Specular);
            m_shaderParameter.puissance = m_materials[SubsetMaterialIndex[i]].Puissance;

            // Activation de la texture ou non
            if (m_materials[SubsetMaterialIndex[i]].pTextureD3D != NULL)
            {
                ID3DX11EffectShaderResourceVariable* variableTexture;
                variableTexture = pEffet->GetVariableByName("textureEntree")->AsShaderResource();
                variableTexture->SetResource(m_materials[SubsetMaterialIndex[i]].pTextureD3D);
                UtilitairesDX::DXRelacher(variableTexture);
            }

            // IMPORTANT pour ajuster les param.
            pPasse->Apply(0, pImmediateContext);

            ID3DX11EffectConstantBuffer* pCB = pEffet->GetConstantBufferByName("param");  // Nous n'avons qu'un seul CBuffer
            pCB->SetConstantBuffer(pConstantBuffer);
            pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &m_shaderParameter, 0, 0);
            UtilitairesDX::DXRelacher(pCB);

            pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);
            UtilitairesDX::DXRelacher(pCB);
        }
    }

    // Réactiver Culling et ZBuffer
    pDispositif->ActiverCulling();
}
*/
