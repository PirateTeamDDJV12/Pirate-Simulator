#include "BlocMeshStructure.h"
#include "Transform.h"

namespace PirateSimulator
{
    D3D11_INPUT_ELEMENT_DESC BlocPoint::layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    UINT BlocPoint::numElements = ARRAYSIZE(BlocPoint::layout);


    BlocStructure::BlocStructure(const DirectX::XMFLOAT3& centralPoint, float xLength, float yLength, float zLength)
    {
        mapPoints(centralPoint, xLength, yLength, zLength);
    }

    BlocStructure::BlocStructure(BlocPoint* blocDatas)
    {
        std::copy(blocDatas, blocDatas + 23, m_apex);
    }

    BlocStructure::BlocStructure(float xLength, float yLength, float zLength)
    {
        mapPoints({ 0.f,0.f,0.f }, xLength, yLength, zLength);
    }


    BlocStructure::BlocStructure(const PirateSimulator::Transform& centralPoint, float xLength, float yLength, float zLength)
    {
        DirectX::XMVECTOR position = centralPoint.getPosition();
        mapPoints(DirectX::XMFLOAT3(
            position.vector4_f32[0],
            position.vector4_f32[1],
            position.vector4_f32[2]
            ),
            xLength, yLength, zLength
        );
    }

    void BlocStructure::mapPoints(const DirectX::XMFLOAT3& centralPoint, float xLength, float yLength, float zLength)
    {
        float xMin = centralPoint.x - xLength / 2;
        float xMax = centralPoint.x + xLength / 2;
        float yMin = centralPoint.y - yLength / 2;
        float yMax = centralPoint.y + yLength / 2;
        float zMin = centralPoint.z - zLength / 2;
        float zMax = centralPoint.z + zLength / 2;

        m_position[0] = DirectX::XMFLOAT3(xMin, yMax, zMin);
        m_position[1] = DirectX::XMFLOAT3(xMax, yMax, zMin);
        m_position[2] = DirectX::XMFLOAT3(xMax, yMin, zMin);
        m_position[3] = DirectX::XMFLOAT3(xMin, yMin, zMin);
        m_position[4] = DirectX::XMFLOAT3(xMin, yMax, zMax);
        m_position[5] = DirectX::XMFLOAT3(xMin, yMin, zMax);
        m_position[6] = DirectX::XMFLOAT3(xMax, yMin, zMax);
        m_position[7] = DirectX::XMFLOAT3(xMax, yMax, zMax);

        DirectX::XMFLOAT3 normal(0.0f, 0.0f, -1.0f);

        // Le devant du bloc
        m_apex[0] = BlocPoint(m_position[0], normal);
        m_apex[1] = BlocPoint(m_position[1], normal);
        m_apex[2] = BlocPoint(m_position[2], normal);
        m_apex[3] = BlocPoint(m_position[3], normal);

        // L'arrière du bloc
        normal.z = 1.f;
        m_apex[4] = BlocPoint(m_position[4], normal);
        m_apex[5] = BlocPoint(m_position[5], normal);
        m_apex[6] = BlocPoint(m_position[6], normal);
        m_apex[7] = BlocPoint(m_position[7], normal);

        // Le dessous du bloc
        normal.z = 0.f;
        normal.y = -1.f;
        m_apex[8] = BlocPoint(m_position[3], normal);
        m_apex[9] = BlocPoint(m_position[2], normal);
        m_apex[10] = BlocPoint(m_position[6], normal);
        m_apex[11] = BlocPoint(m_position[5], normal);

        // Le dessus du bloc
        normal.y = 1.f;
        m_apex[12] = BlocPoint(m_position[0], normal);
        m_apex[13] = BlocPoint(m_position[4], normal);
        m_apex[14] = BlocPoint(m_position[7], normal);
        m_apex[15] = BlocPoint(m_position[1], normal);

        // La face gauche
        normal.y = 0.f;
        normal.x = -1.f;
        m_apex[16] = BlocPoint(m_position[0], normal);
        m_apex[17] = BlocPoint(m_position[3], normal);
        m_apex[18] = BlocPoint(m_position[5], normal);
        m_apex[19] = BlocPoint(m_position[4], normal);

        // La face droite
        normal.x = 1.f;
        m_apex[20] = BlocPoint(m_position[1], normal);
        m_apex[21] = BlocPoint(m_position[7], normal);
        m_apex[22] = BlocPoint(m_position[6], normal);
        m_apex[23] = BlocPoint(m_position[2], normal);
    }
}