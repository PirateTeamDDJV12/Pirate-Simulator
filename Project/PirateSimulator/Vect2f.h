#ifndef VECT2F_H
#define VECT2F_H

namespace PirateSimulator
{
    /*Structure for storing point for mapping texture for example*/
    struct Vect2f
    {
	public:
		enum
		{
			INFO_COUNT = 2
		};


    public:
        /*x or u, as U want*/
        float m_U;

        /*y or v, don't mind it*/
        float m_V;


    public:
        Vect2f() noexcept :
            m_U{},
            m_V{}
        {}

        Vect2f(float u, float v) noexcept :
            m_U{ u },
            m_V{ v }
        {}


    public:
        bool operator==(const Vect2f& other) const noexcept
        {
            return ((m_U == other.m_U) && (m_V == other.m_V));
        }

        bool operator!=(const Vect2f& other) const noexcept
        {
            return !(*this == other);
        }
    };
}

#endif //!VECT2F_H