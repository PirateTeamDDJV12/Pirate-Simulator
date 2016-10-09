#ifndef MATRIX_H
#define MATRIX_H
#include "Matrix.h"
#include "Vect4.h"
//template<typename valueType>
class Matrix4 //: public Matrix
{
private:
    float m_elements[16];
    unsigned int m_nbRows = 4;
    unsigned int m_nbColumns = 4;
public:
    //Matrix4(Vect4<valueType> col1, Vect4<valueType> col2, Vect4<valueType> col3, Vect4<valueType> col4);
    Matrix4(Vect4 col1, Vect4 col2, Vect4 col3, Vect4 col4);
    //Matrix4(valueType elements[16]) : m_elements{ elements }, m_nbRows{ 4 }, M_nbColumns{ 4 } {}
    Matrix4(float elements[16]) : m_elements{ *elements }, m_nbRows{ 4 }, m_nbColumns{ 4 } {}
    unsigned int nbRows()
    {
        return m_nbRows;
    }
    unsigned int nbColumns()
    {
        return m_nbColumns;
    }
    float *elements()
    {
        return m_elements;
    }
    Matrix4 transpose();
    Vect4& operator*(Vect4& other);
    Matrix4& operator*(Matrix4& other);
    Matrix4& operator+(Matrix4& other);
    Matrix4& operator-(Matrix4& other);
    //Matrix4& operator-(Matrix4& other);
    float& operator()(unsigned i, unsigned j)
    {
        return m_elements[i*m_nbColumns + j];
    }
};


#endif // !MATRIX_H
