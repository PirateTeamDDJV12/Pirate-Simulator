#include "stdafx.h"

#include "FreeCamera.h"


//#define TAKE_ELAPSED_TIME_INTO_ACCOUNT
//#define MODIFY_UP_VECTOR_AT_ROTATE


using namespace  PirateSimulator;
using namespace  cameraModule;


void FreeCamera::move(Move::Translation::Direction direction)
{
#ifdef TAKE_ELAPSED_TIME_INTO_ACCOUNT
    using namespace std::chrono;

    time_point<system_clock> nowTime = std::chrono::system_clock::now();

    float elapsedTime = duration_cast<milliseconds>(nowTime - m_lastTime).count() / 1000.f;
#endif //TAKE_ELAPSED_TIME_INTO_ACCOUNT

    switch (direction)
    {
    case PirateSimulator::Move::Translation::FORWARD:
        m_position += m_direction * m_moveParams.translationVelocity 
#ifdef TAKE_ELAPSED_TIME_INTO_ACCOUNT
            * elapsedTime
#endif //TAKE_ELAPSED_TIME_INTO_ACCOUNT
            ;
        break;

    case PirateSimulator::Move::Translation::BACKWARD:
        m_position -= m_direction * m_moveParams.translationVelocity 
#ifdef TAKE_ELAPSED_TIME_INTO_ACCOUNT
            * elapsedTime
#endif //TAKE_ELAPSED_TIME_INTO_ACCOUNT
            ;
        break;

    case PirateSimulator::Move::Translation::LEFT:
        m_position -= m_rightDirection * m_moveParams.translationVelocity
#ifdef TAKE_ELAPSED_TIME_INTO_ACCOUNT
            * elapsedTime
#endif //TAKE_ELAPSED_TIME_INTO_ACCOUNT
            ;
        break;

    case PirateSimulator::Move::Translation::RIGHT:
        m_position += m_rightDirection * m_moveParams.translationVelocity
#ifdef TAKE_ELAPSED_TIME_INTO_ACCOUNT
            * elapsedTime
#endif //TAKE_ELAPSED_TIME_INTO_ACCOUNT
            ;
        break;

    case PirateSimulator::Move::Translation::UP:
        m_position.vector4_f32[1] += m_moveParams.translationVelocity 
#ifdef TAKE_ELAPSED_TIME_INTO_ACCOUNT
            * elapsedTime
#endif //TAKE_ELAPSED_TIME_INTO_ACCOUNT
            ;
        break;

    case PirateSimulator::Move::Translation::DOWN:
        m_position.vector4_f32[1] -= m_moveParams.translationVelocity 
#ifdef TAKE_ELAPSED_TIME_INTO_ACCOUNT
            * elapsedTime
#endif //TAKE_ELAPSED_TIME_INTO_ACCOUNT
            ;
        break;

    case PirateSimulator::Move::Translation::NONE:
    default:
        return;
    }

#ifdef TAKE_ELAPSED_TIME_INTO_ACCOUNT
    m_lastTime = nowTime;
#endif //TAKE_ELAPSED_TIME_INTO_ACCOUNT

    setMatrixView(XMMatrixLookToLH(m_position, m_direction, m_up));
}


