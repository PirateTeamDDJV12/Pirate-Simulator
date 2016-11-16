#ifndef PHYSICS_TASK_HEADER
#define PHYSICS_TASK_HEADER
#include "IGameTask.h"

namespace PirateSimulator
{
    class PhysicsTask : public IGameTask
    {
    public:
        void init() override
        {}
        void cleanup() override
        {}
        void update() override
        {}
    };
}

#endif // PHYSICS_TASK_HEADER