#ifndef RENDER_TASK_HEADER
#define RENDER_TASK_HEADER
#include "IGameTask.h"

namespace PirateSimulator
{
    class RenderTask : public IGameTask
    {
    public:
        void init() override;
        
        void cleanup() override
        {}
        void update() override;
    };
}

#endif // RENDER_TASK_HEADER