#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <string>

namespace PirateSimulator
{
    /*
    A triangle class that contains only 3 indexes (not the specific value of vertex)
    */
    class Triangle
    {
    private:
        using iterator = unsigned int*;


    public:
        enum
        {
            VERTEX_NUMBER = 3
        };


    private:
        unsigned int m_indexToVertex[VERTEX_NUMBER];


    public:
        Triangle() noexcept :
            m_indexToVertex{}
        {}

        Triangle(unsigned int A, unsigned int B, unsigned int C) noexcept :
            m_indexToVertex{ A, B, C }
        {}


    public:
        unsigned int firstPointIndex() const noexcept
        {
            return *m_indexToVertex;
        }

        unsigned int secondPointIndex() const noexcept
        {
            return m_indexToVertex[1];
        }

        unsigned int thirdPointIndex() const noexcept
        {
            return m_indexToVertex[2];
        }

        unsigned int& firstPointIndex() noexcept
        {
            return *m_indexToVertex;
        }

        unsigned int& secondPointIndex() noexcept
        {
            return m_indexToVertex[1];
        }

        unsigned int& thirdPointIndex() noexcept
        {
            return m_indexToVertex[2];
        }


        std::string toString() const noexcept;


    private:
        iterator begin() const noexcept
        {
            return const_cast<iterator>(m_indexToVertex);
        }

        iterator end() const noexcept
        {
            return const_cast<iterator>(m_indexToVertex + VERTEX_NUMBER);
        }


    public:
        bool operator==(Triangle& other) const noexcept
        {
            return ((firstPointIndex() == other.firstPointIndex()) && secondPointIndex() == other.secondPointIndex() && 
                    thirdPointIndex() == other.thirdPointIndex());
        }

        bool operator!=(Triangle& other) const noexcept
        {
            return (!(*this == other));
        }
    };
}


#endif //!TRIANGLE_H