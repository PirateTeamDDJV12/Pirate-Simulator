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
        bool                     m_sprinting;
        float                    m_tangling;
        float                    m_speed;
        float                    m_smooth;
        const float              m_maxSpeed = 10000.0f;
        DirectX::XMVECTOR        m_desiredDirection;
        GameObjectRef            m_cameraRef;

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