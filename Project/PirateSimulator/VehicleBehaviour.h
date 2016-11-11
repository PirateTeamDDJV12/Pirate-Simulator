#ifndef VEHICLE_H
#define VEHICLE_H

#include "IBehaviour.h"

namespace PirateSimulator
{
    class VehicleBehaviour : public IBehaviour
    {
    private:
        float rotation;
        float velocity;

    public:
        VehicleBehaviour() :
            rotation{ 0.5f },
            velocity{ 0.5f }
        {}

        virtual void anime(float ellapsedTime);
    };
}
#endif // !VEHICLE_H
