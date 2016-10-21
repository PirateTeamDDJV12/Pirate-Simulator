#ifndef BASECAMERA_H
#define BASECAMERA_H

#include "Component.h"
#include "Moves.h"

#include <directxmath.h>
#include <d3d11.h>


namespace PirateSimulator
{
    namespace cameraModule
    {
        struct CameraProjectionParameters
        {
            float angle;                    // Angle of view frustum
            float nearest;                  // Nearest view frustum plane
            float farthest;                 // Farthest view frustum plane
            float clientWidth;              // Window's width
            float clientHeight;             // Window's height

            CameraProjectionParameters(float angle, float nearest, float farthest, float clientWidth, float clientHeight) :
                angle{angle},
                nearest{nearest},
                farthest{farthest},
                clientWidth{clientWidth},
                clientHeight{clientHeight}
            {}

            const float aspectRatio() const noexcept
            {
                return clientWidth / clientHeight;
            }
        };

        struct CameraMovingParameters
        {
            float translationVelocity;
            float rotationVelocity;

            CameraMovingParameters(float translationVelocity, float rotationVelocity) :
                translationVelocity{translationVelocity},
                rotationVelocity{rotationVelocity}
            {}
        };


        class BaseCamera : public Component
        {
        protected:
            DirectX::XMVECTOR m_position;     // Camera's coordinates
            DirectX::XMVECTOR m_direction;    // View at a specific direction
            DirectX::XMVECTOR m_target;       // View target's coordinates
            DirectX::XMVECTOR m_up;           // Camera's up vector end coordinates

            CameraProjectionParameters m_Parameters;
            CameraMovingParameters m_moveParams;

            DirectX::XMMATRIX  m_view;      // View matrix
            DirectX::XMMATRIX  m_proj;      // Projection matrix


        public:
            // Constructs default camera looking at 0,0,0
            BaseCamera(
                const CameraProjectionParameters& defaultProjParameters,
                const CameraMovingParameters& moveParams,
                const DirectX::XMVECTOR camPos,
                const DirectX::XMVECTOR camDir,
                const DirectX::XMVECTOR camUp);


        protected:
            void initViewMatrix();

        public:
            // Initialize camera's perspective Projection matrix
            void initProjMatrix();

            // Resize matrices when window size changes
            void onResize(unsigned int width, unsigned int height);


            /*** View matrix transformation interfaces ***/

            // Move camera
            virtual void move(Move::Translation::Direction direction) = 0;

            // Rotate camera around axis by degrees. Camera's position is a
            // pivot point of rotation, so it doesn't change
            virtual void rotate(Move::Rotation::Direction direction) = 0;

            // Get camera position coordinates
            const DirectX::XMVECTOR& position() const
            {
                return m_position;
            }

            // Set camera position coordinates
            void position(const DirectX::XMVECTOR& position);

            // Change camera target position
            virtual void getTarget(const DirectX::XMVECTOR& target) = 0;

            // Get camera's target position coordinates
            DirectX::XMVECTOR target() const
            {
                return m_target;
            }

            // Get camera's direction coordinates
            DirectX::XMVECTOR direction() const
            {
                return m_direction;
            }

            // Set camera's direction coordinates
            void direction(DirectX::XMVECTOR& direction)
            {
                m_direction = direction;
            }

            // Get camera's up vector
            DirectX::XMVECTOR up() const
            {
                return m_up;
            }

            // Get camera's look at target vector
            virtual void lookAtTarget() = 0;

            // Returns transposed camera's View matrix
            DirectX::XMMATRIX& view()
            {
                return m_view;
            }

            // Returns transposed camera's Projection matrix
            DirectX::XMMATRIX& proj()
            {
                return m_proj;
            }

            DirectX::XMMATRIX getViewProjMatrix() const
            {
                return m_view * m_proj;
            }

            void setMatrixView(DirectX::XMMATRIX& mat)
            {
                m_view = mat;
            }

            // Returns transposed camera's Projection matrix
            void setMatrixProj(DirectX::XMMATRIX& mat)
            {
                m_proj = mat;
            }


            /*** Projection matrix transformation interfaces ***/

            // Set view frustum's angle
            void angle(float angle)
            {
                m_Parameters.angle = angle;
                initProjMatrix();
            }

            // Get view frustum's angle
            const float angle() const
            {
                return m_Parameters.angle;
            }

            // Set nearest culling plane distance from view frustum's projection plane
            void nearestPlane(float nearest)
            {
                m_Parameters.nearest = nearest;
                initProjMatrix();
            }

            // Set farthest culling plane distance from view frustum's projection plane
            void farthestPlane(float farthest)
            {
                m_Parameters.farthest = farthest;
                initProjMatrix();
            }

            // Get nearest culling plane distance from view frustum's projection plane
            const float nearestPlane()
            {
                return m_Parameters.nearest;
            }

            // Get farthest culling plane distance from view frustum's projection plane
            const float farthestPlane()
            {
                return m_Parameters.farthest;
            }


            /*Camera's moving parameters such as its translation and rotation velocity*/

            float translationVelocity() const
            {
                return m_moveParams.translationVelocity;
            }

            float rotationVelocity() const
            {
                return m_moveParams.rotationVelocity;
            }

            void translationVelocity(float speed)
            {
                m_moveParams.translationVelocity = speed;
            }

            void rotationVelocity(float speed)
            {
                m_moveParams.rotationVelocity = speed;
            }
        };
    }
}

#endif //!BASECAMERA_H