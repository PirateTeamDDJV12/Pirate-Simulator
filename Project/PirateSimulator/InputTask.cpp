#include "InputTask.h"
#include "InputManager.h"

using namespace PirateSimulator;

void InputTask::update()
{
    InputManager::singleton.update();
}
