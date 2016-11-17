#include "StdAfx.h"

#include "Terrain.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"
#include "../PetitMoteur3D/PetitMoteur3D/resource.h"
#include "../PetitMoteur3D/PetitMoteur3D/util.h"
#include "../PetitMoteur3D/PetitMoteur3D/DispositifD3D11.h"
#include "../PetitMoteur3D/PetitMoteur3D/Texture.h"
#include "../PetitMoteur3D/PetitMoteur3D/GestionnaireDeTextures.h"

#include "ShaderManager.h"

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

    Terrain::Terrain(PM3D::CDispositifD3D11* pDispositif_, int h, int w, const std::string& fieldFileName, const std::string& textureFileName)
        : m_terrainWidth(w), m_terrainHeight(h),
        Mesh<ShaderTerrain::ShadersParams>(ShaderTerrain::ShadersParams())
    {
        pDispositif = pDispositif_; // Prendre en note le dispositif

        std::vector<float> myFile = PirateSimulator::RessourcesManager::GetInstance().ReadHeightMapFile(fieldFileName);
        const int vertexLineCount = 1 + PirateSimulator::Vertex::INFO_COUNT;
        int nbPoint = vertexLineCount * m_terrainWidth * m_terrainHeight;
        for (int i = 0; i < nbPoint; i += vertexLineCount)
        {
            PirateSimulator::Vertex p{ myFile[i + 1], myFile[i + 3], myFile[i + 2], myFile[i + 4], myFile[i + 5], myFile[i + 6], myFile[i + 7], myFile[i + 8] };
            addSommet(p);
        }
        for (int i = nbPoint; i < myFile.size(); i += 3)
        {
            PirateSimulator::Triangle t{ static_cast<unsigned int>(myFile[i]), static_cast<unsigned int>(myFile[i + 1]), static_cast<unsigned int>(myFile[i + 2]) };
            addTriangle(t);
        }

        Init(textureFileName);
    }

    Terrain::Terrain(PM3D::CDispositifD3D11* pDispositif_) :
        Mesh<ShaderTerrain::ShadersParams>(ShaderTerrain::ShadersParams())
    {
        pDispositif = pDispositif_;  // Prendre en note le dispositif
    }

    Terrain::~Terrain()
    {
    }

    void Terrain::InitShaders()
    {
    }

    void Terrain::Init(const std::string& textureFileName)
    {
        ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

        // Création du vertex buffer et copie des sommets
        D3D11_BUFFER_DESC bufferDescription;
        ZeroMemory(&bufferDescription, sizeof(bufferDescription));

        bufferDescription.Usage = D3D11_USAGE_DEFAULT;
        bufferDescription.ByteWidth = sizeof(CSommetBloc) * m_sommets.size();
        bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDescription.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA vertexBufferData;
        ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
        vertexBufferData.pSysMem = m_sommets.data();
        pVertexBuffer = NULL;

        UtilitairesDX::DXEssayer(pD3DDevice->CreateBuffer(&bufferDescription, &vertexBufferData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);

        // Création de l'index buffer et copie des indices
        ZeroMemory(&bufferDescription, sizeof(bufferDescription));

        bufferDescription.Usage = D3D11_USAGE_DEFAULT;
        bufferDescription.ByteWidth = sizeof(unsigned int) * m_index_bloc.size();
        bufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufferDescription.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA indexBufferData;
        ZeroMemory(&indexBufferData, sizeof(indexBufferData));
        indexBufferData.pSysMem = m_index_bloc.data();
        pIndexBuffer = NULL;

        UtilitairesDX::DXEssayer(pD3DDevice->CreateBuffer(&bufferDescription, &indexBufferData, &pIndexBuffer),
            DXE_CREATIONINDEXBUFFER);

        m_matWorld = XMMatrixIdentity();
        rotation = 0.0f;

        // Chargement des textures
        this->loadTexture(textureFileName);
    }



    void Terrain::addSommet(PirateSimulator::Vertex v)
    {
        XMFLOAT3 pos{ v.position().x(), v.position().y(), v.position().z() };
        XMFLOAT3 nor{ v.normalVector().x(), v.normalVector().y(), v.normalVector().z() };

        XMFLOAT2 textCoord{ v.getTextureCoordinate().m_U, v.getTextureCoordinate().m_V };
        CSommetBloc c{ pos, nor, textCoord };

        if (m_arraySommets.size() == v.position().x())
        {
            m_arraySommets.push_back(vector<Vertex>{});
        }
        m_arraySommets[v.position().x()].push_back(v);
        m_sommets.push_back(c);
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


        if (z < 0 || z + 1 > m_terrainHeight || x < 0 || x + 1 > m_terrainWidth)
        {
            return 0.0f;
        }

        float myFirstX = floor(x);
        float mySecondX = ceil(x);
        float myFirstZ = floor(z);
        float mySecondZ = ceil(z);

        Vertex bottomLeft = m_arraySommets[myFirstX][myFirstZ];
        Vertex topLeft = m_arraySommets[myFirstX][mySecondZ];
        Vertex bottomRight = m_arraySommets[mySecondX][myFirstZ];
        Vertex topRight = m_arraySommets[mySecondX][mySecondZ];
        /*
        float height = 0;

        if (x + z >= 1.0f)
        {
            // Top Right triangle
            // A = BR  -  B = TR  -  C = TL
            float Ax = bottomRight.position().x(), Ay = bottomRight.position().y(), Az = bottomRight.position().z();
            float Bx = topRight.position().x(), By = topRight.position().y(), Bz = topRight.position().z();
            float Cx = topLeft.position().x(), Cy = topLeft.position().y(), Cz = topLeft.position().z();
            height = Ay + ((Bx - Ax)*(Cy - Ay) - (Cx - Ax)*(By - Ay)) / ((Bx - Ax)*(Cz - Az) - (Cx - Ax)*(Bz - Az)) * (z - Az) - ((By - Ay)*(Cz - Az) - (Cy - Ay) * (Bz - Az)) / ((Bx - Ax)*(Cz - Az) - (Cx - Ax)*(Bz - Az)) * (x - Ax);
        }
        else
        {
            // Bottom Left triangle
            // A = BL  -  B = BR  -  C = TL
            float Ax = bottomLeft.position().x(), Ay = bottomLeft.position().y(), Az = bottomLeft.position().z();
            float Bx = bottomRight.position().x(), By = bottomRight.position().y(), Bz = bottomRight.position().z();
            float Cx = topLeft.position().x(), Cy = topLeft.position().y(), Cz = topLeft.position().z();
            height = Ay + ((Bx - Ax)*(Cy - Ay) - (Cx - Ax)*(By - Ay)) / ((Bx - Ax)*(Cz - Az) - (Cx - Ax)*(Bz - Az)) * (z - Az) - ((By - Ay)*(Cz - Az) - (Cy - Ay) * (Bz - Az)) / ((Bx - Ax)*(Cz - Az) - (Cx - Ax)*(Bz - Az)) * (x - Ax);
        }

        return height;*/

        float diffLeft = topLeft.position().z() - bottomLeft.position().z();

        if (diffLeft == 0.f)
        {
            diffLeft = 1.f;
        }

        float coeff = (topLeft.position().x() - bottomLeft.position().x()) / diffLeft;

        return coeff * (x + z) + bottomLeft.position().y();
    }



    void Terrain::setTexture(PM3D::CTexture* texture)
    {
        m_material.pTextureD3D = texture->GetD3DTexture();
    }

    void Terrain::loadTexture(const std::string& filename)
    {
        PM3D::CGestionnaireDeTextures& TexturesManager = PM3D::CMoteurWindows::GetInstance().GetTextureManager();

        wstring ws;
        ws.assign(filename.begin(), filename.end());

        this->setTexture(TexturesManager.GetNewTexture(ws.c_str(), pDispositif));

        ID3D11Resource* pResource;
        ID3D11Texture2D *pTextureInterface = 0;
        m_material.pTextureD3D->GetResource(&pResource);
        pResource->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
        D3D11_TEXTURE2D_DESC desc;
        pTextureInterface->GetDesc(&desc);

        ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

        // Création d'un tampon pour les constantes du VS
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));

        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(ShaderTerrain::ShadersParams);
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.CPUAccessFlags = 0;
        HRESULT hr = pD3DDevice->CreateBuffer(&bd, NULL, &pConstantBuffer);

        m_shader = PirateSimulator::ShaderManager::GetInstance().getEffect("Miniphong");

        //// Pour l'effet
        //ID3DBlob* pFXBlob = NULL;

        //// Creation du shader
        //UtilitairesDX::DXEssayer(D3DCompileFromFile(L"MiniPhong.fx", 0, 0, 0,
        //    "fx_5_0", 0, 0, &pFXBlob, 0),
        //    DXE_ERREURCREATION_FX);

        //D3DX11CreateEffectFromMemory(pFXBlob->GetBufferPointer(), pFXBlob->GetBufferSize(), 0, pD3DDevice, &m_shader.m_effect);

        //pFXBlob->Release();

        m_shader->m_technique = m_shader->m_effect->GetTechniqueByIndex(0);
        m_shader->m_pass = m_shader->m_technique->GetPassByIndex(0);

        // Créer l'organisation des sommets pour le VS de notre effet
        D3DX11_PASS_SHADER_DESC effectVSDesc;
        m_shader->m_pass->GetVertexShaderDesc(&effectVSDesc);

        D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
        effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex, &effectVSDesc2);

        const void *vsCodePtr = effectVSDesc2.pBytecode;
        unsigned vsCodeLen = effectVSDesc2.BytecodeLength;

        pVertexLayout = NULL;

        UtilitairesDX::DXEssayer(pD3DDevice->CreateInputLayout(Terrain::layout,
            Terrain::numElements,
            vsCodePtr,
            vsCodeLen,
            &pVertexLayout),
            DXE_CREATIONLAYOUT);

        //// Initialisation des paramètres de sampling de la texture
        //D3D11_SAMPLER_DESC samplerDesc;

        //samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
        //samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        //samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        //samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        //samplerDesc.MipLODBias = 0.0f;
        //samplerDesc.MaxAnisotropy = 4;
        //samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        //samplerDesc.BorderColor[0] = 0;
        //samplerDesc.BorderColor[1] = 0;
        //samplerDesc.BorderColor[2] = 0;
        //samplerDesc.BorderColor[3] = 0;
        //samplerDesc.MinLOD = 0;
        //samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

        //// Création de l'état de sampling
        //pD3DDevice->CreateSamplerState(&samplerDesc, &m_textureEffect.m_sampleState);
    }

    void Terrain::Draw()
    {
        // Obtenir le contexte
        ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

        // Choisir la topologie des primitives
        pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // On bind les vertex avec le shader
        UINT stride = sizeof(CSommetBloc);
        UINT offset = 0;
        pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

        // On bind les index avec le shader
        pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

        // On set le layout de nos vertex
        pImmediateContext->IASetInputLayout(pVertexLayout);

        // Initialiser et sélectionner les «constantes» du VS
        XMMATRIX viewProj = PM3D::CMoteurWindows::GetInstance().GetMatViewProj();

        m_shaderParameter.matWorldViewProj = XMMatrixTranspose(m_matWorld * viewProj);
        m_shaderParameter.matWorld = XMMatrixTranspose(m_matWorld);

        //m_shaderParameter.vAMat = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
        //m_shaderParameter.vDMat = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
        m_shaderParameter.vAMat = XMLoadFloat4(&m_material.m_property.ambientValue);
        m_shaderParameter.vDMat = XMLoadFloat4(&m_material.m_property.diffuseValue);
        m_shaderParameter.vSMat = XMLoadFloat4(&m_material.m_property.specularValue);
        m_shaderParameter.puissance = m_material.m_property.power;

        m_shaderParameter.vCamera = PM3D::CMoteurWindows::GetInstance().getCamera()->m_transform.m_position;

        pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &m_shaderParameter, 0, 0);

        ID3DX11EffectConstantBuffer* pCB = m_shader->m_effect->GetConstantBufferByName("param");  // Nous n'avons qu'un seul CBuffer
        pCB->SetConstantBuffer(pConstantBuffer);

        // Activation de la texture ou non
        if (m_material.pTextureD3D != nullptr)
        {
            ID3DX11EffectShaderResourceVariable* variableTexture;
            variableTexture = m_shader->m_effect->GetVariableByName("textureEntree")->AsShaderResource();
            variableTexture->SetResource(m_material.pTextureD3D);
        }

        /**/
        // Le sampler state
        ID3DX11EffectSamplerVariable* variableSampler;
        variableSampler = m_shader->m_effect->GetVariableByName("SampleState")->AsSampler();
        variableSampler->SetSampler(0, m_shader->m_sampleState);

        // Dessiner les subsets non-transparents    
        //m_material = Material(MaterialProperties());

        // IMPORTANT pour ajuster les param.
        m_shader->m_pass->Apply(0, pImmediateContext);
    
        // **** Rendu de l'objet	   
        pImmediateContext->DrawIndexed(m_index_bloc.size(), 0, 0);
    }
}