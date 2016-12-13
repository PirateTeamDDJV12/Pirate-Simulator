#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

namespace PirateSimulator {

    class Transform
    {
    private:
        DirectX::XMVECTOR m_position;
        DirectX::XMVECTOR m_forward;
        DirectX::XMVECTOR m_up;
        DirectX::XMVECTOR m_right;
        physx::PxTransform m_pose;

    public:
         Transform() :
            m_position{ 0.f, 0.f, 0.f, 0.f },
            m_forward{ 0.f,0.f,1.f, 0.f },
            m_up{ 0.f,1.f,0.f,0.f },
            m_right{ 1.f, 0.f, 0.f, 0.f },
            m_pose{ physx::PxTransform(physx::PxVec3{ 0.f,0.f,0.f }) }
        {}

        Transform(
            const DirectX::XMVECTOR& position, 
            const DirectX::XMVECTOR& forward, 
            const DirectX::XMVECTOR& up
        ) :
            m_position(position),
            m_forward(DirectX::XMVector3Normalize(forward)),
            m_up(DirectX::XMVector3Normalize(up)),
            m_right(DirectX::XMVector3Normalize(DirectX::XMVector3Cross(up, forward)))
        {
            physx::PxVec3 newPos(position.vector4_f32[0], position.vector4_f32[1], position.vector4_f32[2]);
            m_pose.p = newPos;
        }

        Transform(const Transform&) = default;

        const DirectX::XMVECTOR& getPosition() const noexcept
        {
            return m_position;
        }
        const DirectX::XMVECTOR& getForward() const noexcept
        {
            return m_forward;
        }
        const DirectX::XMVECTOR& getUp() const noexcept
        {
            return m_up;
        }
        const DirectX::XMVECTOR& getRight() const noexcept
        {
            return m_right;
        }
        const physx::PxTransform& getPose()
        {
            return m_pose;
        }

        void setPosition(const DirectX::XMVECTOR& pos)
        {
            m_position = pos;
            physx::PxVec3 newPos(m_position.vector4_f32[0], m_position.vector4_f32[1], m_position.vector4_f32[2]);
          //  m_pose.p = newPos;
        }



        void setPosition(float x, float y, float z)
        {
            m_position.vector4_f32[0] = x;
            m_position.vector4_f32[1] = y;
            m_position.vector4_f32[2] = z;
            physx::PxVec3 newPos(m_position.vector4_f32[0], m_position.vector4_f32[1], m_position.vector4_f32[2]);
            //m_pose.p = newPos;
        }
        void setPose(const physx::PxTransform& transform)
        {
            m_pose = transform;
            m_position.vector4_f32[0] = transform.p.x;
            m_position.vector4_f32[1] = transform.p.y;
            m_position.vector4_f32[2] = transform.p.z;
            
        }


        void setForward(const DirectX::XMVECTOR& forward)
        {
            m_forward = DirectX::XMVector3Normalize(forward);
            m_right = DirectX::XMVector3Cross(m_up, m_forward);

        }
        void setUp(const DirectX::XMVECTOR& up)
        {
            m_up = DirectX::XMVector3Normalize(up);
        }
        void setRight(const DirectX::XMVECTOR& right)
        {
            m_right = DirectX::XMVector3Normalize(right);
            m_forward = DirectX::XMVector3Cross(m_right, m_up);
        }

        void translate(const DirectX::XMVECTOR &dir)
        {
            
            m_position += dir;
            physx::PxVec3 newPos(m_position.vector4_f32[0], m_position.vector4_f32[1], m_position.vector4_f32[2]);
            //m_pose.p = newPos;

        }
    };

}

#endif // !_TRANSFORM_H_


