/*
Created by Sun-lay gagneux
*/
#include "Vect4.h"

#include <cmath>




//template<typename valueType>
Vect4& Vect4::operator+(const Vect4& other) noexcept
{
    m_X += other.x();
    m_Y += other.y();
    m_Z += other.z();
    m_T += other.t();
    return *this;
}

//template<typename valueType>
Vect4& Vect4::operator-(const Vect4& other) noexcept
{
    m_X -= other.x();
    m_Y -= other.y();
    m_Z -= other.z();
    m_T -= other.t();

    return *this;
}
//template<typename valueType>


/*template<typename valueType>
Vect4 Vect4::operator*(Vect4 other) const noexcept
{
return (Vect4(((y() * other.z()) - (z() * other.y())),
((z() * other.x()) - (x() * other.z())),
((x() * other.y()) - (y() * other.x()))
)
);
}*/

//template<typename valueType>
Vect4& Vect4::operator*(float scalar) noexcept
{
    m_X *= scalar;
    m_Y *= scalar;
    m_Z *= scalar;
    m_T *= scalar;
    return *this;
}

//template<typename valueType>
Vect4& Vect4::operator/(float scalar) noexcept
{
    m_X /= scalar;
    m_Y /= scalar;
    m_Z /= scalar;
    m_T /= scalar;
    return *this;
}

//template<typename valueType>
float Vect4::length() const noexcept
{
    return sqrtf(this->length2());
}
