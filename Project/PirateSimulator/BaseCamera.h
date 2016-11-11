#ifndef BASECAMERA_H
#define BASECAMERA_H

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

        struct CameraMovingParameters
        {
            float translationVelocity;
            float rotationVelocity;

            CameraMovingParameters(float translationVelocity, float rotationVelocity) :
                translationVelocity{ translationVelocity },
                rotationVelocity{ rotationVelocity }
            {}

            void setTranslationVelocity(float velocity)
            {
                translationVelocity = velocity;
            }

            void setRotationVelocity(float velocity)
            {
                rotationVelocity = velocity;
            }
        };


        class BaseCamera : public Component
        {
        public:
            enum
            {
                HEIGHT_OFFSET = 10
            };

            enum type
            {
                FREE_CAMERA,
                LEVEL_CAMERA,
                OBJECT_CAMERA
            };

        private:
            void (BaseCamera::* m_pUpdateViewMatrix)();


        public:
            template<class Origin>
            Origin* as()
            {
                static_assert(std::is_convertible<Origin*, BaseCamera*>::value, "Error, wrong camera casting");
                return static_cast<Origin*>(this);
            }


        protected:
            CameraProjectionParameters m_Parameters;
            CameraMovingParameters m_moveParams;

            DirectX::XMMATRIX  m_view;      // View matrix
            DirectX::XMMATRIX  m_proj;      // Projection matrix

            GameObjectRef m_target;

            type m_cameraType;


        public:
            BaseCamera(
                const CameraProjectionParameters& defaultProjParameters,
                const CameraMovingParameters& moveParams);

            static std::string typeId() noexcept { return "CameraComponent"; }
            virtual std::string getTypeId() const noexcept { return BaseCamera::typeId(); }

            void setTarget(GameObjectRef target) noexcept
            {
                m_target = target;

                m_pUpdateViewMatrix = (m_target ? 
                    &BaseCamera::updateViewMatrixAsObjectCamera : &BaseCamera::updateViewMatrixAsForwardCamera);
            }

            GameObjectRef getTarget() noexcept { return m_target; }

            void updateViewMatrix() { (this->*m_pUpdateViewMatrix)(); }

            type getCameraType() const noexcept
            {
                return m_cameraType;
            }

            virtual void setCameraType(type cameraType) noexcept
            {
                m_cameraType = cameraType;
            }

        protected:
            virtual void updateViewMatrixAsForwardCamera()
            {
                setMatrixView(XMMatrixLookToLH(
                    m_gameObject->m_transform.m_position, 
                    m_gameObject->m_transform.m_forward, 
                    m_gameObject->m_transform.m_up)
                );
            }

            virtual void updateViewMatrixAsObjectCamera()
            {
                m_gameObject->m_transform.m_forward = DirectX::XMVector3Normalize(m_target->m_transform.m_position - m_gameObject->m_transform.m_position);

                m_gameObject->m_transform.m_position = m_target->m_transform.m_position - m_gameObject->m_transform.m_forward * 10;

                m_gameObject->m_transform.m_right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(m_gameObject->m_transform.m_up, m_gameObject->m_transform.m_forward));

                setMatrixView(XMMatrixLookAtLH(
                    m_gameObject->m_transform.m_position,
                    m_target->m_transform.m_position,
                    m_gameObject->m_transform.m_up));
            }

        public:
            // Initialize camera's perspective Projection matrix
            void initProjMatrix();

            // Resize matrices when window size changes
            void onResize(float width, float height);

            void changeVelocity()
            {
                if (m_moveParams.translationVelocity == 5.0f)
                {
                    m_moveParams.setTranslationVelocity(0.5f);
                    m_moveParams.setRotationVelocity(0.02f);
                }
                else
                {
                    m_moveParams.setTranslationVelocity(5.0f);
                }
            }

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


            /*Camera's moving parameters such as its translation and rotation velocity*/
            float getTranslationVelocity() const noexcept   { return m_moveParams.translationVelocity; }
            float getRotationVelocity() const noexcept      { return m_moveParams.rotationVelocity; }

            void setTranslationVelocity(float speed) noexcept { m_moveParams.translationVelocity = speed; }
            void setRotationVelocity(float speed) noexcept    { m_moveParams.rotationVelocity = speed; }

        public:
            virtual void setGameObject(GameObject* parent);
        };
    }
}

#endif //!BASECAMERA_H