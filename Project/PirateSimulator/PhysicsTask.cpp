#include "PhysicsTask.h"
#include "GameObjectManager.h"
#include "RendererManager.h"
#include "LightManager.h"

using namespace PirateSimulator;

void PhysicsTask::init()
{
    GameObjectManager::singleton.init();
}

void PhysicsTask::update()
{
    // Affichage optimisé 
    RendererManager::singleton.getDispositif()->Present();

    float elapsedTime = TimeManager::GetInstance().getElapsedTimeFrame();

    // On prépare la prochaine image
    GameObjectManager::singleton.animAllGameObject(elapsedTime);

    LightManager::singleton.update(elapsedTime);

    RendererManager::singleton.update();
}
