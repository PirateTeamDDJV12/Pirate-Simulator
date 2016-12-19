#ifndef IBEHAVIOUR_H_INCLUDED
#define IBEHAVIOUR_H_INCLUDED

#include "Component.h"

#include "Moves.h"

namespace PirateSimulator
{
    class IBehaviour : public Component
    {
    public:
        static std::string typeId() noexcept { return "BehaviourComponent"; }
        virtual std::string getTypeId() const noexcept { return IBehaviour::typeId(); }

        //do the animation. By default it is an empty method
        virtual void anime(float elapsedTime) {};

        //for moving object
        virtual void move(float x, float y, float z) {}
        virtual void rotate(float xAngle, float yAngle) {}
        virtual void move(Move::Translation::Direction direction) {}
        virtual void rotate(Move::Rotation::Direction rotation) {}

    };
}

#endif // !IBEHAVIOUR_H_INCLUDED
