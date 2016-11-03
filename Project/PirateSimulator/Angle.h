#ifndef ANGLE_H_INCLUDED
#define ANGLE_H_INCLUDED

#include <DirectXMath.h>

namespace PirateSimulator
{
    class AngleRad
    {
    private:
        float m_angle;


    public:
        AngleRad() noexcept :
            m_angle{}
        {}

        AngleRad(float angle) noexcept :
            m_angle{angle}
        {
            loopPiMinusPi();
        }


    public:
        float getAngle() const noexcept { return m_angle; }
        void setAngle(float angle) noexcept 
        {
            m_angle = angle;
            loopPiMinusPi();
        }


    private:
        void loopPiMinusPi() noexcept
        {
            while (m_angle > DirectX::XM_2PI) { m_angle -= DirectX::XM_2PI; }
            while (m_angle < -DirectX::XM_2PI) { m_angle += DirectX::XM_2PI; }
        }


    public:
        AngleRad& operator=(float angle) noexcept
        {
            this->m_angle = angle;
            this->loopPiMinusPi();
            return *this;
        }


        AngleRad& operator+=(float angle) noexcept
        {
            this->m_angle += angle;
            this->loopPiMinusPi();
            return *this;
        }

        AngleRad& operator+=(const AngleRad& other) noexcept
        {
            this->m_angle += other.m_angle;
            this->loopPiMinusPi();
            return *this;
        }

        AngleRad operator+(float angle) const noexcept
        {
            AngleRad intermediary(*this);
            intermediary.m_angle += angle;
            return intermediary;
        }

        AngleRad operator+(const AngleRad& other) const noexcept
        {
            AngleRad intermediary(*this);
            intermediary += other.m_angle;
            return intermediary;
        }

        AngleRad& operator-=(float angle) noexcept
        {
            this->m_angle -= angle;
            this->loopPiMinusPi();
            return *this;
        }

        AngleRad& operator-=(const AngleRad& other) noexcept
        {
            this->m_angle -= other.m_angle;
            this->loopPiMinusPi();
            return *this;
        }

        AngleRad operator-(float angle) const noexcept
        {
            AngleRad intermediary(*this);
            intermediary.m_angle -= angle;
            return intermediary;
        }

        AngleRad operator-(const AngleRad& other) const noexcept
        {
            AngleRad intermediary(*this);
            intermediary -= other.m_angle;
            return intermediary;
        }


        bool operator==(const AngleRad& other) const noexcept
        {
            return this->m_angle == other.m_angle;
        }

        bool operator!=(const AngleRad& other) const noexcept
        {
            return !(*this == other);
        }

        bool operator<(const AngleRad& other) const noexcept
        {
            return this->m_angle < other.m_angle;
        }

        bool operator<=(const AngleRad& other) const noexcept
        {
            return *this < other || *this == other;
        }

        bool operator>(const AngleRad& other) const noexcept
        {
            return !(*this < other || *this == other);
        }

        bool operator>=(const AngleRad& other) const noexcept
        {
            return !(*this < other);
        }
    };
}

#endif //!ANGLE_H_INCLUDED