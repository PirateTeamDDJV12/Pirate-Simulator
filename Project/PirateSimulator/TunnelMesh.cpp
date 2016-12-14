#include "TunnelMesh.h"

#include "CameraManager.h"
#include "LightManager.h"
#include "RendererManager.h"

#include "..\PetitMoteur3D\PetitMoteur3D\util.h"
#include "..\PetitMoteur3D\PetitMoteur3D\sommetbloc.h"
#include "..\PetitMoteur3D\PetitMoteur3D\ObjetMesh.h"
#include "..\PetitMoteur3D\PetitMoteur3D\MoteurWindows.h"
#include "..\PetitMoteur3D\PetitMoteur3D\Resource.h"
#include "Light.h"

#include <string>
#include <d3dcompiler.h>
#include <winnt.h>


using namespace PirateSimulator;

TunnelMesh::TunnelMesh( const ShaderTunnel::ShadersParams& shaderParameter,
                const std::wstring& shaderName,
                PM3D::IChargeur& chargeur) :
    Mesh<ShaderTunnel::ShadersParams>(shaderParameter),
    pDispositif{ RendererManager::singleton.getDispositif() }
{
    TransfertObjet(chargeur);

    InitEffet(shaderName);

    InitShaderParameter();
}

void TunnelMesh::TransfertObjet(PM3D::IChargeur& chargeur)
{
    // 1. SOMMETS a) Créations des sommets dans un tableau temporaire    
    vector<PM3D::CObjetMesh::CSommetMesh> ts;
    PM3D::CObjetMesh::CSommetMesh s;
    vector<unsigned int> SommetDansMesh;
    vector<unsigned int> Indices;

    for (unsigned int iSubmesh = 0; iSubmesh < chargeur.GetNombreSubmesh(); ++iSubmesh)
    {
        SommetDansMesh.push_back(ts.size());
        for (unsigned int i = 0; i < chargeur.GetNombreSommetsSubmesh(iSubmesh); ++i)
        {
            s.position = chargeur.GetPosition(iSubmesh, i);
            s.normal = chargeur.GetNormale(iSubmesh, i);
            s.coordTex = chargeur.GetCoordTex(iSubmesh, i);

            ts.push_back(s);
        }
    }

    for (unsigned int iSubmesh = 0; iSubmesh < chargeur.GetNombreSubmesh(); ++iSubmesh)
    {
        SubsetIndex.push_back(Indices.size());
        for (unsigned int i = 0; i < chargeur.GetNombrePolygonesSubmesh(iSubmesh); ++i)
        {
            for (unsigned int j = 0; j < 3; ++j)
            {
                unsigned int index = chargeur.GetIndice(iSubmesh, i, j);
                Indices.push_back(SommetDansMesh[iSubmesh] + index);
            }
        }
    }
    SubsetIndex.push_back(Indices.size());


    // 1. SOMMETS b) Création du vertex buffer et copie des sommets
    ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(PM3D::CObjetMesh::CSommetMesh) * ts.size();
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = ts.data();
    pVertexBuffer = NULL;

    UtilitairesDX::DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);


    // 2. INDEX - Création de l'index buffer et copie des indices
    //            Les indices étant habituellement des entiers, j'ai
    //            pris directement ceux du chargeur mais attention au 
    //            format si vous avez autre chose que DXGI_FORMAT_R32_UINT
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(unsigned int) * Indices.size();
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = Indices.data();
    pIndexBuffer = NULL;

    UtilitairesDX::DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer),
        DXE_CREATIONINDEXBUFFER);


    // 3. Les sous-objets
    NombreSubset = chargeur.GetNombreSubmesh();


    // 4. MATERIAUX
    // 4a) Créer un matériau de défaut en index 0
    //     Vous pourriez changer les valeurs, j'ai conservé 
    //     celles du constructeur
    PM3D::CObjetMesh::CMaterial m;
    m_materials.push_back(m);


    // 4b) Copie des matériaux dans la version locale
    for (unsigned int i = 0; i < chargeur.GetNombreMaterial(); ++i)
    {
        PM3D::CObjetMesh::CMaterial mat;

        chargeur.GetMaterial(i, mat.NomFichierTexture,
            mat.NomMateriau,
            mat.Ambient,
            mat.Diffuse,
            mat.Specular,
            mat.Puissance);

        m_materials.push_back(mat);
    }


    // 4c) Trouver l'index du materiau pour chaque sous-ensemble (+1 car 0 est default)
    for (int i = 0; i < chargeur.GetNombreSubmesh(); ++i)
    {
        SubsetMaterialIndex.push_back(chargeur.GetMaterialIndex(i) + 1);
    }


    // 4d) Chargement des textures
    PM3D::CGestionnaireDeTextures& TexturesManager = PM3D::CMoteurWindows::GetInstance().GetTextureManager();

    for (unsigned int i = 0; i < m_materials.size(); ++i)
    {
        if (m_materials[i].NomFichierTexture != "")
        {
            wstring ws;
            ws.assign(m_materials[i].NomFichierTexture.begin(), m_materials[i].NomFichierTexture.end());

            m_materials[i].pTextureD3D = TexturesManager.GetNewTexture(ws.c_str())->GetD3DTexture();
        }
    }
}

