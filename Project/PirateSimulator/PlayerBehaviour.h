#ifndef _PLAYERBEHAVIOUR_H_
#define _PLAYERBEHAVIOUR_H_

#include "IBehaviour.h"
#include "BaseCamera.h"

#include "GameObject.h"

static bool movedBoat = false;

namespace PirateSimulator
{
    class PlayerBehaviour : public IBehaviour
    {
    private:
        float m_speed;
        GameObjectRef m_cameraRef;

    public:
        PlayerBehaviour();

        void setGameObject(GameObject* parent)
        {
            m_gameObject = parent;
        }

        virtual void move(Move::Translation::Direction direction);

        virtual void rotate(Move::Rotation::Direction direction);

        virtual void anime(float ellapsedTime);
    };

}


#endif //!_PLAYERBEHAVIOUR_H_