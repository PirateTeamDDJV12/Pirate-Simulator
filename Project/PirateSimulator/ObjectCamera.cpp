#include "stdafx.h"

#include "ObjectCamera.h"
#include "../Chapitre 10/PetitMoteur3D/MoteurWindows.h"

using namespace PirateSimulator;
using namespace cameraModule;
using namespace DirectX;

void ObjectCamera::setTarget(Transform *target) noexcept
{
    m_target = target;

    m_transform.m_position = target->m_position - m_transform.m_forward * m_offset;
}

void ObjectCamera::move(Move::Translation::Direction direction)
{

}

void ObjectCamera::rotate(Move::Rotation::Direction direction)
{

}

void ObjectCamera::listenInput()
{

}