void TunnelMesh::InitEffet(const std::wstring& shaderName)
{
    // Compilation et chargement du vertex shader
    ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

    // Création d'un tampon pour les constantes du VS
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ShaderTunnel::ShadersParams);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    HRESULT hr = pD3DDevice->CreateBuffer(&bd, NULL, &m_textureEffect.m_constantBuffer);

    // Pour l'effet
    ID3DBlob* pFXBlob = NULL;

    UtilitairesDX::DXEssayer(D3DCompileFromFile(shaderName.c_str(), 0, 0, 0,
        "fx_5_0", 0, 0, &pFXBlob, 0),
        DXE_ERREURCREATION_FX);

    D3DX11CreateEffectFromMemory(pFXBlob->GetBufferPointer(), pFXBlob->GetBufferSize(), 0, pD3DDevice, &m_textureEffect.m_effect);

    pFXBlob->Release();

    m_textureEffect.m_technique = m_textureEffect.m_effect->GetTechniqueByIndex(0);
    m_textureEffect.m_pass = m_textureEffect.m_technique->GetPassByIndex(0);

    // Créer l'organisation des sommets pour le VS de notre effet
    D3DX11_PASS_SHADER_DESC effectVSDesc;
    m_textureEffect.m_pass->GetVertexShaderDesc(&effectVSDesc);

    D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
    effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex, &effectVSDesc2);

    const void *vsCodePtr = effectVSDesc2.pBytecode;
    unsigned vsCodeLen = effectVSDesc2.BytecodeLength;

    pVertexLayout = NULL;

    UtilitairesDX::DXEssayer(pD3DDevice->CreateInputLayout(PM3D::CObjetMesh::CSommetMesh::layout,
        PM3D::CObjetMesh::CSommetMesh::numElements,
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
}

void TunnelMesh::Draw()
{
#ifdef DEBUG_PIRATE_SIMULATOR
    //OutputDebugStringA(LPCSTR((m_gameObject->m_name + " is drawn " + to_string(PirateSimulator::debugCount++) + "\n").c_str()));
#endif

    // Obtenir le contexte
    ID3D11DeviceContext* pImmediateContext = RendererManager::singleton.getDispositif()->GetImmediateContext();

    // Choisir la topologie des primitives
    pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // input layout des sommets
    pImmediateContext->IASetInputLayout(pVertexLayout);

    // Index buffer
    pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Vertex buffer
    UINT stride = sizeof(PM3D::CObjetMesh::CSommetMesh);
    UINT offset = 0;
    pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

    //Update the shader parameters with camera position and light position
    updateShaderParameter();


    // Le sampler state
    ID3DX11EffectSamplerVariable* variableSampler;
    variableSampler = m_textureEffect.m_effect->GetVariableByName("SampleState")->AsSampler();
    variableSampler->SetSampler(0, m_textureEffect.m_sampleState);
    UtilitairesDX::DXRelacher(variableSampler);

    // Désactiver Culling
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
            //m_shaderParameter.puissance = m_materials[SubsetMaterialIndex[i]].Puissance;

            // Activation de la texture ou non
            if (m_materials[SubsetMaterialIndex[i]].pTextureD3D != NULL)
            {
                ID3DX11EffectShaderResourceVariable* variableTexture;
                variableTexture = m_textureEffect.m_effect->GetVariableByName("textureEntree")->AsShaderResource();
                variableTexture->SetResource(m_materials[SubsetMaterialIndex[i]].pTextureD3D);

                UtilitairesDX::DXRelacher(variableTexture);
            }

            // IMPORTANT pour ajuster les param.
            m_textureEffect.m_pass->Apply(0, pImmediateContext);

            ID3DX11EffectConstantBuffer* pCB = m_textureEffect.m_effect->GetConstantBufferByName("param");  // Nous n'avons qu'un seul CBuffer
            pCB->SetConstantBuffer(m_textureEffect.m_constantBuffer);
            pImmediateContext->UpdateSubresource(m_textureEffect.m_constantBuffer, 0, NULL, &m_shaderParameter, 0, 0);

            pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);
            UtilitairesDX::DXRelacher(pCB);
        }
    }

    // Réactiver Culling
    pDispositif->ActiverCulling();
}

