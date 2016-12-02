#include "Terrain.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"
#include "../PetitMoteur3D/PetitMoteur3D/resource.h"
#include "../PetitMoteur3D/PetitMoteur3D/util.h"
#include "../PetitMoteur3D/PetitMoteur3D/DispositifD3D11.h"
#include "../PetitMoteur3D/PetitMoteur3D/Texture.h"
#include "../PetitMoteur3D/PetitMoteur3D/GestionnaireDeTextures.h"
#include "../PetitMoteur3D/PetitMoteur3D/Config/Config.hpp"
#include "RessourceManager.h"
#include "RendererManager.h"
#include "LightManager.h"

using namespace DirectX;

namespace PirateSimulator
{
    D3D11_INPUT_ELEMENT_DESC Terrain::layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    UINT Terrain::numElements = ARRAYSIZE(Terrain::layout);

    Terrain::Terrain()
        : Mesh<ShaderTerrain::ShadersParams>(ShaderTerrain::ShadersParams()),
        pPixelShader{nullptr}, pVertexBuffer{nullptr}, pVertexLayout{nullptr}, pVertexShader{nullptr}, pIndexBuffer{nullptr}, pConstantBuffer{nullptr}
    {
        // Get the configuration from the config file
        Config* terrainConfig = Config::getInstance();
        m_terrainWidth = terrainConfig->getWidth();
        m_terrainHeight = terrainConfig->getHeight();
        m_terrainScale = terrainConfig->getMapScale();

        pDispositif = RendererManager::singleton.getDispositif(); // Prendre en note le dispositif

        m_vertexArray.reserve(m_terrainWidth * m_terrainHeight);
        m_csommetsArray.reserve(m_terrainWidth * m_terrainHeight);
        std::vector<float> myFile = RessourcesManager::GetInstance().ReadHeightMapFile(terrainConfig->getExportName());
        int nbPoint = Vertex::INFO_COUNT * m_terrainWidth * m_terrainHeight;
        for(int i = 0; i < nbPoint; i += Vertex::INFO_COUNT)
        {
            PirateSimulator::Vertex p{myFile[i], myFile[i + 2], myFile[i + 1], myFile[i + 3], myFile[i + 4], myFile[i + 5], myFile[i + 6], myFile[i + 7]};
            addSommet(p);
        }
        for(int i = nbPoint; i < myFile.size(); i += 3)
        {
            PirateSimulator::Triangle t{static_cast<unsigned int>(myFile[i]), static_cast<unsigned int>(myFile[i + 1]), static_cast<unsigned int>(myFile[i + 2])};
            addTriangle(t);
        }

        Init(terrainConfig->getTexturePath());
    }

    Terrain::Terrain(int h, int w, float s, const std::string& fieldFileName, const std::string& textureFileName)
        : m_terrainWidth{w}, m_terrainHeight{h}, m_terrainScale{s},
        Mesh<ShaderTerrain::ShadersParams>(ShaderTerrain::ShadersParams()),
        pPixelShader{nullptr}, pVertexBuffer{nullptr}, pVertexLayout{nullptr}, pVertexShader{nullptr}, pIndexBuffer{nullptr}, pConstantBuffer{nullptr}
    {
        pDispositif = RendererManager::singleton.getDispositif(); // Prendre en note le dispositif

        m_vertexArray.reserve(m_terrainWidth * m_terrainHeight);
        m_csommetsArray.reserve(m_terrainWidth * m_terrainHeight);

        std::vector<float> myFile = PirateSimulator::RessourcesManager::GetInstance().ReadHeightMapFile(fieldFileName);
        const int vertexLineCount = 1 + PirateSimulator::Vertex::INFO_COUNT;
        int nbPoint = vertexLineCount * m_terrainWidth * m_terrainHeight;
        for(int i = 0; i < nbPoint; i += vertexLineCount)
        {
            PirateSimulator::Vertex p{myFile[i + 1], myFile[i + 3], myFile[i + 2], myFile[i + 4], myFile[i + 5], myFile[i + 6], myFile[i + 7], myFile[i + 8]};
            addSommet(p);
        }
        for(int i = nbPoint; i < myFile.size(); i += 3)
        {
            PirateSimulator::Triangle t{static_cast<unsigned int>(myFile[i]), static_cast<unsigned int>(myFile[i + 1]), static_cast<unsigned int>(myFile[i + 2])};
            addTriangle(t);
        }

        Init(textureFileName);
    }

    Terrain::~Terrain()
    {
        using UtilitairesDX::DXRelacher;

        DXRelacher(pVertexBuffer);
        DXRelacher(pIndexBuffer);

        DXRelacher(pVertexShader);
        DXRelacher(pPixelShader);
        DXRelacher(pVertexLayout);
        DXRelacher(pConstantBuffer);
    }

    void Terrain::Anime(float tempsEcoule)
    {}

