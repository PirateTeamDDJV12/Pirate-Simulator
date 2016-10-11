#include "Matrix4.h"
#include "Vect4.h"
#include <algorithm>
#include <array>

Matrix4::Matrix4(Vect4 col1, Vect4 col2, Vect4 col3, Vect4 col4)
{
    m_elements[0] = col1.x();
    m_elements[1] = col2.x();
    m_elements[2] = col3.x();
    m_elements[3] = col4.x();
    m_elements[4] = col1.y();
    m_elements[5] = col2.y();
    m_elements[6] = col3.y();
    m_elements[7] = col4.y();
    m_elements[8] = col1.z();
    m_elements[9] = col2.z();
    m_elements[10] = col3.z();
    m_elements[11] = col4.z();
    m_elements[12] = col1.t();
    m_elements[13] = col2.t();
    m_elements[14] = col3.t();
    m_elements[15] = col4.t();
}


Matrix4 Matrix4::transpose()
{
    std::array<float, 16> elements;
    for (int i = 0; i < m_nbRows; i++)
    {
        for (int j = 0; j < m_nbColumns; j++)
        {
            elements[j * m_nbColumns + i] = m_elements[j * m_nbColumns + i];
        }
    }
    return Matrix4(elements);
}


Vect4& Matrix4::operator*(Vect4& other)
{
    return(Vect4::Vect4(m_elements[0] * other.x() + m_elements[1] * other.y() + m_elements[2] * other.z() + m_elements[3] * other.t(),
        m_elements[4] * other.x() + m_elements[5] * other.y() + m_elements[6] * other.z() + m_elements[7] * other.t(),
        m_elements[8] * other.x() + m_elements[9] * other.y() + m_elements[10] * other.z() + m_elements[11] * other.t(),
        m_elements[12] * other.x() + m_elements[13] * other.y() + m_elements[14] * other.z() + m_elements[15] * other.t()));
}


Matrix4& Matrix4::operator*(Matrix4& other)
{
    Vect4 otherCol1 = Vect4::Vect4(other(0, 0), other(1, 0), other(2, 0), other(3, 0));
    Vect4 otherCol2 = Vect4::Vect4(other(0, 1), other(1, 1), other(2, 1), other(3, 1));
    Vect4 otherCol3 = Vect4::Vect4(other(0, 2), other(1, 2), other(2, 2), other(3, 2));
    Vect4 otherCol4 = Vect4::Vect4(other(0, 3), other(1, 3), other(2, 3), other(3, 3));
    Vect4 resCol1 = *this*otherCol1;
    Vect4 resCol2 = *this*otherCol2;
    Vect4 resCol3 = *this*otherCol3;
    Vect4 resCol4 = *this*otherCol4;
    return Matrix4(resCol1, resCol2, resCol3, resCol4);
}

Matrix4& Matrix4::operator+(Matrix4& other)
{
    std::array<float, 16> res;
        for (int i = 0; i < 16; i++)
        {
            res[i] = other.elements()[i] + m_elements[i];
        }
    return Matrix4::Matrix4(res);
}

Matrix4& Matrix4::operator-(Matrix4& other)
{
    std::array<float, 16> res;
    for (int i = 0; i < 16; i++)
    {
        res[i] = other.elements()[i] - m_elements[i];
    }
    return Matrix4::Matrix4(res);
}