#ifndef CAMERAMANAGER_H_INCLUDED
#define CAMERAMANAGER_H_INCLUDED


#include "GameObject.h"
#include "BaseCamera.h"


namespace PirateSimulator
{
    class IBehaviour;

    struct CameraInfo
    {
        GameObjectRef m_mainCamera;
        cameraModule::Camera* m_cameraComponent;
        IBehaviour* m_cameraBehavior;

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
        cameraModule::Camera* getMainCameraComponent() const noexcept { return m_cameraInfo.m_cameraComponent; }
        IBehaviour* getMainCameraBehaviour() const noexcept { return m_cameraInfo.m_cameraBehavior; }


    private:
        CameraManager(const CameraManager&) = delete;
        CameraManager& operator=(const CameraManager&) = delete;


    public:
        void setMainCamera(GameObjectRef camera) noexcept;

        GameObjectRef createFreeCamera(
            const Transform& transform,
            const cameraModule::CameraProjectionParameters &camProjParameters,
            const std::string& name);

        GameObjectRef createObjectCamera(
            const PirateSimulator::GameObjectRef& target,
            const Transform& transform,
            const cameraModule::CameraProjectionParameters &camProjParameters,
            const std::string& name);


        void animMainCamera()
        {
            m_cameraInfo.m_mainCamera->anime(0);
        }

        const DirectX::XMMATRIX& getMatView() const
        {
            return m_cameraInfo.m_cameraComponent->getViewMatrix();
        }

        const DirectX::XMMATRIX& getMatProj() const
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