#include "stdafx.h"

#include "BaseCamera.h"

using namespace PirateSimulator;
using namespace PirateSimulator::cameraModule;
using namespace DirectX;



BaseCamera::BaseCamera(const CameraProjectionParameters& defaultParameters, const CameraMovingParameters& moveParams, const XMVECTOR camPos, const XMVECTOR camDir, const XMVECTOR camUp) :
    m_Parameters{defaultParameters},
    m_moveParams{moveParams}
{
    m_position = camPos;
    m_direction = camDir;
    m_up = camUp;

    setMatrixView(XMMatrixLookToLH(camPos, camDir, camUp));

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
    m_position = position;

    setMatrixView(XMMatrixLookToLH(position,
                                   m_direction,
                                   m_up));
}