void TunnelMesh::InitShaderParameter()
{
    m_shaderParameter.vSMat = { 0.12f, 0.12f, 0.12f, 1.f };

    LightManager& lightManager = LightManager::singleton;

    m_shaderParameter.sunPower = lightManager.getBrightSun()->m_power;

    auto& lightArray = lightManager.getBrightPointsLights();

    if (lightArray.size() > 7)
    {
        m_shaderParameter.vLightPoint1 = XMLoadFloat3(&lightArray[0]->m_vector);
        m_shaderParameter.vLightPoint2 = XMLoadFloat3(&lightArray[1]->m_vector);
        m_shaderParameter.vLightPoint3 = XMLoadFloat3(&lightArray[2]->m_vector);
        m_shaderParameter.vLightPoint4 = XMLoadFloat3(&lightArray[3]->m_vector);

        m_shaderParameter.mappedLightPointPower1.vector4_f32[0] = lightArray[0]->m_scope;
        m_shaderParameter.mappedLightPointPower1.vector4_f32[1] = lightArray[1]->m_scope;
        m_shaderParameter.mappedLightPointPower1.vector4_f32[2] = lightArray[2]->m_scope;
        m_shaderParameter.mappedLightPointPower1.vector4_f32[3] = lightArray[3]->m_scope;

        m_shaderParameter.vLightPoint5 = XMLoadFloat3(&lightArray[4]->m_vector);
        m_shaderParameter.vLightPoint6 = XMLoadFloat3(&lightArray[5]->m_vector);
        m_shaderParameter.vLightPoint7 = XMLoadFloat3(&lightArray[6]->m_vector);
        m_shaderParameter.vLightPoint8 = XMLoadFloat3(&lightArray[7]->m_vector);

        m_shaderParameter.mappedLightPointPower2.vector4_f32[0] = lightArray[4]->m_scope;
        m_shaderParameter.mappedLightPointPower2.vector4_f32[1] = lightArray[5]->m_scope;
        m_shaderParameter.mappedLightPointPower2.vector4_f32[2] = lightArray[6]->m_scope;
        m_shaderParameter.mappedLightPointPower2.vector4_f32[3] = lightArray[7]->m_scope;
    }
}

void TunnelMesh::updateShaderParameter()
{
    CameraManager& mainCamera = CameraManager::singleton;
    LightManager& lightManager = LightManager::singleton;

    //Camera position
    XMMATRIX viewProj = mainCamera.getMatViewProj();

    m_shaderParameter.matWorldViewProj = XMMatrixTranspose(m_matWorld * viewProj);
    m_shaderParameter.matWorld = XMMatrixTranspose(m_matWorld);

    m_shaderParameter.vCamera = mainCamera.getMainCameraGO()->m_transform.getPosition();

    //Light
    m_shaderParameter.vLumiere = DirectX::XMLoadFloat3(&lightManager.getBrightSun()->m_vector);

    float ambientLightVal = lightManager.getAmbientLightCoefficient();

    m_shaderParameter.vAEcl.vector4_f32[0] = ambientLightVal;
    m_shaderParameter.vAEcl.vector4_f32[1] = ambientLightVal;
    m_shaderParameter.vAEcl.vector4_f32[2] = ambientLightVal;
}