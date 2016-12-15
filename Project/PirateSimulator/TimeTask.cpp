#include "TimeTask.h"
#include "TimeManager.h"

using namespace PirateSimulator;

void TimeTask::init()
{}

void TimeTask::update()
{
    auto timeManager = &TimeManager::GetInstance();
    timeManager->fastSave();
    timeManager->update();
}
