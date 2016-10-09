#ifndef VECT4_H
#define VECT4_H
#include "Vect.h"


//template<typename valueType>
class Vect4 //: public Vect
{
private:
    float m_X;
    float m_Y;
    float m_Z;
    float m_T;

public:
    Vect4() noexcept :
        m_X{},
        m_Y{},
        m_Z{},
        m_T{}
    {}

    Vect4(float x, float y, float z, float t) noexcept :
        m_X{ x },
        m_Y{ y },
        m_Z{ z },
        m_T{ t }
    {}

    Vect4( float x, float y, float z) noexcept :
        m_X{ x },
        m_Y{ y },
        m_Z{ z },
        m_T{}
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
    float t() const noexcept
    {
        return m_T;
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
    void t(float newValue) noexcept
    {
        m_T = newValue;
    }

    float scalarProduct(const Vect4& other) const noexcept
    {
        return scalarProduct(*this, other);
    }



    float length2() const noexcept
    {
        return ((x() * x()) + (y() * y()) + (z() * z()) + (t() * t()));
    }

    float length() const noexcept;

    bool isOrigin() const noexcept
    {
        return ((x() == 0.f) && y() == 0.f && z() == 0.f); //t ne rentre pas en compte dans le calcul de l'origine
    }

public:
    /*
    vect1 . vect2
    */
    static float scalarProduct(const Vect4& vect1, const Vect4& vect2) noexcept
    {
        return ((vect1.x() * vect2.x()) + (vect1.y() * vect2.y()) + (vect1.z() * vect2.z()) + (vect1.t() * vect2.t()));
    }

    /*
    first x second
    */
    /*static Vect4 crossProduct(const Vect4& first, const Vect4& second) noexcept
    {
    return (first * second);
    }*/

    static float length2(const Vect4& vect4) noexcept
    {
        return vect4.length2();
    }

    static float length(const Vect4& vect4) noexcept
    {
        return vect4.length();
    }

public:
    bool operator==(const Vect4& other) const noexcept
    {
        return ((x() == other.x()) && (y() == other.y()) && (z() == other.z()) && (t() == other.t()));
    }

    bool operator!=(const Vect4& other) const noexcept
    {
        return !(*this == other);
    }

    Vect4& operator+(const Vect4& other) noexcept;
    Vect4& operator-(const Vect4& other) noexcept;
    //Vect4& operator*=(const Vect4& other) noexcept; Produit vectoriel en 4D -> inutile

    Vect4& operator*(float scalar) noexcept;
    Vect4& operator/(float scalar) noexcept;


    Vect4& operator+(Vect4& other) const noexcept
    {
        return (other + *this);
    }

    Vect4& operator-(const Vect4& other) const noexcept
    {
        return (*this - other);
    }

    Vect4& operator*(float scalar) const noexcept
    {
        return ((*this) * scalar);
    }

    Vect4& operator/(float scalar) const noexcept
    {
        return ((*this) / scalar);
    }

    /*
    Vectorial product :
    this x other
    */
    //	Vect4<valueType> operator*(Vect4<valueType> other) const noexcept;


};


#endif //!VECT4_H