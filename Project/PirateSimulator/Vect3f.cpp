#include "Vect3f.h"

#include <cmath>


using namespace PirateSimulator;


Vect3f& Vect3f::operator+=(const Vect3f& other) noexcept
{
    m_X += other.x();
    m_Y += other.y();
    m_Z += other.z();

    return *this;
}


Vect3f& Vect3f::operator-=(const Vect3f& other) noexcept
{
    m_X -= other.x();
    m_Y -= other.y();
    m_Z -= other.z();

    return *this;
}

Vect3f& Vect3f::operator*=(const Vect3f& other) noexcept
{
    *this = (*this + other);
    return *this;
}

Vect3f Vect3f::operator*(Vect3f other) const noexcept
{
    return (Vect3f( ((y() * other.z()) - (z() * other.y())),
                    ((z() * other.x()) - (x() * other.z())),
                    ((x() * other.y()) - (y() * other.x()))
                  )
            );
}

Vect3f& Vect3f::operator*=(float scalar) noexcept
{
    m_X *= scalar;
    m_Y *= scalar;
    m_Z *= scalar;

    return *this;
}

Vect3f& Vect3f::operator/=(float scalar) noexcept
{
    m_X /= scalar;
    m_Y /= scalar;
    m_Z /= scalar;

    return *this;
}

float Vect3f::length() const noexcept
{
    return sqrtf(this->length2());
}
