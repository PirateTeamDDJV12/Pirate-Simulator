#ifndef LEVELCAMERA_H
#define LEVELCAMERA_H


#include "AttachedCamera.h"
#include "Angle.h"
#include "Terrain.h"


namespace PirateSimulator
{
    namespace cameraModule
    {
        class LevelCamera : public BaseCamera
        {
        private:
            AngleRad m_rotationAroundY;
            AngleRad m_rotationAroundX;
            Terrain* m_terrain;
            float m_offsetCam;

        public:
            LevelCamera(const CameraProjectionParameters& defaultProjParameters,
                        const CameraMovingParameters& moveParams,
                        const DirectX::XMVECTOR& camPos,
                        const DirectX::XMVECTOR& camDir,
                        const DirectX::XMVECTOR& camUp)
                : BaseCamera(defaultProjParameters, moveParams, camPos, camDir, camUp),
                m_offsetCam{10.0f}
            {}

            void move(Move::Translation::Direction direction) override;
            void rotate(Move::Rotation::Direction direction) override;
            void listenInput() override;

            void setTerrain(Terrain* t)
            {
                m_terrain = t;
            }
        };
    }
}


#endif //!LEVELCAMERA_H