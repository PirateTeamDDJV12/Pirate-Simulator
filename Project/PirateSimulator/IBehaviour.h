#ifndef _IBEHAVIOUR_H_
#define _IBEHAVIOUR_H_

#include "Component.h"


namespace PirateSimulator
{
    class IBehaviour : public Component
    {
    public:
        static std::string typeId() noexcept { return "BehaviourComponent"; }
        virtual std::string getTypeId() const noexcept { return IBehaviour::typeId(); }

        virtual void anime(float ellapsedTime) {};

        virtual void move(float x, float y, float z) {}
        virtual void rotate(float xAngle, float yAngle) {}
    };
}

#endif // !_IBEHAVIOUR_H_
