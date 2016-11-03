#include "stdafx.h"

#include <iostream>
#include <sstream>

#include "FreeCamera.h"

using namespace  PirateSimulator;
using namespace  cameraModule;


void FreeCamera::move(Move::Translation::Direction direction)
{
    switch (direction)
    {
    case PirateSimulator::Move::Translation::FORWARD:
        m_position += m_direction * m_moveParams.translationVelocity;
        break;

    case PirateSimulator::Move::Translation::BACKWARD:
        m_position -= m_direction * m_moveParams.translationVelocity;
        break;

    case PirateSimulator::Move::Translation::LEFT:
        m_position -= m_rightDirection * m_moveParams.translationVelocity;
        break;

    case PirateSimulator::Move::Translation::RIGHT:
        m_position += m_rightDirection * m_moveParams.translationVelocity;
        break;

    case PirateSimulator::Move::Translation::UP:
        m_position += m_up * m_moveParams.translationVelocity;
        break;

    case PirateSimulator::Move::Translation::DOWN:
        m_position -= m_up * m_moveParams.translationVelocity;
        break;

    case PirateSimulator::Move::Translation::NONE:
    default:
        return;
    }
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
        m_rotationAroundX -= m_moveParams.rotationVelocity;
        break;

    case PirateSimulator::Move::Rotation::X_INVERT_CLOCKWISE:
        m_rotationAroundX += m_moveParams.rotationVelocity;
        break;

    case PirateSimulator::Move::Rotation::Y_CLOCKWISE:
        m_rotationAroundY -= m_moveParams.rotationVelocity;
        break;

    case PirateSimulator::Move::Rotation::Y_INVERT_CLOCKWISE:
        m_rotationAroundY += m_moveParams.rotationVelocity;
        break;

    case PirateSimulator::Move::Rotation::Z_CLOCKWISE:
    case PirateSimulator::Move::Rotation::Z_INVERT_CLOCKWISE:
    case PirateSimulator::Move::Rotation::NONE:
    default:
        return;
    }
    if (DirectX::XMConvertToDegrees(m_rotationAroundX.getAngle()) < -89.0f)
        m_rotationAroundX = DirectX::XMConvertToRadians(-89.0f);

    else if (DirectX::XMConvertToDegrees(m_rotationAroundX.getAngle()) > 89.0f)
        m_rotationAroundX = DirectX::XMConvertToRadians(89.0f);

    m_direction.vector4_f32[0] = sin(m_rotationAroundY.getAngle()) * cos(m_rotationAroundX.getAngle());
    m_direction.vector4_f32[1] = sin(m_rotationAroundX.getAngle());
    m_direction.vector4_f32[2] = cos(m_rotationAroundX.getAngle()) * cos(m_rotationAroundY.getAngle());

    // Update the rightDirection vector when rotating
    m_rightDirection = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(m_up, m_direction));


#ifdef MODIFY_UP_VECTOR_AT_ROTATE
    //y'' = (x * 0) + (y * cos(b)) + (- z * sin(b))
    m_up.vector4_f32[0] = 0.f;
    m_up.vector4_f32[1] = cosAngleAroundX;
    m_up.vector4_f32[2] = -sinAngleAroundX;
#endif //MODIFY_UP_VECTOR_AT_ROTATE

    setMatrixView(XMMatrixLookToLH(m_position, m_direction, m_up));
}