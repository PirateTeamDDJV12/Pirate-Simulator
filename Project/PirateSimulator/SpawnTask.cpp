#include "SpawnTask.h"
#include "GameObjectManager.h"

void PirateSimulator::SpawnTask::update()
{
    GameObjectManager::singleton.unspawnCoinsList();
}
