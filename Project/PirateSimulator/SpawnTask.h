#ifndef SPAWNTASK_H_INCLUDED
#define SPAWNTASK_H_INCLUDED
#include "IGameTask.h"

namespace PirateSimulator
{
    class SpawnTask : public IGameTask
    {
    public:
        void init() override;
        void cleanup() override {}
        void update() override;
    };
}

#endif // SPAWNTASK_H_INCLUDED