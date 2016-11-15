#include "stdafx.h"

#include "BlocMeshStructure.h"

namespace PirateSimulator
{
    D3D11_INPUT_ELEMENT_DESC BlocPoint::layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    UINT BlocPoint::numElements;


    BlocStructure::BlocStructure(const DirectX::XMFLOAT3& centralPoint, float xLength, float yLength, float zLength)
    {
        float xMin = centralPoint.x - xLength / 2;
        float xMax = centralPoint.x + xLength / 2;
        float yMin = centralPoint.y - yLength / 2;
        float yMax = centralPoint.y + yLength / 2;
        float zMin = centralPoint.z - zLength / 2;
        float zMax = centralPoint.z + zLength / 2;

        position[0] = DirectX::XMFLOAT3(xMin, yMax, zMin);
        position[1] = DirectX::XMFLOAT3(xMax, yMax, zMin);
        position[2] = DirectX::XMFLOAT3(xMax, yMin, zMin);
        position[3] = DirectX::XMFLOAT3(xMin, yMin, zMin);
        position[4] = DirectX::XMFLOAT3(xMin, yMax, zMax);
        position[5] = DirectX::XMFLOAT3(xMin, yMin, zMax);
        position[6] = DirectX::XMFLOAT3(xMax, yMin, zMax);
        position[7] = DirectX::XMFLOAT3(xMax, yMax, zMax);

        DirectX::XMFLOAT3 normal(0.0f, 0.0f, -1.0f);

        // Le devant du bloc
        m_apex[0] = BlocPoint(position[0], normal);
        m_apex[1] = BlocPoint(position[1], normal);
        m_apex[2] = BlocPoint(position[2], normal);
        m_apex[3] = BlocPoint(position[3], normal);

        // L'arrière du bloc
        normal.z = 1.f;
        m_apex[4] = BlocPoint(position[4], normal);
        m_apex[5] = BlocPoint(position[5], normal);
        m_apex[6] = BlocPoint(position[6], normal);
        m_apex[7] = BlocPoint(position[7], normal);

        // Le dessous du bloc
        normal.z = 0.f;
        normal.y = -1.f;
        m_apex[8] = BlocPoint(position[3], normal);
        m_apex[9] = BlocPoint(position[2], normal);
        m_apex[10] = BlocPoint(position[6], normal);
        m_apex[11] = BlocPoint(position[5], normal);

        // Le dessus du bloc
        normal.y = 1.f;
        m_apex[12] = BlocPoint(position[0], normal);
        m_apex[13] = BlocPoint(position[4], normal);
        m_apex[14] = BlocPoint(position[7], normal);
        m_apex[15] = BlocPoint(position[1], normal);

        // La face gauche
        normal.y = 0.f;
        normal.x = -1.f;
        m_apex[16] = BlocPoint(position[0], normal);
        m_apex[17] = BlocPoint(position[3], normal);
        m_apex[18] = BlocPoint(position[5], normal);
        m_apex[19] = BlocPoint(position[4], normal);

        // La face droite
        normal.x = 1.f;
        m_apex[20] = BlocPoint(position[1], normal);
        m_apex[21] = BlocPoint(position[7], normal);
        m_apex[22] = BlocPoint(position[6], normal);
        m_apex[23] = BlocPoint(position[2], normal);
    }

    BlocStructure::BlocStructure(BlocPoint* blocDatas)
    {
        std::copy(blocDatas, blocDatas + 23, m_apex);
    }

    BlocStructure::BlocStructure(float xLength, float yLength, float zLength)
    {
        float xMin = -xLength / 2;
        float xMax = xLength / 2;
        float yMin = -yLength / 2;
        float yMax = yLength / 2;
        float zMin = -zLength / 2;
        float zMax = zLength / 2;

        DirectX::XMFLOAT3 position[8];

        position[0] = DirectX::XMFLOAT3(xMin, yMax, zMin);
        position[1] = DirectX::XMFLOAT3(xMax, yMax, zMin);
        position[2] = DirectX::XMFLOAT3(xMax, yMin, zMin);
        position[3] = DirectX::XMFLOAT3(xMin, yMin, zMin);
        position[4] = DirectX::XMFLOAT3(xMin, yMax, zMax);
        position[5] = DirectX::XMFLOAT3(xMin, yMin, zMax);
        position[6] = DirectX::XMFLOAT3(xMax, yMin, zMax);
        position[7] = DirectX::XMFLOAT3(xMax, yMax, zMax);


        DirectX::XMFLOAT3 normal(0.0f, 0.0f, -1.0f);

        // Le devant du bloc
        m_apex[0] = BlocPoint(position[0], normal);
        m_apex[1] = BlocPoint(position[1], normal);
        m_apex[2] = BlocPoint(position[2], normal);
        m_apex[3] = BlocPoint(position[3], normal);

        // L'arrière du bloc
        normal.z = 1.f;
        m_apex[4] = BlocPoint(position[4], normal);
        m_apex[5] = BlocPoint(position[5], normal);
        m_apex[6] = BlocPoint(position[6], normal);
        m_apex[7] = BlocPoint(position[7], normal);

        // Le dessous du bloc
        normal.z = 0.f;
        normal.y = -1.f;
        m_apex[8] = BlocPoint(position[3], normal);
        m_apex[9] = BlocPoint(position[2], normal);
        m_apex[10] = BlocPoint(position[6], normal);
        m_apex[11] = BlocPoint(position[5], normal);

        // Le dessus du bloc
        normal.y = 1.f;
        m_apex[12] = BlocPoint(position[0], normal);
        m_apex[13] = BlocPoint(position[4], normal);
        m_apex[14] = BlocPoint(position[7], normal);
        m_apex[15] = BlocPoint(position[1], normal);

        // La face gauche
        normal.y = 0.f;
        normal.x = -1.f;
        m_apex[16] = BlocPoint(position[0], normal);
        m_apex[17] = BlocPoint(position[3], normal);
        m_apex[18] = BlocPoint(position[5], normal);
        m_apex[19] = BlocPoint(position[4], normal);

        // La face droite
        normal.x = 1.f;
        m_apex[20] = BlocPoint(position[1], normal);
        m_apex[21] = BlocPoint(position[7], normal);
        m_apex[22] = BlocPoint(position[6], normal);
        m_apex[23] = BlocPoint(position[2], normal);
    }
}