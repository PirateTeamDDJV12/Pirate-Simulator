#ifndef MATRIX3_H
#define MATRIX_H
#include "Matrix.h"
#include <memory>
#include <array>

class Vect3;

class Matrix3
{
private:
    std::array<float, 9> m_elements{};
    unsigned int m_nbColumns = 3;
    unsigned int m_nbRows = 3;
public:
    Matrix3(Vect3 col1, Vect3 col2, Vect3 col3);
    unsigned int nbRows()
    {
        return m_nbRows;
    }
    unsigned int nbColumns()
    {
        return m_nbColumns;
    }
    std::array<float, 9> elements()
    {
        return m_elements;
    }
   
    Matrix3(std::array<float, 9> elements ): m_nbRows{ 3 }, m_nbColumns{ 3 } {
        std::copy(elements.begin(), elements.end(), m_elements.begin());
    }
    Matrix3 transpose();
    Vect3 operator*(Vect3& other);
    Matrix3& operator*(Matrix3& other);
    Matrix3& Matrix3::operator+(Matrix3& other);
    Matrix3& Matrix3::operator-(Matrix3& other);
    float& operator()(unsigned i, unsigned j) //i,j=0..3
    {
        return m_elements[i*m_nbColumns + j];
    }
   
 
};



#endif // !MATRIX_H
