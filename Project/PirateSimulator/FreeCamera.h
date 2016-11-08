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

        public:
            FreeCamera(const CameraProjectionParameters& defaultParameters, const CameraMovingParameters& moveParams,
                       const Transform &transform) :
                BaseCamera{ defaultParameters, moveParams, transform },
                m_rotationAroundX{},
                m_rotationAroundY{},
                m_lastTime{}
            {
            }

            // Move camera
            void move(Move::Translation::Direction direction) override;

            // Rotate camera around axis by degrees. Camera's position is a
            // pivot point of rotation, so it doesn't change
            void rotate(Move::Rotation::Direction direction) override;

            // Change camera target position
            virtual void getTarget(const DirectX::XMVECTOR& target)
            {}

            virtual void lookAtTarget()
            {}

            void listenInput() override;
        };
    }
}

#endif //!FREECAMERA_H