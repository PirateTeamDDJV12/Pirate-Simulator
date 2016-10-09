#include "Matrix.h"
#include "Matrix4.h"
#include "Matrix3.h"
#include "Vect.h"
#include "Vect3.h"
#include "Vect4.h"
#include <iostream>


int main()
{

    float elem[9] = { 2,0,0,0,2,0,0,0,2 };
    Matrix3 I32 = Matrix3::Matrix3(elem);
    float elem2[9] = { 2,4,3,2,2,1,05,-2,2 };
    Matrix3 M = Matrix3::Matrix3(elem2);
    Matrix3 res = M*I32;
    for (unsigned int i=0;i<9;i++)
    {
        std::cout << res.elements()[i] << "\n";
    }
    system("pause");
    return 1;
}
