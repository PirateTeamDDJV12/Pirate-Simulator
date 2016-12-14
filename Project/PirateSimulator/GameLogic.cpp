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

void GameLogic::createAllTask() const
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

void GameLogic::loadMusics() const
{
    SoundManager& soundManager = SoundManager::singleton;

    soundManager.loadBackgroundMusics(
        "PirateSimulator/SeaNoise.wav",             //Sea noise (sea wave)
        "PirateSimulator/WaterTunnel.wav",          //Water that splashes continuously on an underground cavern.
        "PirateSimulator/PlayBackgroundMusic.mp3"   //Assassin's creed black flag combat music
    );

    soundManager.loadNoises(
        "PirateSimulator/Collision1Noise.wav",      //Boing collision noise
        "PirateSimulator/SeagullNoise.mp3",         //Seagull that passes occasionnaly on the sea
        "PirateSimulator/JackpotPieceNoise.mp3"     //Jackpot machine that sends you a nice Tint
    );
}

void GameLogic::startGameMusic() const
{
    SoundManager& soundManager = SoundManager::singleton;

    //stop the title Screen music
    soundManager.stopMusic("PirateSimulator/UISoundtrack.mp3");


    //Play the background music of the main game
    soundManager.playMusic("PirateSimulator/PlayBackgroundMusic.mp3");  
    soundManager.playMusic("PirateSimulator/SeaNoise.wav");


    //Define the timed song
    constexpr const long long seagulScreamMinTimer = 2800;
    constexpr const long long seagulScreamMaxTimer = 9800;

    PirateSimulator::defineTimedSound(
        "PirateSimulator/SeagullNoise.mp3", 
        seagulScreamMinTimer, 
        seagulScreamMaxTimer
    ); //I want seagull to scream at random interval


    //reduce the sound of the Sea Noise to let the magnificent background music be well listened
    constexpr const float seaNoiseSoundVolume = 0.65f;

    soundManager.setVolumeMusic("PirateSimulator/SeaNoise.wav", seaNoiseSoundVolume);
}

void GameLogic::startTitleScreenMusic() const
{
    SoundManager& soundManager = SoundManager::singleton;

    //start the title Screen music
    soundManager.playMusic("PirateSimulator/UISoundtrack.mp3"); //Loyd's Yohoho music ( ... -_-' )
}