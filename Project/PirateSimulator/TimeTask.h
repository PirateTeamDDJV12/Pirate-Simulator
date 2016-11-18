#ifndef TIME_TASK_HEADER
#define TIME_TASK_HEADER
#include "IGameTask.h"

namespace PirateSimulator
{
    class TimeTask : public IGameTask
    {
    public:
        void init() override;
        void cleanup() override
        {}
        void update() override;
    };
}

#endif // TIME_TASK_HEADER