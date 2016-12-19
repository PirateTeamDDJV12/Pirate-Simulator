#include "BaseCamera.h"
#include "GameObject.h"

using namespace PirateSimulator;
using namespace PirateSimulator::cameraModule;



Camera::Camera(const CameraProjectionParameters& defaultParameters)
 :
    m_Parameters{ defaultParameters }
{
    this->initProjMatrix();
}


void Camera::initProjMatrix()
{
    m_proj = DirectX::XMMatrixPerspectiveFovLH(
        m_Parameters.angle,
        m_Parameters.aspectRatio(),
        m_Parameters.nearest,
        m_Parameters.farthest);
}


void Camera::onResize(float width, float height)
{
    m_Parameters.clientWidth = width;
    m_Parameters.clientHeight = height;
    this->initProjMatrix();
}

void Camera::setGameObject(GameObject* parent)
{
    m_gameObject = parent;

    this->updateViewMatrix();
}

void Camera::updateViewMatrix()
{
    setMatrixView(DirectX::XMMatrixLookToLH(
        m_gameObject->m_transform.getPosition(),
        m_gameObject->m_transform.getForward(),
        m_gameObject->m_transform.getUp())
    );
    //this->*m_pUpdateViewMatrix)();
}