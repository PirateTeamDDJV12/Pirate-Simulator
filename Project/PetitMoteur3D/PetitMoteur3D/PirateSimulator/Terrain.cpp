#include "stdafx.h"

#include "Terrain.h"
#include "../sommetbloc.h"
#include "../util.h"
#include "../resource.h"
#include "../Bloc.h"

PM3D::Terrain::Terrain(const float dx, const float dy, CDispositifD3D11* pDispositif_)
{
    pDispositif = pDispositif_;  // Prendre en note le dispositif


    //// Les points
    //XMFLOAT3 point[8];
    //point[0] = XMFLOAT3(-dx / 2, dy / 2, -dz / 2);
    //point[1] = XMFLOAT3(dx / 2, dy / 2, -dz / 2);
    //point[2] = XMFLOAT3(dx / 2, -dy / 2, -dz / 2);
    //point[3] = XMFLOAT3(-dx / 2, -dy / 2, -dz / 2);
    //point[4] = XMFLOAT3(-dx / 2, dy / 2, dz / 2);
    //point[5] = XMFLOAT3(-dx / 2, -dy / 2, dz / 2);
    //point[6] = XMFLOAT3(dx / 2, -dy / 2, dz / 2);
    //point[7] = XMFLOAT3(dx / 2, dy / 2, dz / 2);

    //// Calculer les normales
    //XMFLOAT3 n0(0.0f, 0.0f, -1.0f); // devant
    //XMFLOAT3 n1(0.0f, 0.0f, 1.0f); // arrière
    //XMFLOAT3 n2(0.0f, -1.0f, 0.0f); // dessous
    //XMFLOAT3 n3(0.0f, 1.0f, 0.0f); // dessus
    //XMFLOAT3 n4(-1.0f, 0.0f, 0.0f); // face gauche
    //XMFLOAT3 n5(1.0f, 0.0f, 0.0f); // face droite

    //CSommetBloc sommets[24];

    //// Le devant du bloc
    //sommets[0] = CSommetBloc(point[0], n0);
    //sommets[1] = CSommetBloc(point[1], n0);
    //sommets[2] = CSommetBloc(point[2], n0);
    //sommets[3] = CSommetBloc(point[3], n0);

    //// L'arrière du bloc
    //sommets[4] = CSommetBloc(point[4], n1);
    //sommets[5] = CSommetBloc(point[5], n1);
    //sommets[6] = CSommetBloc(point[6], n1);
    //sommets[7] = CSommetBloc(point[7], n1);

    //// Le dessous du bloc
    //sommets[8] = CSommetBloc(point[3], n2);
    //sommets[9] = CSommetBloc(point[2], n2);
    //sommets[10] = CSommetBloc(point[6], n2);
    //sommets[11] = CSommetBloc(point[5], n2);

    //// Le dessus du bloc
    //sommets[12] = CSommetBloc(point[0], n3);
    //sommets[13] = CSommetBloc(point[4], n3);
    //sommets[14] = CSommetBloc(point[7], n3);
    //sommets[15] = CSommetBloc(point[1], n3);
    //// La face gauche
    //sommets[16] = CSommetBloc(point[0], n4);
    //sommets[17] = CSommetBloc(point[3], n4);
    //sommets[18] = CSommetBloc(point[5], n4);
    //sommets[19] = CSommetBloc(point[4], n4);

    //// La face droite
    //sommets[20] = CSommetBloc(point[1], n5);
    //sommets[21] = CSommetBloc(point[7], n5);
    //sommets[22] = CSommetBloc(point[6], n5);
    //sommets[23] = CSommetBloc(point[2], n5);
    
    // Création du vertex buffer et copie des sommets
    ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(CSommetBloc) * 24;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    //InitData.pSysMem = sommets;
    pVertexBuffer = NULL;

    UtilitairesDX::DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);

    // Création de l'index buffer et copie des indices
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(unsigned int) * 36;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = index_bloc;
    pIndexBuffer = NULL;

    UtilitairesDX::DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer),
              DXE_CREATIONINDEXBUFFER);

    // Initialisation de l'effet
    InitEffet();

    matWorld = XMMatrixIdentity();

    rotation = 0.0f;
}

PM3D::Terrain::~Terrain()
{
}

void PM3D::Terrain::Anime(float tempsEcoule)
{
}

void PM3D::Terrain::Draw()
{
}

void PM3D::Terrain::InitEffet()
{
}
