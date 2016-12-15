#include "GameObject.h"
#include "CameraManager.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"
#include <DirectXMath.h>
#include "GameObjectManager.h"

using namespace PirateSimulator;
using namespace cameraModule;
using namespace DirectX;


void GameObject::translate(float x, float y, float z)
{
    m_transform.translate(XMVECTOR{ x, y, z });

    if (m_mesh)
    {
        //float angleAroundX = DirectX::XMVector3AngleBetweenVectors(m_transform.getRight(), {1,0,0,0}).vector4_f32[0];
        float angleAroundY = DirectX::XMVector3AngleBetweenVectors(m_transform.getForward(), {0,0,1,0}).vector4_f32[0];
        //float angleAroundZ = DirectX::XMVector3AngleBetweenVectors(m_transform.getUp(), {0,1,0,0}).vector4_f32[0];

        if (m_transform.getForward().vector4_f32[0] < 0)
        {
            angleAroundY = -angleAroundY;
        }

        m_mesh->setWorldMatrix(DirectX::XMMatrixRotationY(angleAroundY)
            * DirectX::XMMatrixTranslationFromVector(m_transform.getPosition()));
    }
}


void GameObject::translate(const DirectX::XMVECTOR &dir)
{
    translate(dir.vector4_f32[0], dir.vector4_f32[1], dir.vector4_f32[2]);
}

void GameObject::setPosition(float x, float y, float z)
{
    m_transform.setPosition(x, y, z);

    if(m_mesh)
    {
        float rotationAngle = DirectX::XMVector3AngleBetweenVectors(m_transform.getForward(), {0,0,1,0}).vector4_f32[0];

        if(m_transform.getForward().vector4_f32[0] < 0)
        {
            rotationAngle = -rotationAngle;
        }

        m_mesh->setWorldMatrix(DirectX::XMMatrixRotationY(rotationAngle)
                               * DirectX::XMMatrixTranslationFromVector(m_transform.getPosition()));
    }
}

void GameObject::setPosition(const DirectX::XMVECTOR &newPos)
{
    setPosition(newPos.vector4_f32[0], newPos.vector4_f32[1], newPos.vector4_f32[2]);
}

void GameObject::rotate(float angle, const XMVECTOR &axis)
{
   
}

void GameObject::cleanUp()
{
    RendererManager::singleton.removeAStaticSortableMesh(m_mesh);
    GameObjectManager::singleton.unspawnGameObject(m_name);
}
