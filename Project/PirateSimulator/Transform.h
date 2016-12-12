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
        constexpr Transform() :
            m_position{ 0.f, 0.f, 0.f, 0.f },
            m_forward{ 0.f, 0.f, 1.f, 0.f },
            m_up{ 0.f, 1.f, 0.f, 0.f },
            m_right{ 1.f, 0.f, 0.f, 0.f }
        {}

        Transform(
            const DirectX::XMVECTOR& position,
            const DirectX::XMVECTOR& forward,
            const DirectX::XMVECTOR& up,
            const DirectX::XMVECTOR& right
        ) :
            m_position(position),
            m_forward(forward),
            m_up(up),
            m_right(right)
        {}

        Transform(const Transform&) = default;
    };
}

#endif // !_TRANSFORM_H_
