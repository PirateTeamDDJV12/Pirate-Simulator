#include "PlayerTask.h"
#include "CameraManager.h"
#include "TimeManager.h"

using namespace PirateSimulator;

void PlayerTask::update()
{
    CameraManager::singleton.getMainCameraGO()->anime(TimeManager::GetInstance().getElapsedTimeFrame());
}
