#include "CameraManager.h"

#include "LevelCameraBehaviour.h"
#include "FreeCameraBehaviour.h"
#include "ObjectCameraBehaviour.h"

#include <algorithm>

using PirateSimulator::CameraManager;

CameraManager CameraManager::singleton;



PirateSimulator::GameObjectRef CameraManager::createCamera(PirateSimulator::cameraModule::BaseCamera::type cameraType,
    const PirateSimulator::Transform& transform,
    const PirateSimulator::cameraModule::CameraProjectionParameters &camProjParameters,
    const PirateSimulator::cameraModule::CameraMovingParameters &camMovParameters,
    const std::string& name)
{
    PirateSimulator::GameObjectRef camera;

    camera = PirateSimulator::GameObjectRef(new PirateSimulator::GameObject(transform, name));

    camera->addComponent<PirateSimulator::cameraModule::BaseCamera>(
            new PirateSimulator::cameraModule::BaseCamera(
                camProjParameters, camMovParameters
            )
        );

    IBehaviour* cameraBehaviour;

    switch (cameraType)
    {
    case PirateSimulator::cameraModule::BaseCamera::FREE_CAMERA:
        cameraBehaviour = new PirateSimulator::cameraModule::FreeCameraBehaviour();
        camera->addComponent<PirateSimulator::IBehaviour>(cameraBehaviour);
        break;

    case PirateSimulator::cameraModule::BaseCamera::LEVEL_CAMERA:
        cameraBehaviour = new PirateSimulator::cameraModule::LevelCameraBehaviour();
        camera->addComponent<PirateSimulator::IBehaviour>(cameraBehaviour);
        break;

    case PirateSimulator::cameraModule::BaseCamera::OBJECT_CAMERA:
        cameraBehaviour = new PirateSimulator::cameraModule::ObjectCameraBehaviour();
        camera->addComponent<PirateSimulator::IBehaviour>(cameraBehaviour);
        break;

    default:
        m_cameraInfo.m_cameraBehavior = nullptr;
        return PirateSimulator::GameObjectRef();
    }

    m_cameraInfo.m_mainCamera       = camera;
    m_cameraInfo.m_cameraComponent  = camera->getComponent<cameraModule::BaseCamera>();
    m_cameraInfo.m_cameraComponent->setCameraType(cameraType);
    m_cameraInfo.m_cameraType       = cameraType;
    m_cameraInfo.m_cameraBehavior   = cameraBehaviour;

    return camera;
}

void CameraManager::setPairedTarget(PirateSimulator::GameObjectRef pairedTarget)
{
    if (m_cameraInfo.m_cameraType == PirateSimulator::cameraModule::BaseCamera::OBJECT_CAMERA)
    {
        m_cameraInfo.m_cameraBehavior->as<PirateSimulator::cameraModule::ObjectCameraBehaviour>()->setTarget(pairedTarget);
    }
    else if (m_cameraInfo.m_cameraType == PirateSimulator::cameraModule::BaseCamera::LEVEL_CAMERA)
    {
        m_cameraInfo.m_cameraBehavior->as<PirateSimulator::cameraModule::LevelCameraBehaviour>()->setTerrain(pairedTarget);
    }
    else
    {
        return;
    }

    m_cameraInfo.m_cameraTarget = pairedTarget;
}

void CameraManager::setMainCamera(PirateSimulator::GameObjectRef camera) noexcept
{
    if (camera)
    {
        cameraModule::BaseCamera* cameraComponent = camera->getComponent<cameraModule::BaseCamera>();

        if (cameraComponent)
        {
            m_cameraInfo.m_mainCamera = camera;
            m_cameraInfo.m_cameraComponent = cameraComponent;

            m_cameraInfo.m_cameraType = cameraComponent->getCameraType();
            m_cameraInfo.m_cameraTarget = cameraComponent->getTarget();
        }
    }
}
