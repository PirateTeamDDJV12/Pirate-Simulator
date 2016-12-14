#ifndef SOUND_TASK_HEADER
#define SOUND_TASK_HEADER
#include "IGameTask.h"

namespace PirateSimulator
{
    class SoundTask : public IGameTask
    {
    public:
        void init() override;
        void cleanup() override;
        void update() override;
    };
}

#endif // SOUND_TASK_HEADER