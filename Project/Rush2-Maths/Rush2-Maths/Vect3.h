#ifndef VECT3_H
#define VECT3_H
#include "Vect.h"

//    template<typename valueType>
    class Vect3//: Vect
    {
    private:
        float m_X;
        float m_Y;
        float m_Z;

    public:
        Vect3() noexcept :
            m_X{},
            m_Y{},
            m_Z{}
        {}

        Vect3(float x, float y) noexcept :
        m_X{ x },
            m_Y{ y },
            m_Z{}
        {}

        Vect3(float x, float y, float z) noexcept :
        m_X{ x },
            m_Y{ y },
            m_Z{ z }
        {}

    public:
        float x() const noexcept
        {
            return m_X;
        }

        float y() const noexcept
        {
            return m_Y;
        }

        float z() const noexcept
        {
            return m_Z;
        }

        void x(float newValue) noexcept
        {
            m_X = newValue;
        }

        void y(float newValue) noexcept
        {
            m_Y = newValue;
        }

        void z(float newValue) noexcept
        {
            m_Z = newValue;
        }


        float scalarProduct(const Vect3& other) const noexcept
        {
            return scalarProduct(*this, other);
        }

        /*
        this x other
        */
        Vect3 crossProduct(const Vect3& other) const noexcept
        {
            return (*this * other);
        }

        float length2() const noexcept
        {
            return ((x() * x()) + (y() * y()) + (z() * z()));
        }

        float length() const noexcept;

        bool isOrigin() const noexcept
        {
            return ((x() == 0.f) && y() == 0.f && z() == 0.f);
        }

    public:
        /*
        vect1 . vect2
        */
        static float scalarProduct(const Vect3& vect1, const Vect3& vect2) noexcept
        {
            return ((vect1.x() * vect2.x()) + (vect1.y() * vect2.y()) + (vect1.z() * vect2.z()));
        }

        /*
        first x second
        */
        static Vect3& crossProduct(const Vect3& first, const Vect3& second) noexcept
        {
            return (first * second);
        }

        static float length2(const Vect3& vect3) noexcept
        {
            return vect3.length2();
        }

        static float length(const Vect3& vect3) noexcept
        {
            return vect3.length();
        }

    public:
        bool operator==(const Vect3& other) const noexcept
        {
            return ((x() == other.x()) && (y() == other.y()) && (z() == other.z()));
        }

        bool operator!=(const Vect3& other) const noexcept
        {
            return !(*this == other);
        }

        Vect3& operator+(const Vect3& other) noexcept;
        Vect3& operator-(const Vect3& other) noexcept;
        Vect3& operator*(const Vect3& other) noexcept;

        Vect3& operator*(float scalar) noexcept;
        Vect3& operator/(float scalar) noexcept;


        Vect3 operator+(Vect3 other) const noexcept
        {
            return (other + *this);
        }

        Vect3& operator-(const Vect3& other) const noexcept
        {
            return (Vect3(*this) - other);
        }

        Vect3 operator*(float scalar) const noexcept
        {
            return (Vect3(*this) * scalar);
        }

        Vect3 operator/(float scalar) const noexcept
        {
            return (Vect3(*this) / scalar);
        }

        /*
        Vectorial product :
        this x other
        */
        Vect3 operator*(Vect3 other) const noexcept;


    };


#endif //!VECT3_H