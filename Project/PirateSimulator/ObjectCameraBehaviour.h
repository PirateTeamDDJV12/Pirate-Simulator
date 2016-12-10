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
            enum
            {
                DEFAULT_OFFSET = 100
            };

        private:
            cameraModule::Camera* m_cameraComponent;

            const float m_maxAngleX = 0.0f;
            const float m_minAngleX = -85.0f;
            float m_offset;
            float m_rotationAroundY;
            float m_rotationAroundX;
            GameObjectRef m_target;

        public:
            ObjectCameraBehaviour(GameObjectRef target, float offset = DEFAULT_OFFSET) :
                m_offset{ offset },
                m_rotationAroundY{ 0.0f },
                m_rotationAroundX{ 0.0f },
                m_target {target}
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

            virtual void anime(float ellapsedTime);
		};
	}
}


#endif //!OBJECTCAMERABEHAVIOUR_H