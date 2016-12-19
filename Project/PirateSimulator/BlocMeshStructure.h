#ifndef BLOCMESHSTRUCTURE_H_INCLUDED
#define BLOCMESHSTRUCTURE_H_INCLUDED


#include <directxmath.h>

namespace PirateSimulator
{
    struct BlocPoint
    {
    public:
        static UINT numElements;
        static D3D11_INPUT_ELEMENT_DESC layout[];


    public:
        DirectX::XMFLOAT3 m_position;
        DirectX::XMFLOAT3 m_normal;
        DirectX::XMFLOAT2 m_textureCoordinate;


    public:
        BlocPoint() :
            m_position{},
            m_normal{},
            m_textureCoordinate{}
        {}

        BlocPoint(const DirectX::XMFLOAT3& _position, 
            const DirectX::XMFLOAT3& _normal, 
            const DirectX::XMFLOAT2& _coordTex = DirectX::XMFLOAT2(0, 0)
        ) :
            m_position{_position},
            m_normal{_normal},
            m_textureCoordinate{_coordTex}
        {}
    };

    class Transform;

    /*A structure for bloc. Inherit it and make the mapping as you like*/
    struct BlocStructure
    {
    public:
        DirectX::XMFLOAT3 m_position[8];

        BlocPoint m_apex[24];


    public:
        BlocStructure(const DirectX::XMFLOAT3& centralPoint, float xLength, float yLength, float zLength);
        BlocStructure(const PirateSimulator::Transform& centralPoint, float xLength, float yLength, float zLength);
        BlocStructure(BlocPoint* blocDatas);
        BlocStructure(float xLength, float yLength, float zLength);


    public:
        virtual void mapPoints(const DirectX::XMFLOAT3& centralPoint, float xLength, float yLength, float zLength);
    };
}

#endif //!BLOCMESHSTRUCTURE_H_INCLUDED