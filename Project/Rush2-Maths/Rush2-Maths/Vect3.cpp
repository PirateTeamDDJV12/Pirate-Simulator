/*
Created by Sun-lay gagneux
*/
#include "Vect3.h"

#include <cmath>




 //   template<typename valueType>
    Vect3& Vect3::operator+(const Vect3& other) noexcept
    {
        m_X += other.x();
        m_Y += other.y();
        m_Z += other.z();

        return *this;
    }

    //template<typename valueType>
    Vect3& Vect3::operator-(const Vect3& other) noexcept
    {
        m_X -= other.x();
        m_Y -= other.y();
        m_Z -= other.z();

        return *this;
    }
    //template<typename valueType>
    Vect3& Vect3::operator*(const Vect3& other) noexcept
    {
        *this = (*this + other);
        return *this;
    }

    //template<typename valueType>
    Vect3 Vect3::operator*(Vect3 other) const noexcept
    {
        return (Vect3(((y() * other.z()) - (z() * other.y())),
            ((z() * other.x()) - (x() * other.z())),
            ((x() * other.y()) - (y() * other.x()))
            )
            );
    }

    //template<typename valueType>
    Vect3& Vect3::operator*(float scalar) noexcept
    {
        m_X *= scalar;
        m_Y *= scalar;
        m_Z *= scalar;

        return *this;
    }

    //template<typename valueType>
    Vect3& Vect3::operator/(float scalar) noexcept
    {
        m_X /= scalar;
        m_Y /= scalar;
        m_Z /= scalar;

        return *this;
    }

    //template<typename valueType>
    float Vect3::length() const noexcept
    {
        return sqrtf(this->length2());
    }
