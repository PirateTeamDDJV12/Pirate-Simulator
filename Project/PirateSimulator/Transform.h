#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

namespace PirateSimulator {

    class Transform
    {
    public:
        DirectX::XMVECTOR m_position;
        DirectX::XMVECTOR m_forward;
        DirectX::XMVECTOR m_up;
        DirectX::XMVECTOR m_right;

    public:
        Transform();
        ~Transform();
    };

}

#endif // !_TRANSFORM_H_


