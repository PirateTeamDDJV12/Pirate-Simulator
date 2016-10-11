#ifndef MATRIX4_H
#define MATRIX4_H
#include "Matrix.h" 
#include "Vect4.h"
#include <array>
class Matrix4 
{
private:
    std::array<float, 16> m_elements;
    unsigned int m_nbRows = 4;
    unsigned int m_nbColumns = 4;
public:
    Matrix4(Vect4 col1, Vect4 col2, Vect4 col3, Vect4 col4);
    Matrix4(std::array<float, 16> elements) : m_nbRows{ 4 }, m_nbColumns{ 4 }
    {
        std::copy(elements.begin(), elements.end(), m_elements.begin());
    }
    ~Matrix4(){}
    
    unsigned int nbRows()
    {
        return m_nbRows;
    }
    unsigned int nbColumns()
    {
        return m_nbColumns;
    }
    std::array<float, 16> elements()
    {
        return m_elements;
    }
    Matrix4 transpose();
    Vect4& operator*(Vect4& other);
    Matrix4& operator*(Matrix4& other);
    Matrix4& operator+(Matrix4& other);
    Matrix4& operator-(Matrix4& other);

    float& operator()(unsigned i, unsigned j) //i,j=0..3
    {
        return m_elements[i*m_nbColumns + j];
    }

};


#endif // !MATRIX4_H
