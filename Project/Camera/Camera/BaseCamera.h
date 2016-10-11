#ifndef BASECAMERA_H
#define BASECAMERA_H

#include "Component.h"
#include "Util.h"

#include <d3d11.h>
#include <iostream>

namespace pirateSimulator
{
    namespace cameraModule
    {
        class BaseCamera : public Component
        {
        private:
            DirectX::XMFLOAT3 mPosition;     // Camera's coordinates
            DirectX::XMFLOAT3 mTarget;       // View target's coordinates
            DirectX::XMFLOAT3 mUp;           // Camera's up vector end coordinates

                                             /*** Projection parameters ***/
            float mAngle;                    // Angle of view frustum
            float mClientWidth;              // Window's width
            float mClientHeight;             // Window's height
            float mNearest;                  // Nearest view frustum plane
            float mFarthest;                 // Farthest view frustum plane

            DirectX::XMFLOAT4X4  mView;      // View matrix
            DirectX::XMFLOAT4X4  mProj;      // Projection matrix
            DirectX::XMFLOAT4X4  mOrtho;     // Ortho matrix for drawing without tranformation

        public:
            BaseCamera(); // Constructs default camera looking at 0,0,0
            BaseCamera(const BaseCamera&);
            ~BaseCamera() {}

            BaseCamera& operator=(const BaseCamera&);

        private:
            void initViewMatrix();

        public:
            // Initialize camera's perspective Projection matrix
            void InitProjMatrix(const float, const float, const float, const float, const float);

            // Initialize camera's orthogonal projection
            void InitOrthoMatrix(const float, const float, const float, const float);

            // Resize matrices when window size changes
            void OnResize(unsigned int, unsigned int);


            /*** View matrix transformation interfaces ***/

            // Move camera
            void Move(DirectX::XMFLOAT3);

            // Rotate camera around axis by degrees. Camera's position is a
            // pivot point of rotation, so it doesn't change
            void Rotate(DirectX::XMFLOAT3, float);

            // Get camera position coordinates
            const DirectX::XMFLOAT3& Position() const { return mPosition; }

            // Set camera position coordinates
            void Position(DirectX::XMFLOAT3&);

            // Change camera target position
            void Target(DirectX::XMFLOAT3);

            // Get camera's target position coordinates
            const DirectX::XMFLOAT3& Target() const { return mTarget; }

            // Get camera's up vector
            const DirectX::XMFLOAT3 Up() { return MathVF(DirectX::XMVectorSubtract(MathFV(mUp), MathFV(mPosition))); }

            // Get camera's look at target vector
            const DirectX::XMFLOAT3 LookAtTarget() { return MathVF(DirectX::XMVectorSubtract(MathFV(mTarget), MathFV(mPosition))); }

            // Returns transposed camera's View matrix
            const DirectX::XMFLOAT4X4 View() { return MathMF(DirectX::XMMatrixTranspose(MathFM(mView))); }


            /*** Projection matrix transformation interfaces ***/

            // Set view frustum's angle
            void Angle(float);

            // Get view frustum's angle
            const float& Angle() const { return mAngle; }

            // Set nearest culling plane distance from view frustum's projection plane
            void NearestPlane(float);

            // Set farthest culling plane distance from view frustum's projection plane
            void FarthestPlane(float);

            // Returns transposed camera's Projection matrix
            const DirectX::XMFLOAT4X4 Proj() { return MathMF(DirectX::XMMatrixTranspose(MathFM(mProj))); }

            // Returns transposed orthogonal camera matrix
            const DirectX::XMFLOAT4X4 Ortho() { return MathMF(DirectX::XMMatrixTranspose(MathFM(mOrtho))); }
        };
    }
}

#endif //!BASECAMERA_H