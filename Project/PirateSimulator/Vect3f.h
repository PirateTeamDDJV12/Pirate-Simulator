#ifndef VECT3F_H
#define VECT3F_H


namespace PirateSimulator
{
    class Vect3f
    {
	public:
		enum
		{
			INFO_COUNT = 3
		};

    private:
        float m_X;
        float m_Y;
        float m_Z;

    public:
        Vect3f() noexcept :
            m_X{},
            m_Y{},
            m_Z{}
        {}

        Vect3f(float x, float y) noexcept :
        m_X{ x },
            m_Y{ y },
            m_Z{}
        {}

        Vect3f(float x, float y, float z) noexcept :
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


        float scalarProduct(const Vect3f& other) const noexcept
        {
            return scalarProduct(*this, other);
        }

        /*
        this x other
        */
        Vect3f crossProduct(const Vect3f& other) const noexcept
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
        static float scalarProduct(const Vect3f& vect1, const Vect3f& vect2) noexcept
        {
            return ((vect1.x() * vect2.x()) + (vect1.y() * vect2.y()) + (vect1.z() * vect2.z()));
        }

        /*
        first x second
        */
        static Vect3f crossProduct(const Vect3f& first, const Vect3f& second) noexcept
        {
            return (first * second);
        }

        static float length2(const Vect3f& vect3) noexcept
        {
            return vect3.length2();
        }

        static float length(const Vect3f& vect3) noexcept
        {
            return vect3.length();
        }

    public:
        bool operator==(const Vect3f& other) const noexcept
        {
            return ((x() == other.x()) && (y() == other.y()) && (z() == other.z()));
        }

        bool operator!=(const Vect3f& other) const noexcept
        {
            return !(*this == other);
        }

        Vect3f& operator+=(const Vect3f& other) noexcept;
        Vect3f& operator-=(const Vect3f& other) noexcept;
        Vect3f& operator*=(const Vect3f& other) noexcept;

        Vect3f& operator*=(float scalar) noexcept;
        Vect3f& operator/=(float scalar) noexcept;


        Vect3f operator+(Vect3f other) const noexcept
        {
            return (other += *this);
        }

        Vect3f operator-(const Vect3f& other) const noexcept
        {
            return (Vect3f(*this) -= other);
        }

        Vect3f operator*(float scalar) const noexcept
        {
            return (Vect3f(*this) *= scalar);
        }

        Vect3f operator/(float scalar) const noexcept
        {
            return (Vect3f(*this) /= scalar);
        }

        /*
        Vectorial product :
        this x other
        */
        Vect3f operator*(Vect3f other) const noexcept;

        
    };
}

#endif //!VECT3F_H