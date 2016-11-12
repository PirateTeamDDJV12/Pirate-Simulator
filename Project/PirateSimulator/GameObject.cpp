#include "stdafx.h"

#include "GameObject.h"
#include "CameraManager.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"

using namespace PirateSimulator;
using namespace cameraModule;
using namespace DirectX;


void GameObject::translate(float x, float y, float z)
{
    m_transform.m_position.vector4_f32[0] += x;
    m_transform.m_position.vector4_f32[1] += y;
    m_transform.m_position.vector4_f32[2] += z;

    m_transform.m_forward = DirectX::XMVector3Normalize({ x,0,z,0 });
    m_transform.m_right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(m_transform.m_up, m_transform.m_forward));

    if (m_mesh)
    {
        float rotationAngle = DirectX::XMVector3AngleBetweenVectors(m_transform.m_forward, { 0,0,1,0 }).vector4_f32[0];
        float rotationAngleR = DirectX::XMVector3AngleBetweenVectors(m_transform.m_right, { 1,0,0,0 }).vector4_f32[0];

        if (z > 0)
        {
            rotationAngle = -rotationAngle;
        }

        //CameraManager::singleton.getMainCameraGO()->m_transform
        m_mesh->setWorldMatrix(DirectX::XMMatrixRotationY(rotationAngle)
            * DirectX::XMMatrixTranslationFromVector(m_transform.m_position));
    }
}

void GameObject::rotate(float angleX, float angleY)
{
    if (m_mesh)
    {
        m_mesh->setWorldMatrix(XMMatrixRotationX(angleX) * XMMatrixRotationY(angleY));
    }
}