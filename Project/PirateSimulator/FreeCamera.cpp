#include "stdafx.h"

#include "FreeCamera.h"

using namespace  PirateSimulator::cameraModule;


void FreeCamera::move(const DirectX::XMFLOAT3& direction)
{
    m_position = m_position + XMLoadFloat3(
        &DirectX::XMFLOAT3(
            direction.x * m_moveParams.translationVelocity, 
            direction.y * m_moveParams.translationVelocity, 
            direction.z * m_moveParams.translationVelocity)
    );

    setMatrixView(XMMatrixLookToLH(m_position,
        m_direction,
        m_up));
}


void FreeCamera::rotate(const DirectX::XMFLOAT3& axis)
{


    //m_view = m_view + DirectX::XMMatrixRotationY(axis.y * m_moveParams.rotationVelocity) + DirectX::XMMatrixRotationX(axis.x * m_moveParams.rotationVelocity);
    

    // rotate vectors
    //DirectX::XMFLOAT3 look_at_target = MathVF(DirectX::XMVectorSubtract(MathFV(m_Target), MathFV(m_Position)));
    DirectX::XMFLOAT3 lookAtUp;
    DirectX::XMStoreFloat3(&lookAtUp, m_direction - m_position);


    //look_at_target = MathVF(DirectX::XMVector3Transform(MathFV(look_at_target),
        //DirectX::XMMatrixRotationAxis(MathFV(axis), DirectX::XMConvertToRadians(degrees))));

    XMStoreFloat3(&lookAtUp, DirectX::XMVector3Transform(XMLoadFloat3(&lookAtUp), DirectX::XMMatrixRotationAxis(XMLoadFloat3(&axis), DirectX::XMConvertToRadians(m_moveParams.rotationVelocity))));

    // restore vectors's end points mTarget and mUp from new rotated vectors
    //m_Target = MathVF(DirectX::XMVectorAdd(MathFV(m_Position), MathFV(look_at_target)));
    m_direction = m_position + XMLoadFloat3(&lookAtUp);

    setMatrixView(XMMatrixLookToLH(m_position,
        m_direction,
        m_up));
}