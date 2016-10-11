#include "Matrix3.h"
#include "Vect3.h"
#include <algorithm>
#include <array>

Matrix3::Matrix3(Vect3 col1, Vect3 col2, Vect3 col3)
{
    m_elements[0] = col1.x();
    m_elements[1] = col2.x();
    m_elements[2] = col3.x();
    m_elements[3] = col1.y();
    m_elements[4] = col2.y();
    m_elements[5] = col3.y();
    m_elements[6] = col1.z();
    m_elements[7] = col2.z();
    m_elements[8] = col3.z();
}


Matrix3 Matrix3::transpose()
{
    std::array<float, 9> elements{0};
    for (unsigned int i = 0; i < m_nbRows; i++)
    {
        for (unsigned int j = 0; j < m_nbColumns; j++)
        {
            elements[j*m_nbColumns+i] = m_elements[i * m_nbColumns + j];
        }
            
    }
    return Matrix3(elements);
}


Vect3 Matrix3::operator*(Vect3& other)
{
    Vect3 res = (Vect3::Vect3(m_elements[0] * other.x() + m_elements[1] * other.y() + m_elements[2] * other.z(),
        m_elements[3] * other.x() + m_elements[4] * other.y() + m_elements[5] * other.z(),
        m_elements[6] * other.x() + m_elements[7] * other.y() + m_elements[8] * other.z()));
    return res;
}


Matrix3& Matrix3::operator*(Matrix3& other)
{
    Vect3 otherCol1 = Vect3::Vect3(other(0, 0), other(1, 0), other(2, 0));
    Vect3 otherCol2 = Vect3::Vect3(other(0, 1), other(1, 1), other(2, 1));
    Vect3 otherCol3 = Vect3::Vect3(other(0, 2), other(1, 2), other(2, 2));
    Vect3 resCol1 = *this*(otherCol1);
    Vect3 resCol2 = *this*otherCol2;
    Vect3 resCol3 = *this*otherCol3;
    Matrix3& res =  Matrix3(resCol1, resCol2, resCol3);
    return res;
}

Matrix3& Matrix3::operator+(Matrix3& other)
{
    std::array<float, 9> res;
        for (int i = 0; i < 9; i++)
        {
            res[i] = other.elements()[i] + m_elements[i];
        }
    return Matrix3::Matrix3(res);
}

Matrix3& Matrix3::operator-(Matrix3& other)
{
    std::array<float, 9> res;
    for (int i = 0; i < 9; i++)
    {
        res[i] = other.elements()[i] - m_elements[i];
    }
    return Matrix3::Matrix3(res);
}
