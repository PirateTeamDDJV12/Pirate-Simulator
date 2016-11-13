#ifndef FREECAMERABEHAVIOUR_H
#define FREECAMERABEHAVIOUR_H

#include "Angle.h"
#include "BaseCamera.h"
#include "GameObject.h"
#include <chrono>

namespace PirateSimulator
{
    namespace cameraModule
    {
        class FreeCameraBehaviour : public IBehaviour
        {
        protected:
            cameraModule::BaseCamera* m_cameraComponent;

            AngleRad m_rotationAroundX;
            AngleRad m_rotationAroundY;

            std::chrono::time_point<std::chrono::system_clock> m_lastTime;

        public:
            void setGameObject(GameObject* parent)
            {
                m_gameObject = parent;
                m_cameraComponent = m_gameObject->getComponent<BaseCamera>();
            }

            virtual void anime(float ellapsedTime);

            virtual void move(Move::Translation::Direction direction);
            virtual void rotate(Move::Rotation::Direction rotation);
        };
    }
}

#endif //!FREECAMERABEHAVIOUR_H