void FreeCamera::rotate(Move::Rotation::Direction direction)
{
    using namespace std::chrono;

    time_point<system_clock> nowTime = std::chrono::system_clock::now();

    float elapsedTime = duration_cast<milliseconds>(nowTime - m_lastTime).count() / 1000.f;

    switch (direction)
    {
    case PirateSimulator::Move::Rotation::X_CLOCKWISE:
        m_rotationAroundX -= m_moveParams.rotationVelocity
#ifdef TAKE_ELAPSED_TIME_INTO_ACCOUNT
            * elapsedTime
#endif //TAKE_ELAPSED_TIME_INTO_ACCOUNT
            ;
        break;

    case PirateSimulator::Move::Rotation::X_INVERT_CLOCKWISE:
        m_rotationAroundX += m_moveParams.rotationVelocity 
#ifdef TAKE_ELAPSED_TIME_INTO_ACCOUNT
            * elapsedTime
#endif //TAKE_ELAPSED_TIME_INTO_ACCOUNT
            ;
        break;

    case PirateSimulator::Move::Rotation::Y_CLOCKWISE:
        m_rotationAroundY -= m_moveParams.rotationVelocity 
#ifdef TAKE_ELAPSED_TIME_INTO_ACCOUNT
            * elapsedTime
#endif //TAKE_ELAPSED_TIME_INTO_ACCOUNT
            ;
        break;

    case PirateSimulator::Move::Rotation::Y_INVERT_CLOCKWISE:
        m_rotationAroundY += m_moveParams.rotationVelocity 
#ifdef TAKE_ELAPSED_TIME_INTO_ACCOUNT
            * elapsedTime
#endif //TAKE_ELAPSED_TIME_INTO_ACCOUNT
            ;
        break;

    case PirateSimulator::Move::Rotation::Z_CLOCKWISE:
    case PirateSimulator::Move::Rotation::Z_INVERT_CLOCKWISE:
    case PirateSimulator::Move::Rotation::NONE:
    default:
        return;
    }

#ifdef TAKE_ELAPSED_TIME_INTO_ACCOUNT
    m_lastTime = nowTime;
#endif //TAKE_ELAPSED_TIME_INTO_ACCOUNT

     
    const float cosAngleAroundX = cosf(m_rotationAroundX.getAngle()); // cos(b)
    const float sinAngleAroundX = sinf(m_rotationAroundX.getAngle()); // sin(b)
    const float cosAngleAroundY = cosf(m_rotationAroundY.getAngle()); // cos(a)
    const float sinAngleAroundY = sinf(m_rotationAroundY.getAngle()); // sin(a)


    /* // 2 composition XY Rotation Matrix : Works. Good Formula but works only for cos and sin of angle difference (if raw angle->momentum effect)
    DirectX::XMMATRIX rotationMatrix
    {
        cosAngleAroundY     ,   sinAngleAroundX * sinAngleAroundY   ,    cosAngleAroundX * sinAngleAroundY  ,   0.f,

              0.f           ,             cosAngleAroundX           ,            -sinAngleAroundX           ,   0.f,

        -sinAngleAroundY    ,   sinAngleAroundX * cosAngleAroundY   ,    cosAngleAroundX * cosAngleAroundY  ,   0.f,

              0.f           ,                  0.f                  ,                   0.f                 ,   1.f
    };

    */
// // Teacher formula 
//
//     float AngleVision;
// 
// 
//     AngleVision = AngleVision + vitesseRotation * tempsEcoule;
// 
// 
// 
//     m_direction.vector4_f32[0] = cosf(AngleVision);
//     m_direction.vector4_f32[1] = 0.0f;
//     m_direction.vector4_f32[2] = sinf(AngleVision);

    // Composition with rotation matrix
    /*m_direction = DirectX::XMVector4Transform(m_direction, rotationMatrix);
    m_rightDirection = DirectX::XMVector4Transform(m_rightDirection, rotationMatrix);*/


    //z'' = (- x * sin(a)) + (y * sin(b) * cos(a)) + (z * cos(a) * cos(b))
    m_direction.vector4_f32[0] = -sinAngleAroundY;
    m_direction.vector4_f32[1] = cosAngleAroundY * sinAngleAroundX;
    m_direction.vector4_f32[2] = cosAngleAroundY * cosAngleAroundX;

    //x'' = (x * cos(a)) + (y * sin(b) * sin(a)) + (z * sin(a) * cos(b))
    m_rightDirection.vector4_f32[0] = cosAngleAroundY;
    m_rightDirection.vector4_f32[1] = sinAngleAroundX * sinAngleAroundY;
    m_rightDirection.vector4_f32[2] = sinAngleAroundY * cosAngleAroundX;

#ifdef MODIFY_UP_VECTOR_AT_ROTATE
    //y'' = (x * 0) + (y * cos(b)) + (- z * sin(b))
    m_up.vector4_f32[0] = 0.f;
    m_up.vector4_f32[1] = cosAngleAroundX;
    m_up.vector4_f32[2] = - sinAngleAroundX;
#endif //MODIFY_UP_VECTOR_AT_ROTATE

    setMatrixView(XMMatrixLookToLH(m_position, m_direction, m_up)); // OK
}


