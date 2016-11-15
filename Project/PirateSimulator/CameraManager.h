#ifndef CAMERAMANAGER_H_INCLUDED
#define CAMERAMANAGER_H_INCLUDED


#include "GameObject.h"
#include "BaseCamera.h"
#include "IBehaviour.h"

#include "LevelCameraBehaviour.h"
#include "FreeCameraBehaviour.h"
#include "ObjectCameraBehaviour.h"


namespace PirateSimulator
{
    struct CameraInfo
    {
        GameObjectRef m_mainCamera;
        cameraModule::BaseCamera* m_cameraComponent;
        IBehaviour* m_cameraBehavior;

        cameraModule::BaseCamera::type m_cameraType;
        GameObjectRef m_cameraTarget;
    };

    class CameraManager
    {
    public:
        static CameraManager singleton;


    private:
        CameraInfo m_cameraInfo;

        
    private:
        CameraManager(){}


    public:
        GameObjectRef getMainCameraGO() const noexcept { return m_cameraInfo.m_mainCamera; }
        cameraModule::BaseCamera* getMainCameraComponent() const noexcept { return m_cameraInfo.m_cameraComponent; }
        IBehaviour* getMainCameraBehaviour() const noexcept { return m_cameraInfo.m_cameraBehavior; }


    private:
        CameraManager(const CameraManager&) = delete;
        CameraManager& operator=(const CameraManager&) = delete;


    public:
        void setMainCamera(GameObjectRef camera) noexcept
        {
            if (camera)
            {
                cameraModule::BaseCamera* cameraComponent = camera->getComponent<cameraModule::BaseCamera>();

                if (cameraComponent)
                {
                    m_cameraInfo.m_mainCamera      = camera;
                    m_cameraInfo.m_cameraComponent = cameraComponent;

                    m_cameraInfo.m_cameraType   = cameraComponent->getCameraType();
                    m_cameraInfo.m_cameraTarget = cameraComponent->getTarget();

                    switch (m_cameraInfo.m_cameraType)
                    {
                    case cameraModule::BaseCamera::OBJECT_CAMERA:
                        m_cameraInfo.m_cameraBehavior = m_cameraInfo.m_mainCamera->getComponent<cameraModule::ObjectCameraBehaviour>();

                        if (!m_cameraInfo.m_cameraBehavior)
                        {
                            m_cameraInfo.m_cameraBehavior = camera->getComponent<cameraModule::ObjectCameraBehaviour>();
                        }
                        break;

                    case cameraModule::BaseCamera::LEVEL_CAMERA:
                        m_cameraInfo.m_cameraBehavior = m_cameraInfo.m_mainCamera->getComponent<cameraModule::LevelCameraBehaviour>();
                        break;

                    case cameraModule::BaseCamera::FREE_CAMERA:
                        m_cameraInfo.m_cameraBehavior = m_cameraInfo.m_mainCamera->getComponent<cameraModule::FreeCameraBehaviour>();

                    default:
                        m_cameraInfo.m_cameraBehavior = nullptr;
                        break;
                    }
                }
            }
        }
    };
}

#endif // !CAMERAMANAGER_H_INCLUDED