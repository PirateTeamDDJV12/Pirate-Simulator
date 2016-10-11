
#include "Vect4.h"

#include <cmath>




Vect4& Vect4::operator+=(const Vect4& other) noexcept
{
    m_Pos += Vect3::Vect3(other.x(), other.y(), other.z());
    m_T += other.t();
    return *this;
}

Vect4& Vect4::operator-=(const Vect4& other) noexcept
{
    m_Pos -= Vect3::Vect3(other.x(),other.y(),other.z());
    m_T -= other.t();

    return *this;
}



Vect4& Vect4::operator*=(float scalar) noexcept
{
    m_Pos *= scalar;
    m_T *= scalar;
    return *this;
}

Vect4& Vect4::operator/=(float scalar) noexcept
{
    m_Pos /= scalar;
    m_T /= scalar;
    return *this;
}

float Vect4::length() const noexcept
{
    return sqrtf(this->length2());
}
