#include "Matrix.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Vect.h"
#include "Vect3.h"
#include "Vect4.h"
#include <array>
#include <iostream>


int main()
{

    std::array<float, 9>elem = { 2,-1,1,5,2,-4,2,3,2 };
    Matrix3 I32 = Matrix3::Matrix3(elem);
    std::array<float, 9> elem2 = { 2,4,3,2,2,1,5,-2,2 };
    Matrix3 M = Matrix3::Matrix3(elem2);
    Matrix3 res = I32.transpose();
    for (unsigned int i=0;i<9;i++)
    {
        std::cout << res.elements()[i] << "\n";
    }
    system("pause");


    std::array<float, 16>elem3 = { 2,-1,1,5,2,-4,2,3,2,-1,-1,-4,5,6,3,2 };
    Matrix4 I42 = Matrix4::Matrix4(elem3);
    std::array<float, 16> elem4 = { 2,4,3,2,2,-1,05,-2,2,-1,7,0,-3,-1,-2,1 };
    Matrix4 M4 = Matrix4::Matrix4(elem4);
    Matrix4 res4 = I42.transpose();
    for (unsigned int i = 0;i < 16;i++)
    {
        std::cout << res4.elements()[i] << "\n";
    }
    system("pause");
    return 1;
}
