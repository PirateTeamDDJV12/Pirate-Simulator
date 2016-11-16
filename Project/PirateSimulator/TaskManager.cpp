#include "TaskManager.h"
#include <algorithm>

using namespace std;
using namespace PirateSimulator;

template<class T>
struct unique_ptr_equals_ptr_t
{
    T *_ptr;
    unique_ptr_equals_ptr_t(T *ptr) : _ptr(ptr)
    {}
    bool operator()(const unique_ptr<T> &rhs)
    {
        return rhs.get() == _ptr;
    }
};

template<class T>
unique_ptr_equals_ptr_t<T> unique_ptr_equals_ptr(T *ptr)
{
    return unique_ptr_equals_ptr_t<T>(ptr);
}

IGameTask* TaskManager::addTask(unique_ptr<IGameTask> iTask, int iPriority)
{
    IGameTask* ref = iTask.get();
    _tasks[iPriority].emplace_back(move(iTask));
    ref->init();
    return ref;
}

void TaskManager::init()
{
    for(pair<const int, vector<unique_ptr<IGameTask>>> &taskGroup : _tasks)
    {
        for(unique_ptr<IGameTask> &task : taskGroup.second)
        {
            task->init();
        }
    }
}

void TaskManager::update()
{
    for(pair<const int, vector<unique_ptr<IGameTask>>> &taskGroup : _tasks)
    {
        for(unique_ptr<IGameTask> &task : taskGroup.second)
        {
            task->update();
        }
    }
}

void TaskManager::reset()
{
    for(auto taskGroupIter = _tasks.rbegin(); taskGroupIter != _tasks.rend(); ++taskGroupIter)
    {
        for(auto taskIter = taskGroupIter->second.rbegin(); taskIter != taskGroupIter->second.rend(); ++taskIter)
        {
            (*taskIter)->cleanup();
        }
    }

    _tasks.clear();
}

int TaskManager::getPriority(IGameTask *iTask)
{
    for(auto &taskBucket : _tasks)
    {
        auto found = find_if(taskBucket.second.begin(), taskBucket.second.end(), unique_ptr_equals_ptr(iTask));
        if(found != taskBucket.second.end())
            return taskBucket.first;
    }

    return -1;
}

void TaskManager::setPriority(IGameTask *iTask, int iPriority)
{
    std::unique_ptr<IGameTask> task = removeTask(iTask);
    if(task)
    {
        addTask(move(task), iPriority);
    }
}

unique_ptr<IGameTask> TaskManager::removeTask(IGameTask *iTask)
{
    unique_ptr<IGameTask> task;

    for(pair<const int, vector<unique_ptr<IGameTask>>> &taskGroup : _tasks)
    {
        auto found = find_if(taskGroup.second.begin(), taskGroup.second.end(), unique_ptr_equals_ptr(iTask));
        if(found != taskGroup.second.end())
        {
            unique_ptr<IGameTask> ptr = move(*found);
            taskGroup.second.erase(found);
            ptr->cleanup();
            return ptr;
        }
    }

    return nullptr;
}