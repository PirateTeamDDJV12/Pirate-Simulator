#include "PieceMesh.h"
#include "..\PetitMoteur3D\PetitMoteur3D\moteurWindows.h"
#include "..\PetitMoteur3D\PetitMoteur3D\util.h"
#include "..\PetitMoteur3D\PetitMoteur3D\resource.h"
#include "..\PetitMoteur3D\PetitMoteur3D\ObjetMesh.h"
#include "..\PetitMoteur3D\PetitMoteur3D\sommetbloc.h"

#include "Moves.h"

#include "RendererManager.h"
#include "CameraManager.h"
#include "LightManager.h"

#include "Light.h"

#include <string>
#include <fstream>


using namespace UtilitairesDX;


namespace PirateSimulator
{
    // Ancien constructeur
    PieceMesh::PieceMesh(const ShaderPieceMesh::ShadersParams& shaderParameter, const std::wstring& shaderName, PM3D::IChargeur& chargeur) :
        Mesh<ShaderPieceMesh::ShadersParams>(shaderParameter)
    {
        // prendre en note le dispositif
        pDispositif = PirateSimulator::RendererManager::singleton.getDispositif();

        // Placer l'objet sur la carte graphique
        TransfertObjet(chargeur);

        // Initialisation de l'effet
        InitEffet(shaderName);
    }

    // Constructeur pour lecture d'un objet de format OMB
    PieceMesh::PieceMesh(const std::string& nomFichier, const std::wstring& shaderName, const ShaderPieceMesh::ShadersParams& shaderParameter) :
        Mesh<ShaderPieceMesh::ShadersParams>(shaderParameter)
    {
        // prendre en note le dispositif
        pDispositif = PirateSimulator::RendererManager::singleton.getDispositif();

        // Placer l'objet sur la carte graphique
        LireFichierBinaire(nomFichier);

        // Initialisation de l'effet
        InitEffet(shaderName);
    }


    PieceMesh::~PieceMesh(void)
    {
        SubsetMaterialIndex.clear();
        SubsetIndex.clear();
        m_materials.clear();

        DXRelacher(m_effect.m_constantBuffer);
        DXRelacher(m_effect.m_sampleState);

        DXRelacher(m_effect.m_effect);
        DXRelacher(pVertexLayout);
        DXRelacher(pIndexBuffer);
        DXRelacher(pVertexBuffer);
    }


