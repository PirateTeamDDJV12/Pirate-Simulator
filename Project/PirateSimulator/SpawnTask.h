#ifndef SPAWN_TASK_HEADER
#define SPAWN_TASK_HEADER
#include "IGameTask.h"

namespace PirateSimulator
{
    class SpawnTask : public IGameTask
    {
    public:
        void init() override
        {}
        void cleanup() override;
        void update() override;
    };
}

#endif // SPAWN_TASK_HEADER