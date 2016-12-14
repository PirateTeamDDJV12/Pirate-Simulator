#include "GameLogic.h"

#include "TaskManager.h"
#include "SoundManager.h"

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

void GameLogic::loadMusics()
{
    SoundManager& soundManager = SoundManager::singleton;

    // for the menu. First Music
    soundManager.playMusic("PirateSimulator/UISoundtrack.mp3");
    

    soundManager.loadBackgroundMusics(
        "PirateSimulator/SeaNoise.wav",
        "PirateSimulator/WaterTunnel.wav",
        "PirateSimulator/PlayBackgroundMusic.mp3"
    );

    soundManager.loadNoises(
        "PirateSimulator/Collision1Noise.wav",
        "PirateSimulator/SeagullNoise.mp3",
        "PirateSimulator/JackpotPieceNoise.mp3"
    );
}