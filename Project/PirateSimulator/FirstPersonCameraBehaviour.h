/*
Created by Sun-lay Gagneux
*/
#ifndef _FIRSTPERSONCAMERABEHAVIOUR_H_
#define _FIRSTPERSONCAMERABEHAVIOUR_H_


#include "IBehaviour.h"
#include "BaseCamera.h"

#include "GameObject.h"

#include <sstream>

namespace PirateSimulator
{
	namespace cameraModule
	{
		class FirstPersonCameraBehaviour : public IBehaviour
		{
        private:
            cameraModule::Camera*   m_cameraComponent;

            const float             m_maxAngleX = 0.0f;
            const float             m_minAngleX = -10.0f;
            float                   m_rotationAroundY;
            float                   m_rotationAroundX;
            GameObjectRef           m_target;
            DirectX::XMVECTOR       m_offset;

        public:
            FirstPersonCameraBehaviour(GameObjectRef target, const DirectX::XMVECTOR &offset) :
                m_offset (offset),
                m_rotationAroundY{ 0.0f },
                m_rotationAroundX{ 0.0f },
                m_target {target}
            {

            }

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


#endif //!_FIRSTPERSONCAMERABEHAVIOUR_H_