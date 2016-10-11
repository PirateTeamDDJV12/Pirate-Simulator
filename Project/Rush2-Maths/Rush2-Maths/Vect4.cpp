
#include "Vect4.h"

#include <cmath>




Vect4& Vect4::operator+(const Vect4& other) noexcept
{
    m_X += other.x();
    m_Y += other.y();
    m_Z += other.z();
    m_T += other.t();
    return *this;
}

Vect4& Vect4::operator-(const Vect4& other) noexcept
{
    m_X -= other.x();
    m_Y -= other.y();
    m_Z -= other.z();
    m_T -= other.t();

    return *this;
}



Vect4& Vect4::operator*(float scalar) noexcept
{
    m_X *= scalar;
    m_Y *= scalar;
    m_Z *= scalar;
    m_T *= scalar;
    return *this;
}

Vect4& Vect4::operator/(float scalar) noexcept
{
    m_X /= scalar;
    m_Y /= scalar;
    m_Z /= scalar;
    m_T /= scalar;
    return *this;
}

float Vect4::length() const noexcept
{
    return sqrtf(this->length2());
}
