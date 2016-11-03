#include "stdafx.h"

#include "Terrain.h"
#include "../Chapitre 10/PetitMoteur3D/MoteurWindows.h"
#include "../Chapitre 10/PetitMoteur3D/resource.h"
#include "../Chapitre 10/PetitMoteur3D/util.h"
#include "../Chapitre 10/PetitMoteur3D/DispositifD3D11.h"

namespace PirateSimulator
{
    struct ShadersParams
    {
        XMMATRIX matWorldViewProj;	// la matrice totale 
        XMMATRIX matWorld;			// matrice de transformation dans le monde 
        XMVECTOR vLumiere; 			// la position de la source d'éclairage (Point)
        XMVECTOR vCamera; 			// la position de la caméra
        XMVECTOR vAEcl; 			// la valeur ambiante de l'éclairage
        XMVECTOR vAMat; 			// la valeur ambiante du matériau
        XMVECTOR vDEcl; 			// la valeur diffuse de l'éclairage 
        XMVECTOR vDMat; 			// la valeur diffuse du matériau 
    };

    Terrain::Terrain(PM3D::CDispositifD3D11* pDispositif_, int h, int w)
        : m_terrainWidth(w), m_terrainHeight(h)
    {
        pDispositif = pDispositif_; // Prendre en note le dispositif
    }

    Terrain::Terrain(PM3D::CDispositifD3D11* pDispositif_)
    {
        pDispositif = pDispositif_;  // Prendre en note le dispositif
    }

    Terrain::~Terrain()
    {}

    void Terrain::Anime(float tempsEcoule)
    {
        //rotation =  rotation + ( (XM_PI * 2.0f) / 20.0f * tempsEcoule );
        //
        //// modifier la matrice de l'objet bloc
        //matWorld = XMMatrixRotationZ( rotation );
    }

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
        ShadersParams sp;
        XMMATRIX viewProj = PM3D::CMoteurWindows::GetInstance().GetMatViewProj();

        sp.matWorldViewProj = XMMatrixTranspose(matWorld * viewProj);
        sp.matWorld = XMMatrixTranspose(matWorld);

        sp.vLumiere = XMVectorSet(130.0f, 130.0f, -50.0f, 1.0f);
        sp.vCamera = PM3D::CMoteurWindows::GetInstance().getCamera()->position();
        sp.vAEcl = XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
        sp.vAMat = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
        sp.vDEcl = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
        sp.vDMat = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);

        pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &sp, 0, 0);

        pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);

        // Pas de Geometry Shader
        pImmediateContext->GSSetShader(NULL, NULL, 0);

        // Activer le PS
        pImmediateContext->PSSetShader(pPixelShader, NULL, 0);
        pImmediateContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

        // **** Rendu de l'objet	   
        pImmediateContext->DrawIndexed(m_index_bloc.size(), 0, 0);
    }

    void Terrain::Init()
    {
        // Création du vertex buffer et copie des sommets
        ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));

        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(CSommetBloc) * m_sommets.size();
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = m_sommets.data();
        pVertexBuffer = NULL;

        UtilitairesDX::DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);

        // Création de l'index buffer et copie des indices
        ZeroMemory(&bd, sizeof(bd));

        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(unsigned int) * m_index_bloc.size();
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;

        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = m_index_bloc.data();
        pIndexBuffer = NULL;

        UtilitairesDX::DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer),
                                 DXE_CREATIONINDEXBUFFER);

        // Initialisation des shaders
        InitShaders();

        matWorld = XMMatrixIdentity();

        rotation = 0.0f;
    }

    void Terrain::addSommet(PirateSimulator::Vertex v)
    {
        XMFLOAT3 pos{v.position().x(), v.position().y(), v.position().z()};
        XMFLOAT3 nor{v.normalVector().x(), v.normalVector().y(), v.normalVector().z()};
        CSommetBloc c{pos, nor};

        if(m_arraySommets.size() == v.position().x())
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


        if(z < 0 || z + 1 > m_terrainHeight || x < 0 || x + 1 > m_terrainWidth)
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

        float height = 0;

        if(x + z >= 1.0f)
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

        return height;
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

        UtilitairesDX::DXEssayer(pD3DDevice->CreateVertexShader(pVSBlob->GetBufferPointer(),
                                                                pVSBlob->GetBufferSize(),
                                                                NULL,
                                                                &pVertexShader),
                                 DXE_CREATION_VS);

        // Créer l'organisation des sommets
        pVertexLayout = NULL;
        UtilitairesDX::DXEssayer(pD3DDevice->CreateInputLayout(CSommetBloc::layout,
                                                               CSommetBloc::numElements,
                                                               pVSBlob->GetBufferPointer(),
                                                               pVSBlob->GetBufferSize(),
                                                               &pVertexLayout),
                                 DXE_CREATIONLAYOUT);

        pVSBlob->Release(); //  On n'a plus besoin du blob

                            // Création d'un tampon pour les constantes du VS
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));

        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(ShadersParams);
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

        UtilitairesDX::DXEssayer(pD3DDevice->CreatePixelShader(pPSBlob->GetBufferPointer(),
                                                               pPSBlob->GetBufferSize(),
                                                               NULL,
                                                               &pPixelShader),
                                 DXE_CREATION_PS);

        pPSBlob->Release(); //  On n'a plus besoin du blob
    }

}