    void Terrain::Draw()
    {

        // Obtenir le contexte
        ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

        // Choisir la topologie des primitives
        pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Source des sommets
        UINT stride = sizeof(CSommetBloc);
        UINT offset = 0;
        pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

        // Source des index
        pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

        // Activer le VS
        pImmediateContext->VSSetShader(pVertexShader, NULL, 0);

        // input layout des sommets
        pImmediateContext->IASetInputLayout(pVertexLayout);

        // Initialiser et sélectionner les «constantes» du VS
        XMMATRIX viewProj = PirateSimulator::CameraManager::singleton.getMatViewProj();

        m_shaderParameter.matWorldViewProj = XMMatrixTranspose(m_matWorld * viewProj);
        m_shaderParameter.matWorld = XMMatrixTranspose(m_matWorld);

        m_shaderParameter.vAMat = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
        m_shaderParameter.vDMat = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);

        m_shaderParameter.vCamera = PirateSimulator::CameraManager::singleton.getMainCameraGO()->m_transform.m_position;

        pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &m_shaderParameter, 0, 0);

        pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);

        // Pas de Geometry Shader
        pImmediateContext->GSSetShader(NULL, NULL, 0);

        // Activer le PS
        pImmediateContext->PSSetShader(pPixelShader, NULL, 0);
        pImmediateContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

        // Dessiner les subsets non-transparents    
        //m_material = Material(MaterialProperties());

        m_shaderParameter.vAMat = XMLoadFloat4(&m_material.m_property.ambientValue);
        m_shaderParameter.vDMat = XMLoadFloat4(&m_material.m_property.diffuseValue);
        m_shaderParameter.vSMat = XMLoadFloat4(&m_material.m_property.specularValue);
        m_shaderParameter.puissance = m_material.m_property.power;

        // IMPORTANT pour ajuster les param.
        m_textureEffect.m_pass->Apply(0, pImmediateContext);

        ID3DX11EffectConstantBuffer* pCB = m_textureEffect.m_effect->GetConstantBufferByName("param");  // Nous n'avons qu'un seul CBuffer
        pCB->SetConstantBuffer(pConstantBuffer);
        pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &m_shaderParameter, 0, 0);

        UtilitairesDX::DXRelacher(pCB);

        // **** Rendu de l'objet	   
        pImmediateContext->DrawIndexed(m_index_bloc.size(), 0, 0);
    }

    void Terrain::Init(const std::string& textureFileName)
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

        UtilitairesDX::DXRelacher(pD3DDevice);
    }

    void Terrain::addSommet(PirateSimulator::Vertex v)
    {
        XMFLOAT3 pos{v.position().x(), v.position().y(), v.position().z()};
        XMFLOAT3 nor{v.normalVector().x(), v.normalVector().y(), v.normalVector().z()};

        XMFLOAT2 textCoord{v.getTextureCoordinate().m_U, v.getTextureCoordinate().m_V};
        CSommetBloc c{pos, nor, textCoord};

        m_vertexArray.push_back(v);
        m_csommetsArray.push_back(c);
    }

    void Terrain::addTriangle(PirateSimulator::Triangle t)
    {
        m_index_bloc.push_back(t.firstPointIndex());
        m_index_bloc.push_back(t.secondPointIndex());
        m_index_bloc.push_back(t.thirdPointIndex());
    }

    float Terrain::getHeight(XMVECTOR pos)
    {
        float x = pos.vector4_f32[0], z = pos.vector4_f32[2];


        if(z < 0 || z + 1 > m_terrainHeight * m_terrainScale || x < 0 || x + 1 > m_terrainWidth * m_terrainScale)
        {
            return 0.0f;
        }

        float myFirstX = UtilitairesDX::roundNum(x, m_terrainScale, false) / m_terrainScale;
        float myFirstZ = UtilitairesDX::roundNum(z, m_terrainScale, false) / m_terrainScale;
        float mySecondZ = UtilitairesDX::roundNum(z, m_terrainScale, true) / m_terrainScale;

        Vertex bottomLeft = m_vertexArray[myFirstX + myFirstZ * m_terrainWidth];
        Vertex topLeft = m_vertexArray[myFirstX + mySecondZ * m_terrainWidth];

        float diffLeft = topLeft.position().z() - bottomLeft.position().z();

        if(diffLeft == 0.f)
        {
            diffLeft = 1.f;
        }

        float coeff = (topLeft.position().x() - bottomLeft.position().x()) / diffLeft;

        return coeff * (x + z) + bottomLeft.position().y();
    }

    void Terrain::InitShaders()
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


    void Terrain::setTexture(PM3D::CTexture* texture)
    {
        UtilitairesDX::DXRelacher(m_material.pTextureD3D);
        m_material.pTextureD3D = texture->GetD3DTexture();
    }

    void Terrain::loadTexture(const std::string& filename)
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
        if(m_material.pTextureD3D != nullptr)
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
}