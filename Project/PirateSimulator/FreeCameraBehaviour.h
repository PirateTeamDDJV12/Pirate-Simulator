#ifndef FREECAMERABEHAVIOUR_H
#define FREECAMERABEHAVIOUR_H

#include "Angle.h"
#include "BaseCamera.h"
#include "IBehaviour.h"

#include <chrono>

namespace PirateSimulator
{
    namespace cameraModule
    {
        class FreeCameraBehaviour : public IBehaviour
        {
        protected:
            cameraModule::Camera*   m_cameraComponent;

            AngleRad                m_rotationAroundX;
            AngleRad                m_rotationAroundY;
            float                   m_speed;
            float                   m_rotationSpeed;

            std::chrono::time_point<std::chrono::system_clock> m_lastTime;

        public:
            FreeCameraBehaviour(float speed = 50.0f, float rotationSpeed = 5.0f) : m_speed{speed}, m_rotationSpeed {rotationSpeed}
            {}

            void setGameObject(GameObject* parent);

            virtual void anime(float elapsedTime);

            virtual void move(Move::Translation::Direction direction);
            virtual void rotate(Move::Rotation::Direction rotation);
        };
    }
}

#endif //!FREECAMERABEHAVIOUR_H