#ifndef LEVELCAMERABEHAVIOUR_H
#define LEVELCAMERABEHAVIOUR_H


#include "Angle.h"
#include "Terrain.h"
#include "BaseCamera.h"
#include "GameObject.h"


namespace PirateSimulator
{
    namespace cameraModule
    {
        class LevelCameraBehaviour : public IBehaviour
        {
        public:


        private:
            cameraModule::Camera* m_cameraComponent;

            AngleRad m_rotationAroundY;
            AngleRad m_rotationAroundX;
            Terrain* m_terrain;
            float m_offsetCam;
            float m_speed;

        public:
            LevelCameraBehaviour(float heightOffset) :
                m_offsetCam{ 100.0f }, m_speed{50.0f}
            {}

            void setGameObject(GameObject* parent)
            {
                m_gameObject = parent;
                m_cameraComponent = m_gameObject->getComponent<Camera>();
            }

            void move(Move::Translation::Direction direction);
            void rotate(Move::Rotation::Direction direction);
            void anime(float elapsedTime);

            void setTerrain(GameObjectRef fieldObject)
            {
                m_terrain = fieldObject->getComponent<IMesh>()->as<Terrain>();
            }
        };
    }
}


#endif //!LEVELCAMERABEHAVIOUR_H