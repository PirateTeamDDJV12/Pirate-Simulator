#include "StdAfx.h"

#include "BaseCamera.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"

using namespace PirateSimulator;
using namespace PirateSimulator::cameraModule;



BaseCamera::BaseCamera(const CameraProjectionParameters& defaultParameters, const CameraMovingParameters& moveParams, const Transform &transform) :
    GameObject(transform),
    m_Parameters{ defaultParameters },
    m_moveParams{ moveParams }
{
    m_transform.m_right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(m_transform.m_up, m_transform.m_forward));

    setMatrixView(XMMatrixLookToLH(m_transform.m_position, m_transform.m_forward, m_transform.m_up));

    this->initViewMatrix();
    this->initProjMatrix();
}


void BaseCamera::initViewMatrix()
{}


void BaseCamera::initProjMatrix()
{
    m_proj = XMMatrixPerspectiveFovLH(
        m_Parameters.angle,
        m_Parameters.aspectRatio(),
        m_Parameters.nearest,
        m_Parameters.farthest);
}


void BaseCamera::onResize(unsigned int width, unsigned int height)
{
    m_Parameters.clientWidth = width;
    m_Parameters.clientHeight = height;
    initProjMatrix();
}


void BaseCamera::position(const DirectX::XMVECTOR& position)
{
    m_transform.m_position = position;

    setMatrixView(XMMatrixLookToLH(m_transform.m_position,
        m_transform.m_forward,
        m_transform.m_up));
}