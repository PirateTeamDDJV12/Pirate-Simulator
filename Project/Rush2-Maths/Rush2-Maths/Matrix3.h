#ifndef MATRIX3_H
#define MATRIX_H
#include "Matrix.h"
class Vect3;
//template<typename valueType>
class Matrix3 //: public Matrix//<valueType>
{
private:
    float m_elements[9];
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
    float *elements()
    {
        return m_elements;
    }
    //Matrix3(valueType elements[9]) : m_elements{ elements }, m_nbRows{ 3 }, M_nbColumns{ 3 } {}
    Matrix3(float elements[9]) : m_elements{ *elements }, m_nbRows{ 3 }, m_nbColumns{ 3 } {}
    Matrix3 transpose();
    Vect3& operator*(Vect3& other);
    Matrix3& operator*(Matrix3& other);
    Matrix3& Matrix3::operator+(Matrix3& other);
    Matrix3& Matrix3::operator-(Matrix3& other);
    float& operator()(unsigned i, unsigned j)
    {
        return m_elements[i*m_nbColumns + j];
    }
   // Matrix3& Matrix3::operator=(const Matrix3& other);
 
};



#endif // !MATRIX_H
