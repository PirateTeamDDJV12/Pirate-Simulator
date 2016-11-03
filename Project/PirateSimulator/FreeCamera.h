#ifndef FREECAMERA_H
#define FREECAMERA_H

#include "BaseCamera.h"
#include "Angle.h"

#include <chrono>

namespace PirateSimulator
{
    namespace cameraModule
    {
        class FreeCamera : public BaseCamera
        {
        private:
            AngleRad m_rotationAroundY;
            AngleRad m_rotationAroundX;


            std::chrono::time_point<std::chrono::system_clock> m_lastTime;

            DirectX::XMVECTOR m_rightDirection;


        public:
            FreeCamera(const CameraProjectionParameters& defaultParameters, const CameraMovingParameters& moveParams,
                       const DirectX::XMVECTOR camPos,
                       const DirectX::XMVECTOR camDir,
                       const DirectX::XMVECTOR camUp) :
                BaseCamera{defaultParameters, moveParams, camPos, camDir, camUp},
                m_rotationAroundX{},
                m_rotationAroundY{},
                m_lastTime{},
                m_rightDirection(DirectX::XMVector3Normalize(DirectX::XMVector3Cross(m_up, m_direction)))
            {
            }

            // Move camera
            virtual void move(Move::Translation::Direction direction);

            // Rotate camera around axis by degrees. Camera's position is a
            // pivot point of rotation, so it doesn't change
            virtual void rotate(Move::Rotation::Direction direction);

            // Change camera target position
            virtual void getTarget(const DirectX::XMVECTOR& target)
            {}

            virtual void lookAtTarget()
            {}
        };
    }
}

#endif //!FREECAMERA_H