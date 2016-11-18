#ifndef TASK_MANAGER_HEADER
#define TASK_MANAGER_HEADER
#include "../PetitMoteur3D/PetitMoteur3D/Singleton.h"
#include "IGameTask.h"
#include <vector>
#include <map>
#include <memory>
#include <type_traits>

namespace PirateSimulator
{
    class TaskManager : public PM3D::CSingleton<TaskManager>
    {
        friend class PM3D::CSingleton<TaskManager>;
        std::map<int, std::vector<std::unique_ptr<IGameTask>>> _tasks;
        TaskManager()
        {}

    public:
        using PM3D::CSingleton<TaskManager>::GetInstance;
        void init();
        void update();
        IGameTask* addTask(std::unique_ptr<IGameTask> iTask, int iPriority);
        std::unique_ptr<IGameTask> removeTask(IGameTask *iTask);
        void reset();
        int getPriority(IGameTask *iTask);
        void setPriority(IGameTask *iTask, int iPriority);

        template <class T>
        IGameTask* addTask(int iPriority)
        {
            static_assert(std::is_base_of<IGameTask, T>::value, "Not based of IGameTask");
            return addTask(std::make_unique<T>(), iPriority);
        }
    };
}

#endif //TASK_MANAGER_HEADER