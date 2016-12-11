#include "SoundTask.h"
#include "SoundManager.h"

using namespace PirateSimulator;


void SoundTask::init()
{
    SoundManager::singleton.init();
}

void SoundTask::update()
{
    SoundManager::singleton.update();
}

void SoundTask::cleanup()
{
    SoundManager::singleton.cleanup();
}
