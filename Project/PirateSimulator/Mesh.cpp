#include "Mesh.h"

#include "GameObject.h"

using namespace PirateSimulator;



void IMesh::setGameObject(GameObject* parent)
{
    m_gameObject = parent;

    float rotationAngle = DirectX::XMVector3AngleBetweenVectors(m_gameObject->m_transform.getForward(), { 0,0,-1,0 }).vector4_f32[0];

    if (m_gameObject->m_transform.getForward().vector4_f32[0] < 0)
    {
        rotationAngle = -rotationAngle;
    }

    this->setWorldMatrix(DirectX::XMMatrixRotationY(rotationAngle)
        * DirectX::XMMatrixTranslationFromVector(m_gameObject->m_transform.getPosition()));
}
