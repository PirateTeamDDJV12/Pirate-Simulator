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
    soundManager.playMusic("PirateSimulator/SeaNoise.wav");
    soundManager.playMusic("PirateSimulator/PlayBackgroundMusic.mp3");  


    //Define the timed song
    constexpr const long long seagulScreamMinTimer = 4800;
    constexpr const long long seagulScreamMaxTimer = 13800;

    PirateSimulator::defineTimedSound(
        "PirateSimulator/SeagullNoise.mp3", 
        seagulScreamMinTimer, 
        seagulScreamMaxTimer
    ); //I want seagull to scream at random interval


    //Duration in milliseconds of the Background music.
    constexpr const long long backgroundTimer = 165000; //2min45

    //Fix the music problem (the background isn't an audio music that can loop back)
    PirateSimulator::defineTimedSound(
        "PirateSimulator/PlayBackgroundMusic.mp3",
        backgroundTimer,
        backgroundTimer
    );


    //reduce the sound of seagull because they're screaming too loudly
    soundManager.setVolumeMusic("PirateSimulator/SeagullNoise.mp3", 0.75f);


    //reduce the sound of the Sea Noise to let the magnificent background music be well listened
    constexpr const float seaNoiseSoundVolume = 0.75f;

    soundManager.setVolumeMusic("PirateSimulator/SeaNoise.wav", seaNoiseSoundVolume);
}

void GameLogic::startTitleScreenMusic() const
{
    SoundManager& soundManager = SoundManager::singleton;

    //start the title Screen music
    soundManager.playMusic("PirateSimulator/UISoundtrack.mp3"); //Loyd's Yohoho music ( ... -_-' )
}

void GameLogic::cleanAllTasks()
{
    PirateSimulator::TaskManager* taskManager = &PirateSimulator::TaskManager::GetInstance();
    taskManager->reset();
}
