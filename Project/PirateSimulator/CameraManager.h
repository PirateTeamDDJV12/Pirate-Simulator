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
        cameraModule::BaseCamera::type getCameraType() const noexcept { return m_cameraInfo.m_cameraType; }


    private:
        CameraManager(const CameraManager&) = delete;
        CameraManager& operator=(const CameraManager&) = delete;


    public:
        void setMainCamera(GameObjectRef camera) noexcept;

        void setPairedTarget(GameObjectRef pairedTarget);

        GameObjectRef createCamera(cameraModule::BaseCamera::type cameraType,
            const Transform& transform,
            const cameraModule::CameraProjectionParameters &camProjParameters,
            const cameraModule::CameraMovingParameters &camMovParameters,
            const std::string& name);


        void animMainCamera()
        {
            m_cameraInfo.m_mainCamera->anime(0);
        }

        DirectX::XMMATRIX getMatView() const
        {
            return m_cameraInfo.m_cameraComponent->getViewMatrix();
        }

        DirectX::XMMATRIX getMatProj() const
        {
            return m_cameraInfo.m_cameraComponent->getProjMatrix();
        }

        DirectX::XMMATRIX getMatViewProj() const
        {
            return m_cameraInfo.m_cameraComponent->getViewProjMatrix();
        }
    };
}

#endif // !CAMERAMANAGER_H_INCLUDED