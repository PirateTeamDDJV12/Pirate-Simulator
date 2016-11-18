#ifndef IGAME_TASK_HEADER
#define IGAME_TASK_HEADER

class IGameTask
{
public:
    virtual ~IGameTask()
    {}
public:
    // called when the task is added to the game loop
    virtual void init() = 0;

    // called when the task is removed from the game loop
    virtual void cleanup() = 0;

    // called each frame
    virtual void update() = 0;
};

#endif // IGAME_TASK_HEADER