/*
Created by Sun-lay Gagneux
*/
#ifndef OBJECTCAMERABEHAVIOUR_H
#define OBJECTCAMERABEHAVIOUR_H


#include "IBehaviour.h"
#include "BaseCamera.h"

#include "GameObject.h"


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
            cameraModule::BaseCamera* m_cameraComponent;

            float m_offset;

        public:
            ObjectCameraBehaviour(float offset = DEFAULT_OFFSET) :
                m_offset{ offset }
            {}

            void setGameObject(GameObject* parent)
            {
                m_gameObject = parent;
                m_cameraComponent = m_gameObject->getComponent<BaseCamera>();
            }

            virtual void move(Move::Translation::Direction direction);

            virtual void rotate(Move::Rotation::Direction direction);

            virtual void anime(float ellapsedTime);
		};
	}
}


#endif //!OBJECTCAMERABEHAVIOUR_H