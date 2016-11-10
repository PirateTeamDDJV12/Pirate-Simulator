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
            enum
            {
                HEIGHT_OFFSET = 10
            };

        private:
            cameraModule::BaseCamera* m_cameraComponent;

            AngleRad m_rotationAroundY;
            AngleRad m_rotationAroundX;
            Terrain* m_terrain;
            float m_offsetCam;

        public:
            LevelCameraBehaviour(float heightOffset = HEIGHT_OFFSET) :
                m_offsetCam{ heightOffset }
            {}

            void setGameObject(GameObject* parent)
            {
                m_gameObject = parent;
                m_cameraComponent = m_gameObject->getComponent<BaseCamera>();
            }

            void move(Move::Translation::Direction direction);
            void rotate(Move::Rotation::Direction direction);
            void anime(float ellapsedTime);

            void setTerrain(GameObject* fieldObject)
            {
                m_terrain = fieldObject->getComponent<IMesh>()->as<Terrain>();
            }
        };
    }
}


#endif //!LEVELCAMERABEHAVIOUR_H