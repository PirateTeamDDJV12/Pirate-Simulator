#include "PhysicsTask.h"
#include "GameObjectManager.h"
#include "RendererManager.h"

using namespace PirateSimulator;

void PhysicsTask::update()
{
    // Affichage optimisé 
    RendererManager::singleton.getDispositif()->Present();

    // On prépare la prochaine image
    GameObjectManager::singleton.animAllGameObject(TimeManager::GetInstance().getElapsedTimeFrame());

    RendererManager::singleton.update();
}