    void PieceMesh::InitEffet(const std::wstring& shaderName)
    {
        // Compilation et chargement du vertex shader
        ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

        // Cr�ation d'un tampon pour les constantes du VS
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));

        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(ShaderPieceMesh::ShadersParams);
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.CPUAccessFlags = 0;
        HRESULT hr = pD3DDevice->CreateBuffer(&bd, NULL, &m_effect.m_constantBuffer);

        // Pour l'effet
        ID3DBlob* pFXBlob = NULL;

        DXEssayer(D3DCompileFromFile(shaderName.c_str(), 0, 0, 0,
            "fx_5_0", 0, 0, &pFXBlob, 0),
            DXE_ERREURCREATION_FX);

        D3DX11CreateEffectFromMemory(pFXBlob->GetBufferPointer(), pFXBlob->GetBufferSize(), 0, pD3DDevice, &m_effect.m_effect);

        pFXBlob->Release();

        m_effect.m_technique = m_effect.m_effect->GetTechniqueByIndex(0);
        m_effect.m_pass = m_effect.m_technique->GetPassByIndex(0);

        // Cr�er l'organisation des sommets pour le VS de notre effet
        D3DX11_PASS_SHADER_DESC effectVSDesc;
        m_effect.m_pass->GetVertexShaderDesc(&effectVSDesc);

        D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
        effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex, &effectVSDesc2);

        const void *vsCodePtr = effectVSDesc2.pBytecode;
        unsigned vsCodeLen = effectVSDesc2.BytecodeLength;

        pVertexLayout = NULL;

        DXEssayer(pD3DDevice->CreateInputLayout(CSommetBloc::layout,
            CSommetBloc::numElements,
            vsCodePtr,
            vsCodeLen,
            &pVertexLayout),
            DXE_CREATIONLAYOUT);

        // Initialisation des param�tres de sampling de la texture
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

        // Cr�ation de l'�tat de sampling
        pD3DDevice->CreateSamplerState(&samplerDesc, &m_effect.m_sampleState);
    }


    void PieceMesh::Draw()
    {
#ifdef DEBUG_PIRATE_SIMULATOR
        //OutputDebugStringA(LPCSTR((m_gameObject->m_name + " is drawn " + to_string(PirateSimulator::debugCount++) + "\n").c_str()));
#endif

        // Obtenir le contexte
        ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

        // Choisir la topologie des primitives
        pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // input layout des sommets
        pImmediateContext->IASetInputLayout(pVertexLayout);

        // Index buffer
        pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

        // Vertex buffer
        UINT stride = sizeof(CSommetBloc);
        UINT offset = 0;
        pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);


        // Initialiser et s�lectionner les �constantes� de l'effet
        DirectX::XMMATRIX viewProj = PirateSimulator::CameraManager::singleton.getMatViewProj();

        m_shaderParameter.matWorldViewProj = DirectX::XMMatrixTranspose(m_matWorld * viewProj);
        m_shaderParameter.matWorld = DirectX::XMMatrixTranspose(m_matWorld);

        PirateSimulator::LightManager& lightManager = PirateSimulator::LightManager::singleton;
        PirateSimulator::LightRef sun = lightManager.getBrightSun();

        m_shaderParameter.vLumiere = DirectX::XMLoadFloat3(&sun->m_vector);

        // Le sampler state
        ID3DX11EffectSamplerVariable* variableSampler;
        variableSampler = m_effect.m_effect->GetVariableByName("SampleState")->AsSampler();
        variableSampler->SetSampler(0, m_effect.m_sampleState);
        DXRelacher(variableSampler);

        // Dessiner les subsets non-transparents
        for (int i = 0; i < NombreSubset; ++i)
        {
            int indexStart = SubsetIndex[i];
            int indexDrawAmount = SubsetIndex[i + 1] - indexStart;
            if (indexDrawAmount)
            {
                m_shaderParameter.vAMat = DirectX::XMLoadFloat4(&m_materials[SubsetMaterialIndex[i]].m_property.ambientValue);
                m_shaderParameter.vDMat = DirectX::XMLoadFloat4(&m_materials[SubsetMaterialIndex[i]].m_property.diffuseValue);

                // Activation de la texture ou non
                if (m_materials[SubsetMaterialIndex[i]].pTextureD3D != NULL)
                {
                    ID3DX11EffectShaderResourceVariable* variableTexture;
                    variableTexture = m_effect.m_effect->GetVariableByName("textureEntree")->AsShaderResource();
                    variableTexture->SetResource(m_materials[SubsetMaterialIndex[i]].pTextureD3D);
                    DXRelacher(variableTexture);
                }

                // IMPORTANT pour ajuster les param.
                m_effect.m_pass->Apply(0, pImmediateContext);

                ID3DX11EffectConstantBuffer* pCB = m_effect.m_effect->GetConstantBufferByName("param");  // Nous n'avons qu'un seul CBuffer
                pCB->SetConstantBuffer(m_effect.m_constantBuffer);
                pImmediateContext->UpdateSubresource(m_effect.m_constantBuffer, 0, NULL, &m_shaderParameter, 0, 0);

                pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);
                DXRelacher(pCB);
            }
        }
    }


    void PieceMesh::TransfertObjet(PM3D::IChargeur& chargeur)
    {
        // 1. SOMMETS a) Cr�ations des sommets dans un tableau temporaire    
        vector<CSommetBloc> ts;
        CSommetBloc s;
        vector<unsigned int> SommetDansMesh;
        vector<unsigned int> Indices;

        for (unsigned int iSubmesh = 0; iSubmesh < chargeur.GetNombreSubmesh(); ++iSubmesh)
        {
            SommetDansMesh.push_back(ts.size());
            for (unsigned int i = 0; i < chargeur.GetNombreSommetsSubmesh(iSubmesh); ++i)
            {
                s.position = chargeur.GetPosition(iSubmesh, i);
                s.normal = chargeur.GetNormale(iSubmesh, i);

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


        // 1. SOMMETS b) Cr�ation du vertex buffer et copie des sommets
        ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));

        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(CSommetBloc) * ts.size();
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = ts.data();
        pVertexBuffer = NULL;

        DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);


        // 2. INDEX - Cr�ation de l'index buffer et copie des indices
        //            Les indices �tant habituellement des entiers, j'ai
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

        DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer),
            DXE_CREATIONINDEXBUFFER);


        // 3. Les sous-objets
        NombreSubset = chargeur.GetNombreSubmesh();


        // 4. MATERIAUX
        // 4a) Cr�er un mat�riau de d�faut en index 0
        //     Vous pourriez changer les valeurs, j'ai conserv� 
        //     celles du constructeur
        Material m;
        m_materials.push_back(m);


        // 4b) Copie des mat�riaux dans la version locale
        for (unsigned int i = 0; i < chargeur.GetNombreMaterial(); ++i)
        {
            Material mat;

            chargeur.GetMaterial(i, mat.m_textureFileName,
                mat.m_materialName,
                mat.m_property.ambientValue,
                mat.m_property.diffuseValue,
                mat.m_property.specularValue,
                mat.m_property.power
            );

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
            if (m_materials[i].m_textureFileName != "")
            {
                wstring ws;
                ws.assign(m_materials[i].m_textureFileName.begin(), m_materials[i].m_textureFileName.end());

                m_materials[i].pTextureD3D = TexturesManager.GetNewTexture(ws.c_str())->GetD3DTexture();
            }

        }
    }


    void PieceMesh::LireFichierBinaire(const std::string& nomFichier)
    {
        ifstream fichier;
        fichier.open(nomFichier, ios::in | ios_base::binary);


        // 1. SOMMETS a) Cr�ations des sommets dans un tableau temporaire
        int nombreSommets;
        fichier.read((char*)&nombreSommets, sizeof(nombreSommets));

        CSommetBloc* ts = new CSommetBloc[nombreSommets];


        // 1. SOMMETS b) Lecture des sommets � partir d'un fichier binaire
        fichier.read((char*)ts, nombreSommets * sizeof(CSommetBloc));


        // 1. SOMMETS b) Cr�ation du vertex buffer et copie des sommets
        ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));

        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(CSommetBloc) * nombreSommets;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = ts;
        pVertexBuffer = NULL;

        DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);

        // D�truire ts, devenu inutile
        delete[] ts;


        // 2. INDEX 
        int nombreIndex;
        fichier.read((char*)&nombreIndex, sizeof(nombreIndex));

        unsigned int* index = new unsigned int[nombreIndex];
        fichier.read((char*)index, nombreIndex * sizeof(unsigned int));

        ZeroMemory(&bd, sizeof(bd));

        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(unsigned int) * nombreIndex;
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;

        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = index;
        pIndexBuffer = NULL;

        DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer),
            DXE_CREATIONINDEXBUFFER);

        // D�truire index, devenu inutile
        delete[] index;


        // 3. Les sous-objets
        fichier.read((char*)&NombreSubset, sizeof(NombreSubset));
        //    D�but de chaque sous-objet et un pour la fin
        int* si = new int[NombreSubset + 1];
        fichier.read((char*)si, (NombreSubset + 1) * sizeof(int));
        SubsetIndex.assign(si, si + (NombreSubset + 1));

        delete[] si;


        // 4. MATERIAUX
        // 4a) Cr�er un mat�riau de d�faut en index 0
        //     Vous pourriez changer les valeurs, j'ai conserv� 
        //     celles du constructeur
        Material mat;

        int NbMaterial;
        fichier.read((char*)&NbMaterial, sizeof(int));

        m_materials.resize(NbMaterial);

        PM3D::CObjetMesh::MaterialBlock mb;
        for (int i = 0; i < NbMaterial; ++i)
        {
            fichier.read((char*)&mb, sizeof(PM3D::CObjetMesh::MaterialBlock));

            m_materials[i].m_textureFileName.append(mb.NomFichierTexture);
            m_materials[i].m_materialName.append(mb.NomMateriau);
            m_materials[i].m_property.ambientValue = mb.Ambient;
            m_materials[i].m_property.diffuseValue = mb.Diffuse;
            m_materials[i].m_property.specularValue = mb.Specular;
            m_materials[i].m_property.power = mb.Puissance;
            m_materials[i].m_property.transparency = mb.transparent;
        }


        // 4c) Trouver l'index du materiau pour chaque sous-ensemble
        int* smi = new int[NombreSubset];
        fichier.read((char*)smi, (NombreSubset) * sizeof(int));
        SubsetMaterialIndex.assign(smi, smi + NombreSubset);
        delete[] smi;


        // 4d) Chargement des textures
        PM3D::CGestionnaireDeTextures& TexturesManager = PM3D::CMoteurWindows::GetInstance().GetTextureManager();

        for (unsigned int i = 0; i < m_materials.size(); ++i)
        {
            if (m_materials[i].m_textureFileName != "")
            {
                wstring ws;
                ws.assign(m_materials[i].m_textureFileName.begin(), m_materials[i].m_textureFileName.end());

                m_materials[i].pTextureD3D = TexturesManager.GetNewTexture(ws.c_str())->GetD3DTexture();
            }
        }
        fichier.close();
    }

}
