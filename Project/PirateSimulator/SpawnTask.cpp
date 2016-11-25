#include "SpawnTask.h"

#include "GameObjectManager.h"
#include "TimeManager.h"

using namespace PirateSimulator;

void SpawnTask::init()
{
    GameObjectManager::singleton.init();
}


void SpawnTask::update()
{
    GameObjectManager::singleton.animAllGameObject(TimeManager::GetInstance().getElapsedTimeFrame());
}
