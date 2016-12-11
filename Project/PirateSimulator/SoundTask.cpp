#include "SoundTask.h"
#include "SoundManager.h"

using namespace PirateSimulator;


void SoundTask::init()
{
    SoundManager::singleton.init();

    SoundManager::singleton.loadMusicFromFile("PirateSimulator/UISoundtrack.mp3");
    SoundManager::singleton.playMusic(0);

    SoundManager::singleton.setVolume(1.0f);
}

void SoundTask::update()
{
    SoundManager::singleton.update();
}

void SoundTask::cleanup()
{
    SoundManager::singleton.cleanup();
}
