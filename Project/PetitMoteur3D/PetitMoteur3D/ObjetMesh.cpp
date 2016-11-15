#include "StdAfx.h"

#include <string>

#include "ObjetMesh.h"
#include "moteurWindows.h"
#include "util.h"
#include "resource.h"

#include "../../PirateSimulator/Moves.h"

#include <fstream>
#include <corecrt_io.h>

using namespace UtilitairesDX;

namespace PM3D
{

    // Definir l'organisation de notre sommet
    D3D11_INPUT_ELEMENT_DESC CObjetMesh::CSommetMesh::layout[] =
    {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    UINT CObjetMesh::CSommetMesh::numElements;


    //void CObjetMesh::ConvertToOMB(string totalFileNameIn, const string& totalFileNameOut)
    //{
    //    if (totalFileNameIn != totalFileNameOut)
    //    {
    //        if (access(totalFileNameIn.c_str(), 0) != -1)
    //        {
    //            CParametresChargement param;

    //            param.bInverserCulling = false;
    //            param.bMainGauche = true;
    //            param.NomFichier = totalFileNameIn;

    //            CChargeurOBJ chargeur;
    //            chargeur.Chargement(param);

    //            EcrireFichierBinaire(chargeur, totalFileNameOut);
    //        }
    //    }        
    //}
    

    // Ancien constructeur
    CObjetMesh::CObjetMesh(const ShaderCObjectMesh::ShadersParams& shaderParameter, IChargeur& chargeur, CDispositifD3D11* _pDispositif) :
        Mesh<ShaderCObjectMesh::ShadersParams>( shaderParameter )
    {
        // prendre en note le dispositif
        pDispositif = _pDispositif;

        // Placer l'objet sur la carte graphique
        TransfertObjet(chargeur);

        // Initialisation de l'effet
        InitEffet();
    }

    // Constructeur de conversion
    // Constructeur pour test ou pour création d'un objet de format OMB
    CObjetMesh::CObjetMesh(string nomFichier, const ShaderCObjectMesh::ShadersParams& shaderParameter, IChargeur& chargeur, CDispositifD3D11* _pDispositif) :
        PirateSimulator::Mesh<ShaderCObjectMesh::ShadersParams>(shaderParameter)
    {
        // prendre en note le dispositif
        pDispositif = _pDispositif;


        //// Placer l'objet sur la carte graphique
        // TransfertObjet(chargeur); // On n'utilisera plus cette fonction

        EcrireFichierBinaire(chargeur, nomFichier);

        LireFichierBinaire(nomFichier);

        // Initialisation de l'effet
        InitEffet();
    }

    // Constructeur pour lecture d'un objet de format OMB
    CObjetMesh::CObjetMesh(string nomFichier, const ShaderCObjectMesh::ShadersParams& shaderParameter, CDispositifD3D11* _pDispositif) :
        PirateSimulator::Mesh<ShaderCObjectMesh::ShadersParams>(shaderParameter)
    {
        // prendre en note le dispositif
        pDispositif = _pDispositif;

        // Placer l'objet sur la carte graphique
        LireFichierBinaire(nomFichier);

        // Initialisation de l'effet
        InitEffet();
    }

    CObjetMesh::~CObjetMesh(void)
    {
        SubsetMaterialIndex.clear();
        SubsetIndex.clear();
        m_materials.clear();

        DXRelacher(pConstantBuffer);
        DXRelacher(pSampleState);

        DXRelacher(pEffet);
        DXRelacher(pVertexLayout);
        DXRelacher(pIndexBuffer);
        DXRelacher(pVertexBuffer);
    }

    void CObjetMesh::InitEffet()
    {
        // Compilation et chargement du vertex shader
        ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

        // Création d'un tampon pour les constantes du VS
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));

        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(ShaderCObjectMesh::ShadersParams);
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.CPUAccessFlags = 0;
        HRESULT hr = pD3DDevice->CreateBuffer(&bd, NULL, &pConstantBuffer);

        // Pour l'effet
        ID3DBlob* pFXBlob = NULL;

