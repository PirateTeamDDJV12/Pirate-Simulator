#ifndef LEVELCAMERABEHAVIOUR_H
#define LEVELCAMERABEHAVIOUR_H


#include "Angle.h"
#include "BaseCamera.h"
#include "IBehaviour.h"
#include <memory>


namespace PirateSimulator
{
    class Terrain;

    namespace cameraModule
    {
        class LevelCameraBehaviour : public IBehaviour
        {
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

            void setGameObject(GameObject* parent);

            void move(Move::Translation::Direction direction);
            void rotate(Move::Rotation::Direction direction);
            void anime(float elapsedTime);

            void setTerrain(std::shared_ptr<GameObject> fieldObject);
        };
    }
}


#endif //!LEVELCAMERABEHAVIOUR_H