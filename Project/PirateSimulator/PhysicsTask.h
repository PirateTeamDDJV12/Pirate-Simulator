#ifndef PHYSICS_TASK_HEADER
#define PHYSICS_TASK_HEADER
#include "IGameTask.h"
#include "PhysicsManager.h"

namespace PirateSimulator
{
    class PhysicsTask : public IGameTask
    {
    public:
        void init() override;
        void cleanup() override
        {
            PhysicsManager::singleton.reset();
        }
        void update() override;
    };
}

#endif // PHYSICS_TASK_HEADER