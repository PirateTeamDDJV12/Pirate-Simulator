#include "CameraManager.h"

#include "IBehaviour.h"

#include "LevelCameraBehaviour.h"
#include "FreeCameraBehaviour.h"
#include "ObjectCameraBehaviour.h"

#include <string>

using PirateSimulator::CameraManager;

CameraManager CameraManager::singleton;



PirateSimulator::GameObjectRef CameraManager::createFreeCamera(
    const PirateSimulator::Transform& transform,
    const PirateSimulator::cameraModule::CameraProjectionParameters &camProjParameters,
    const std::string& name)
{
    PirateSimulator::GameObjectRef camera;

    camera = PirateSimulator::GameObjectRef(new PirateSimulator::GameObject(transform, name));

    camera->addComponent<PirateSimulator::cameraModule::Camera>(
        new PirateSimulator::cameraModule::Camera(
            camProjParameters
        )
        );

    IBehaviour* cameraBehaviour;
    cameraBehaviour = new PirateSimulator::cameraModule::FreeCameraBehaviour();
    camera->addComponent<PirateSimulator::IBehaviour>(cameraBehaviour);

    m_cameraInfo.m_mainCamera = camera;
    m_cameraInfo.m_cameraComponent = camera->getComponent<cameraModule::Camera>();
    m_cameraInfo.m_cameraBehavior = cameraBehaviour;

    return camera;
}

PirateSimulator::GameObjectRef CameraManager::createObjectCamera(
    const PirateSimulator::GameObjectRef& target,
    const PirateSimulator::Transform& transform,
    const PirateSimulator::cameraModule::CameraProjectionParameters &camProjParameters,
    const std::string& name)
{
    PirateSimulator::GameObjectRef camera;

    camera = PirateSimulator::GameObjectRef(new PirateSimulator::GameObject(transform, name));

    camera->addComponent<PirateSimulator::cameraModule::Camera>(
        new PirateSimulator::cameraModule::Camera(
            camProjParameters
        )
        );

    IBehaviour* cameraBehaviour;
    cameraBehaviour = new PirateSimulator::cameraModule::ObjectCameraBehaviour(target);
    camera->addComponent<PirateSimulator::IBehaviour>(cameraBehaviour);

    m_cameraInfo.m_mainCamera = camera;
    m_cameraInfo.m_cameraComponent = camera->getComponent<cameraModule::Camera>();
    m_cameraInfo.m_cameraBehavior = cameraBehaviour;

    return camera;
}

void CameraManager::setMainCamera(PirateSimulator::GameObjectRef camera) noexcept
{
    if(camera)
    {
        cameraModule::Camera* cameraComponent = camera->getComponent<cameraModule::Camera>();

        if(cameraComponent)
        {
            m_cameraInfo.m_mainCamera = camera;
            m_cameraInfo.m_cameraComponent = cameraComponent;
        }
    }
}
