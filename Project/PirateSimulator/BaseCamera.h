#ifndef CAMERA_H
#define CAMERA_H

#include "Component.h"
#include "GameObject.h"

#include <type_traits>

#include <directxmath.h>


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
                angle{ angle },
                nearest{ nearest },
                farthest{ farthest },
                clientWidth{ clientWidth },
                clientHeight{ clientHeight }
            {}

            const float aspectRatio() const noexcept
            {
                return clientWidth / clientHeight;
            }
        };

        class Camera : public Component
        {
        private:
            void (Camera::* m_pUpdateViewMatrix)();


        public:
            template<class Origin>
            Origin* as()
            {
                static_assert(std::is_convertible<Origin*, Camera*>::value, "Error, wrong camera casting");
                return static_cast<Origin*>(this);
            }


        protected:
            CameraProjectionParameters m_Parameters;

            DirectX::XMMATRIX  m_view;      // View matrix
            DirectX::XMMATRIX  m_proj;      // Projection matrix

        public:
            Camera(
                const CameraProjectionParameters& defaultProjParameters);

            static std::string typeId() noexcept { return "CameraComponent"; }
            virtual std::string getTypeId() const noexcept { return Camera::typeId(); }

            void updateViewMatrix() {
                setMatrixView(XMMatrixLookToLH(
                    m_gameObject->m_transform.getPosition(),
                    m_gameObject->m_transform.getForward(),
                    m_gameObject->m_transform.getUp())
                );
                //this->*m_pUpdateViewMatrix)();
            }

        public:
            // Initialize camera's perspective Projection matrix
            void initProjMatrix();

            // Resize matrices when window size changes
            void onResize(float width, float height);

            // Returns transposed camera's View matrix
            const DirectX::XMMATRIX& getViewMatrix() const noexcept { return m_view; }

            // Returns transposed camera's Projection matrix
            const DirectX::XMMATRIX& getProjMatrix() const noexcept { return m_proj; }

            DirectX::XMMATRIX getViewProjMatrix() const { return m_view * m_proj; }

            void setMatrixView(DirectX::XMMATRIX& mat) { m_view = mat; }

            // Returns transposed camera's Projection matrix
            void setMatrixProj(DirectX::XMMATRIX& mat) { m_proj = mat; }


            /*** Projection matrix transformation interfaces ***/

            // Set view frustum's angle
            void setAngle(float angle)
            {
                m_Parameters.angle = angle;
                initProjMatrix();
            }

            // Get view frustum's angle
            const float getAngle() const noexcept { return m_Parameters.angle; }

            // Set nearest culling plane distance from view frustum's projection plane
            void setNearestPlane(float nearest)
            {
                m_Parameters.nearest = nearest;
                initProjMatrix();
            }

            // Set farthest culling plane distance from view frustum's projection plane
            void setFarthestPlane(float farthest)
            {
                m_Parameters.farthest = farthest;
                initProjMatrix();
            }

            // Get nearest culling plane distance from view frustum's projection plane
            const float getNearestPlane() const noexcept { return m_Parameters.nearest; }

            // Get farthest culling plane distance from view frustum's projection plane
            const float getFarthestPlane() const noexcept { return m_Parameters.farthest; }

        public:
            virtual void setGameObject(GameObject* parent);
        };
    }
}

#endif //!CAMERA_H