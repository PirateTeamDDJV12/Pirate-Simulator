#include "StdAfx.h"

#include "GameObject.h"
#include "CameraManager.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"
#include <DirectXMath.h>

using namespace PirateSimulator;
using namespace cameraModule;
using namespace DirectX;


void GameObject::translate(float x, float y, float z)
{
    m_transform.m_position.vector4_f32[0] += x;
    m_transform.m_position.vector4_f32[1] += y;
    m_transform.m_position.vector4_f32[2] += z;

    m_transform.m_right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(m_transform.m_up, m_transform.m_forward));

    if (m_mesh)
    {
        float rotationAngle = DirectX::XMVector3AngleBetweenVectors(m_transform.m_forward, { 0,0,1,0 }).vector4_f32[0];

        if (m_transform.m_forward.vector4_f32[0] < 0)
        {
            rotationAngle = -rotationAngle;
        }

        m_mesh->setWorldMatrix(DirectX::XMMatrixRotationY(rotationAngle)
            * DirectX::XMMatrixTranslationFromVector(m_transform.m_position));
    }
}

void GameObject::translate(const DirectX::XMVECTOR &dir)
{
    translate(dir.vector4_f32[0], dir.vector4_f32[1], dir.vector4_f32[2]);
}

void GameObject::rotate(float angleX, float angleY)
{
    if (m_mesh)
    {
        m_mesh->setWorldMatrix(XMMatrixRotationX(angleX) * XMMatrixRotationY(angleY));
    }
}