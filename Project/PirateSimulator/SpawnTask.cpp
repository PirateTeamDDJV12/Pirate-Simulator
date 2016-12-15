#include "SpawnTask.h"
#include "GameObjectManager.h"

void PirateSimulator::SpawnTask::cleanup()
{
    GameObjectManager::singleton.cleanAllGameObjects();
}

void PirateSimulator::SpawnTask::update()
{
    GameObjectManager::singleton.unspawnCoinsList();
}
