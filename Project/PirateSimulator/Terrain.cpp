#include "stdafx.h"

#include "Terrain.h"
#include "../sommetbloc.h"
#include "../util.h"
#include "../resource.h"
#include "../Bloc.h"
#include "../MoteurWindows.h"

namespace PM3D
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

    Terrain::Terrain(CDispositifD3D11* pDispositif_)
    {
        pDispositif = pDispositif_;  // Prendre en note le dispositif
    }

    Terrain::~Terrain()
    {}

    void Terrain::Anime(float tempsEcoule)
    {
        rotation = rotation + ((XM_PI * 2.0f) / 10.0f * tempsEcoule);

        // modifier la matrice de l'objet bloc
        matWorld = XMMatrixRotationZ(rotation);
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
        XMMATRIX viewProj = CMoteurWindows::GetInstance().GetMatViewProj();

        sp.matWorldViewProj = XMMatrixTranspose(matWorld * viewProj);
        sp.matWorld = XMMatrixTranspose(matWorld);

        sp.vLumiere = XMVectorSet(-10.0f, 10.0f, -10.0f, 1.0f);
        sp.vCamera = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
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
        m_sommets.push_back(c);
    }

    void Terrain::addTriangle(PirateSimulator::Triangle t)
    {
        m_index_bloc.push_back(t.firstPointIndex());
        m_index_bloc.push_back(t.secondPointIndex());
        m_index_bloc.push_back(t.thirdPointIndex());
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
