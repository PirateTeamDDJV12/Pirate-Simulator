#include "InputTask.h"
#include "InputManager.h"
#include "..\PetitMoteur3D\PetitMoteur3D\MoteurWindows.h"

using namespace PirateSimulator;


void InputTask::init()
{
    PM3D::CMoteurWindows& winMoteur = PM3D::CMoteurWindows::GetInstance();
    // Initialisation de DirectInput
    PirateSimulator::InputManager::singleton.InitManipulator(winMoteur.getApplicationInstance(), winMoteur.getMainWindowHandle());
}

void InputTask::cleanup()
{}

void InputTask::update()
{
    InputManager::singleton.update();
}
