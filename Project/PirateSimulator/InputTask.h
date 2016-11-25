#ifndef INPUT_TASK_HEADER
#define INPUT_TASK_HEADER
#include "IGameTask.h"

namespace PirateSimulator
{
    class InputTask : public IGameTask
    {
    public:
        void init() override;
        void cleanup() override
        {}
        void update() override;
    };
}

#endif // INPUT_TASK_HEADER