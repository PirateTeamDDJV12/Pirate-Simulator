/*
Created by Sun-lay Gagneux
*/
#ifndef OBJECTCAMERABEHAVIOUR_H
#define OBJECTCAMERABEHAVIOUR_H


#include "IBehaviour.h"
#include "BaseCamera.h"

#include "GameObject.h"

#include <sstream>


namespace PirateSimulator
{
    namespace cameraModule
    {
        class ObjectCameraBehaviour : public IBehaviour
        {
        public:

            enum CameraState
            {
                ThirdPersonCamera,
                FirstPersonCamera
            };

        private:
            cameraModule::Camera* m_cameraComponent;

            bool                        m_firstPersonPositionOk;
            float                       m_maxAngleX;
            float                       m_minAngleX;
            float                       m_distanceFromTarget;
            float                       m_rotationAroundY;
            float                       m_rotationAroundX;
            float                       m_newAngleX;
            float                       m_newAngleY;
            float                       m_rotationSmooth;
            float                       m_translationSmooth;
            GameObjectRef               m_target;
            CameraState                 m_state;
            DirectX::XMVECTOR           m_desiredPosition;

        public:
            ObjectCameraBehaviour(GameObjectRef target) :
                m_firstPersonPositionOk{false},
                m_maxAngleX {-10.0f},
                m_minAngleX{-85.0f},
                m_distanceFromTarget{60.0f},
                m_rotationAroundY{0.0f},
                m_rotationAroundX{0.0f},
                m_newAngleX{0.0f},
                m_newAngleY{0.0f},
                m_rotationSmooth{0.1f},
                m_translationSmooth{0.1f},
                m_target{target},
                m_state{CameraState::ThirdPersonCamera}
            {}

            void setGameObject(GameObject* parent)
            {
                m_gameObject = parent;
                m_cameraComponent = m_gameObject->getComponent<Camera>();
            }

            void setTarget(GameObjectRef target)
            {
                m_target = target;
            }

            virtual void move(Move::Translation::Direction direction);

            virtual void rotate(Move::Rotation::Direction direction);

            virtual void anime(float elapsedTime);
        };
    }
}


#endif //!OBJECTCAMERABEHAVIOUR_H