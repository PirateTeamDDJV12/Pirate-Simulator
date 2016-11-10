#include "StdAfx.h"

#include "BaseCamera.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"

using namespace PirateSimulator;
using namespace PirateSimulator::cameraModule;



BaseCamera::BaseCamera(const CameraProjectionParameters& defaultParameters, 
                       const CameraMovingParameters& moveParams,
                       GameObject* target = nullptr
    ) :
        m_Parameters{ defaultParameters },
        m_moveParams{ moveParams }
{
    setTarget(target);
    this->initProjMatrix();
}


void BaseCamera::initProjMatrix()
{
    m_proj = XMMatrixPerspectiveFovLH(
        m_Parameters.angle,
        m_Parameters.aspectRatio(),
        m_Parameters.nearest,
        m_Parameters.farthest);
}


void BaseCamera::onResize(float width, float height)
{
    m_Parameters.clientWidth = width;
    m_Parameters.clientHeight = height;
    this->initProjMatrix();
}

void BaseCamera::setGameObject(GameObject* parent)
{
    m_gameObject = parent;

    this->updateViewMatrix();
}