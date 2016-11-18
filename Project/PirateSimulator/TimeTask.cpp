#include "TimeTask.h"
#include "TimeManager.h"

using namespace PirateSimulator;

void TimeTask::init()
{
    // Initialisation du temps de jeu
    TimeManager::GetInstance().startGameTime();
}

void TimeTask::update()
{
    auto timeManager = &TimeManager::GetInstance();
    timeManager->fastSave();
    timeManager->update();
}
