#ifndef FREECAMERA_H
#define FREECAMERA_H

#include "BaseCamera.h"

namespace PirateSimulator
{
    namespace cameraModule
    {
        class FreeCamera : public BaseCamera
        {
        public:
            FreeCamera(const CameraProjectionParameters& defaultParameters, const CameraMovingParameters& moveParams,
                       const DirectX::XMVECTOR camPos,
                       const DirectX::XMVECTOR camDir,
                       const DirectX::XMVECTOR camUp) :
                BaseCamera{defaultParameters, moveParams, camPos, camDir, camUp}
            {}

            // Move camera
            virtual void move(const DirectX::XMFLOAT3& direction);

            // Rotate camera around axis by degrees. Camera's position is a
            // pivot point of rotation, so it doesn't change
            virtual void rotate(const DirectX::XMFLOAT3& axis);

            // Change camera target position
            virtual void getTarget(const DirectX::XMVECTOR& target)
            {}

            virtual void lookAtTarget()
            {}
        };
    }
}

#endif //!FREECAMERA_H