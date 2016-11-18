#ifndef PLAYER_TASK_HEADER
#define PLAYER_TASK_HEADER
#include "IGameTask.h"

namespace PirateSimulator
{
    class PlayerTask : public IGameTask
    {
    public:
        void init() override
        {}
        void cleanup() override
        {}
        void update() override;
    };
}

#endif // PLAYER_TASK_HEADER