        DXEssayer(D3DCompileFromFile(L"MiniPhong.fx", 0, 0, 0,
                                     "fx_5_0", 0, 0, &pFXBlob, 0),
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
        unsigned vsCodeLen = effectVSDesc2.BytecodeLength;

        pVertexLayout = NULL;

        CSommetMesh::numElements = ARRAYSIZE(CSommetMesh::layout);
        DXEssayer(pD3DDevice->CreateInputLayout(CSommetMesh::layout,
                                                CSommetMesh::numElements,
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
        pD3DDevice->CreateSamplerState(&samplerDesc, &pSampleState);
    }
    
    void CObjetMesh::Draw()
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
        UINT stride = sizeof(CSommetMesh);
        UINT offset = 0;
        pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);


        // Initialiser et sélectionner les «constantes» de l'effet
        XMMATRIX viewProj = CMoteurWindows::GetInstance().GetMatViewProj();

        m_shaderParameter.matWorldViewProj = XMMatrixTranspose(m_matWorld * viewProj);
        m_shaderParameter.matWorld = XMMatrixTranspose(m_matWorld);


        // Le sampler state
        ID3DX11EffectSamplerVariable* variableSampler;
        variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
        variableSampler->SetSampler(0, pSampleState);


        // Dessiner les subsets non-transparents
        for(int i = 0; i < NombreSubset; ++i)
        {
            int indexStart = SubsetIndex[i];
            int indexDrawAmount = SubsetIndex[i + 1] - indexStart;
            if(indexDrawAmount)
            {
                m_shaderParameter.vAMat = XMLoadFloat4(&m_materials[SubsetMaterialIndex[i]].Ambient);
                m_shaderParameter.vDMat = XMLoadFloat4(&m_materials[SubsetMaterialIndex[i]].Diffuse);
                m_shaderParameter.vSMat = XMLoadFloat4(&m_materials[SubsetMaterialIndex[i]].Specular);
                m_shaderParameter.puissance = m_materials[SubsetMaterialIndex[i]].Puissance;

                // Activation de la texture ou non
                if(m_materials[SubsetMaterialIndex[i]].pTextureD3D != NULL)
                {
                    ID3DX11EffectShaderResourceVariable* variableTexture;
                    variableTexture = pEffet->GetVariableByName("textureEntree")->AsShaderResource();
                    variableTexture->SetResource(m_materials[SubsetMaterialIndex[i]].pTextureD3D);
                }

                // IMPORTANT pour ajuster les param.
                pPasse->Apply(0, pImmediateContext);

                ID3DX11EffectConstantBuffer* pCB = pEffet->GetConstantBufferByName("param");  // Nous n'avons qu'un seul CBuffer
                pCB->SetConstantBuffer(pConstantBuffer);
                pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &m_shaderParameter, 0, 0);

                pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);
            }
        }
    }

    void CObjetMesh::TransfertObjet(IChargeur& chargeur)
    {
        // 1. SOMMETS a) Créations des sommets dans un tableau temporaire
        unsigned int nombreSommets = chargeur.GetNombreSommets();
        CSommetMesh* ts = new CSommetMesh[nombreSommets];

        for(unsigned int i = 0; i < nombreSommets; ++i)
        {
            ts[i].position = chargeur.GetPosition(i);
            ts[i].normal = chargeur.GetNormale(i);
            ts[i].coordTex = chargeur.GetCoordTex(i);
        }

        // 1. SOMMETS b) Création du vertex buffer et copie des sommets
        ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));

        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(CSommetMesh) * nombreSommets;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = ts;
        pVertexBuffer = NULL;

        DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);

        // Détruire ts, devenu inutile
        delete[] ts;

        // 2. INDEX - Création de l'index buffer et copie des indices
        //            Les indices étant habituellement des entiers, j'ai
        //            pris directement ceux du chargeur mais attention au 
        //            format si vous avez autre chose que DXGI_FORMAT_R32_UINT
        ZeroMemory(&bd, sizeof(bd));

        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(unsigned int) * chargeur.GetNombreIndex();
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;

        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = chargeur.GetIndexData();
        pIndexBuffer = NULL;

        DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer),
                  DXE_CREATIONINDEXBUFFER);

        // 3. Les sous-objets
        NombreSubset = chargeur.GetNombreSubset();

        //    Début de chaque sous-objet et un pour la fin
        chargeur.CopieSubsetIndex(SubsetIndex);

        // 4. MATERIAUX
        // 4a) Créer un matériau de défaut en index 0
        //     Vous pourriez changer les valeurs, j'ai conservé 
        //     celles du constructeur
        CMaterial m;
        m_materials.push_back(m);

        // 4b) Copie des matériaux dans la version locale
        for(int i = 0; i < chargeur.GetNombreMaterial(); ++i)
        {
            CMaterial mat;

            chargeur.GetMaterial(i, mat.NomFichierTexture,
                                 mat.NomMateriau,
                                 mat.Ambient,
                                 mat.Diffuse,
                                 mat.Specular,
                                 mat.Puissance);

            m_materials.push_back(mat);
        }

        // 4c) Trouver l'index du materiau pour chaque sous-ensemble
        for(int i = 0; i < chargeur.GetNombreSubset(); ++i)
        {
            int index;
            for(index = 0; index < m_materials.size(); ++index)
            {
                if(m_materials[index].NomMateriau == chargeur.GetMaterialName(i)) break;
            }

            if(index >= m_materials.size()) index = 0;  // valeur de défaut

            SubsetMaterialIndex.push_back(index);
        }


        // 4d) Chargement des textures
        CGestionnaireDeTextures& TexturesManager = CMoteurWindows::GetInstance().GetTextureManager();

        for(unsigned int i = 0; i < m_materials.size(); ++i)
        {
            if(m_materials[i].NomFichierTexture != "")
            {
                wstring ws;
                ws.assign(m_materials[i].NomFichierTexture.begin(), m_materials[i].NomFichierTexture.end());

                m_materials[i].pTextureD3D = TexturesManager.GetNewTexture(ws.c_str(), pDispositif)->GetD3DTexture();
            }

        }
    }

    void CObjetMesh::EcrireFichierBinaire(IChargeur& chargeur, const string& nomFichier)
    {
        // 1. SOMMETS a) Créations des sommets dans un tableau temporaire
        unsigned int nombreSommets = chargeur.GetNombreSommets();
        CSommetMesh* ts = new CSommetMesh[nombreSommets];

        for(unsigned int i = 0; i < nombreSommets; ++i)
        {
            ts[i].position = chargeur.GetPosition(i);
            ts[i].normal = chargeur.GetNormale(i);
            ts[i].coordTex = chargeur.GetCoordTex(i);
        }

        // 1. SOMMETS b) Écriture des sommets dans un fichier binaire
        ofstream fichier;
        fichier.open(nomFichier, ios::out | ios_base::binary);

        fichier.write((char*)&nombreSommets, sizeof(nombreSommets));
        fichier.write((char*)ts, nombreSommets * sizeof(CSommetMesh));

        // Détruire ts, devenu inutile
        delete[] ts;

        // 2. INDEX 
        int nombreIndex = chargeur.GetNombreIndex();

        fichier.write((char*)&nombreIndex, sizeof(nombreIndex));
        fichier.write((char*)chargeur.GetIndexData(), nombreIndex * sizeof(unsigned int));

        // 3. Les sous-objets
        NombreSubset = chargeur.GetNombreSubset();

        //    Début de chaque sous-objet et un pour la fin
        vector<int> SI;
        chargeur.CopieSubsetIndex(SI);

        fichier.write((char*)&NombreSubset, sizeof(NombreSubset));
        fichier.write((char*)SI.data(), (NombreSubset + 1) * sizeof(int));

        // 4. MATERIAUX
        // 4a) Créer un matériau de défaut en index 0
        //     Vous pourriez changer les valeurs, j'ai conservé 
        //     celles du constructeur
        CMaterial mat;
        vector<CMaterial> MatLoad;
        MatLoad.push_back(mat);
        int NbMaterial = chargeur.GetNombreMaterial();
        // 4b) Copie des matériaux dans la version locale
        for(int i = 0; i < NbMaterial; ++i)
        {
            chargeur.GetMaterial(i, mat.NomFichierTexture,
                                 mat.NomMateriau,
                                 mat.Ambient,
                                 mat.Diffuse,
                                 mat.Specular,
                                 mat.Puissance);

            MatLoad.push_back(mat);
        }

        NbMaterial++;
        fichier.write((char*)&NbMaterial, sizeof(int));

        MaterialBlock mb;
        for(size_t i = 0; i < NbMaterial; ++i)
        {
            MatLoad[i].MatToBlock(mb);
            fichier.write((char*)&mb, sizeof(MaterialBlock));
        }

        // 4c) Trouver l'index du materiau pour chaque sous-ensemble
        vector<int> SubsetMI;
        for(int i = 0; i < NombreSubset; ++i)
        {
            int index;
            for(index = 0; index < MatLoad.size(); ++index)
            {
                if(MatLoad[index].NomMateriau == chargeur.GetMaterialName(i)) break;
            }

            if(index >= MatLoad.size()) index = 0;  // valeur de défaut

            SubsetMI.push_back(index);
        }

        fichier.write((char*)SubsetMI.data(), (NombreSubset) * sizeof(int));

        fichier.close();
    }

    void CObjetMesh::LireFichierBinaire(string nomFichier)
    {
        ifstream fichier;
        fichier.open(nomFichier, ios::in | ios_base::binary);
        // 1. SOMMETS a) Créations des sommets dans un tableau temporaire

        int nombreSommets;
        fichier.read((char*)&nombreSommets, sizeof(nombreSommets));

        CSommetMesh* ts = new CSommetMesh[nombreSommets];

        // 1. SOMMETS b) Lecture des sommets à partir d'un fichier binaire
        fichier.read((char*)ts, nombreSommets * sizeof(CSommetMesh));

        // 1. SOMMETS b) Création du vertex buffer et copie des sommets
        ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));

        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(CSommetMesh) * nombreSommets;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = ts;
        pVertexBuffer = NULL;

        DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);

        // Détruire ts, devenu inutile
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

        // Détruire index, devenu inutile
        delete[] index;

        // 3. Les sous-objets
        fichier.read((char*)&NombreSubset, sizeof(NombreSubset));
        //    Début de chaque sous-objet et un pour la fin
        int* si = new int[NombreSubset + 1];
        fichier.read((char*)si, (NombreSubset + 1) * sizeof(int));
        SubsetIndex.assign(si, si + (NombreSubset + 1));

        delete[] si;

        // 4. MATERIAUX
        // 4a) Créer un matériau de défaut en index 0
        //     Vous pourriez changer les valeurs, j'ai conservé 
        //     celles du constructeur
        CMaterial mat;

        int NbMaterial;
        fichier.read((char*)&NbMaterial, sizeof(int));

        m_materials.resize(NbMaterial);

        MaterialBlock mb;
        for(int i = 0; i < NbMaterial; ++i)
        {
            fichier.read((char*)&mb, sizeof(MaterialBlock));
            m_materials[i].BlockToMat(mb);
        }

        // 4c) Trouver l'index du materiau pour chaque sous-ensemble
        int* smi = new int[NombreSubset];
        fichier.read((char*)smi, (NombreSubset) * sizeof(int));
        SubsetMaterialIndex.assign(smi, smi + NombreSubset);
        delete[] smi;

        // 4d) Chargement des textures
        CGestionnaireDeTextures& TexturesManager = CMoteurWindows::GetInstance().GetTextureManager();

        for(unsigned int i = 0; i < m_materials.size(); ++i)
        {
            if(m_materials[i].NomFichierTexture != "")
            {
                wstring ws;
                ws.assign(m_materials[i].NomFichierTexture.begin(), m_materials[i].NomFichierTexture.end());

                m_materials[i].pTextureD3D = TexturesManager.GetNewTexture(ws.c_str(), pDispositif)->GetD3DTexture();
            }

        }

        fichier.close();

    }

}   // fin PM3D