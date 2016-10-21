#include "stdafx.h"

#include "FreeCamera.h"


//#define TAKE_ELAPSED_TIME_INTO_ACCOUNT


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

    m_lastTime = nowTime;

    const float cosAngleAroundX = cosf(m_rotationAroundX.getAngle());
    const float sinAngleAroundX = sinf(m_rotationAroundX.getAngle());
    const float cosAngleAroundY = cosf(m_rotationAroundY.getAngle());
    const float sinAngleAroundY = sinf(m_rotationAroundY.getAngle());



    DirectX::XMMATRIX rotationMatrix
    {
        cosAngleAroundY     ,   sinAngleAroundX * sinAngleAroundY   ,    cosAngleAroundX * sinAngleAroundY  ,   0.f,

              0.f           ,             cosAngleAroundX           ,            -sinAngleAroundX           ,   0.f,

        -sinAngleAroundY    ,   sinAngleAroundX * cosAngleAroundY   ,    cosAngleAroundX * cosAngleAroundY  ,   0.f,

              0.f           ,                  0.f                  ,                   0.f                 ,   1.f
    };

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

    m_direction = DirectX::XMVector4Transform(m_direction, rotationMatrix);
    m_rightDirection = DirectX::XMVector4Transform(m_rightDirection, rotationMatrix);

    setMatrixView(XMMatrixLookToLH(m_position, m_direction, m_up)); // OK
}