#include "stdafx.h"

#include "GameObject.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"

using namespace PirateSimulator;
using namespace cameraModule;
using namespace DirectX;


void GameObject::translate(float x, float y, float z)
{
    m_transform.m_position.vector4_f32[0] += x;
    m_transform.m_position.vector4_f32[1] += y;
    m_transform.m_position.vector4_f32[2] += z;

    if (m_mesh)
    {
        m_mesh->setWorldMatrix(DirectX::XMMatrixTranslationFromVector(m_transform.m_position));
    }
}

void GameObject::rotate(float angleX, float angleY)
{
    if (m_mesh)
    {
        m_mesh->setWorldMatrix(XMMatrixRotationX(angleX) * XMMatrixRotationY(angleY));
    }
}