/*
void FreeCamera::rotate(Move::Rotation::Direction direction)
{
    float cosAngleAroundX = 0.f; // cos(b)
    float sinAngleAroundX = 0.f; // sin(b)
    float cosAngleAroundY = 0.f; // cos(a)
    float sinAngleAroundY = 0.f; // sin(a)


    switch (direction)
    {
    case PirateSimulator::Move::Rotation::X_CLOCKWISE:
        m_rotationAroundX -= m_moveParams.rotationVelocity;

        cosAngleAroundX = cosf(m_rotationAroundX.getAngle()); // cos(b)
        sinAngleAroundX = sinf(m_rotationAroundX.getAngle()); // sin(b)

        
        m_direction.vector4_f32[1] = sinAngleAroundX;
        m_direction.vector4_f32[2] = cosAngleAroundX;
        break;

    case PirateSimulator::Move::Rotation::X_INVERT_CLOCKWISE:
        m_rotationAroundX += m_moveParams.rotationVelocity;

        cosAngleAroundX = cosf(m_rotationAroundX.getAngle()); // cos(b)
        sinAngleAroundX = sinf(m_rotationAroundX.getAngle()); // sin(b)

        m_direction.vector4_f32[1] = sinAngleAroundX;
        m_direction.vector4_f32[2] = cosAngleAroundX;
        break;

    case PirateSimulator::Move::Rotation::Y_CLOCKWISE:
        m_rotationAroundY -= m_moveParams.rotationVelocity;

        cosAngleAroundY = cosf(m_rotationAroundY.getAngle()); // cos(a)
        sinAngleAroundY = sinf(m_rotationAroundY.getAngle()); // sin(a)

        m_direction.vector4_f32[0] = -sinAngleAroundY;
        m_direction.vector4_f32[2] = cosAngleAroundY;
        break;

    case PirateSimulator::Move::Rotation::Y_INVERT_CLOCKWISE:
        m_rotationAroundY += m_moveParams.rotationVelocity;

        cosAngleAroundY = cosf(m_rotationAroundY.getAngle()); // cos(a)
        sinAngleAroundY = sinf(m_rotationAroundY.getAngle()); // sin(a)

        m_direction.vector4_f32[0] = -sinAngleAroundY;
        m_direction.vector4_f32[2] = cosAngleAroundY;
        break;

    case PirateSimulator::Move::Rotation::Z_CLOCKWISE:
    case PirateSimulator::Move::Rotation::Z_INVERT_CLOCKWISE:
    case PirateSimulator::Move::Rotation::NONE:
    default:
        return;
    }


    

    //x'' = (x * cos(a)) + (y * sin(b) * sin(a)) + (z * sin(a) * cos(b))
    m_rightDirection.vector4_f32[0] = cosAngleAroundY;
    m_rightDirection.vector4_f32[1] = sinAngleAroundX * sinAngleAroundY;
    m_rightDirection.vector4_f32[2] = sinAngleAroundY * cosAngleAroundX;

#ifdef MODIFY_UP_VECTOR_AT_ROTATE
    //y'' = (x * 0) + (y * cos(b)) + (- z * sin(b))
    m_up.vector4_f32[0] = 0.f;
    m_up.vector4_f32[1] = cosAngleAroundX;
    m_up.vector4_f32[2] = -sinAngleAroundX;
#endif //MODIFY_UP_VECTOR_AT_ROTATE

    setMatrixView(XMMatrixLookToLH(m_position, m_direction, m_up)); // OK
}*/