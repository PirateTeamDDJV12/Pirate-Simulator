#include "GameLogic.h"

#include "TaskManager.h"

// Tasks
#include "TimeTask.h"
#include "InputTask.h"
#include "PhysicsTask.h"
#include "RenderTask.h"
#include "PlayerTask.h"
#include "SoundTask.h"
#include "SpawnTask.h"

using namespace PirateSimulator;

void GameLogic::createAllTask()
{
    PirateSimulator::TaskManager* taskManager = &PirateSimulator::TaskManager::GetInstance();

    taskManager->addTask<PirateSimulator::TimeTask>(TIMETASK);
    taskManager->addTask<PirateSimulator::InputTask>(INPUTTASK);
    taskManager->addTask<PirateSimulator::RenderTask>(RENDERTASK);
    taskManager->addTask<PirateSimulator::PhysicsTask>(PHYSICSTASK);
    taskManager->addTask<PirateSimulator::PlayerTask>(PLAYERTASK);
    taskManager->addTask<PirateSimulator::SoundTask>(SOUNDTASK);
    taskManager->addTask<PirateSimulator::SpawnTask>(SPAWNTASK);
}

void GameLogic::cleanAllTasks()
{
    PirateSimulator::TaskManager* taskManager = &PirateSimulator::TaskManager::GetInstance();
    taskManager->reset();
}
