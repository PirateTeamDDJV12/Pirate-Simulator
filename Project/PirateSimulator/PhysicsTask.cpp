#include "PhysicsTask.h"
#include "GameObjectManager.h"
#include "RendererManager.h"

using namespace PirateSimulator;

void PhysicsTask::update()
{
    // Affichage optimis� 
    RendererManager::singleton.getDispositif()->Present();

    // On pr�pare la prochaine image
    GameObjectManager::singleton.animAllGameObject(TimeManager::GetInstance().getElapsedTimeFrame());

    RendererManager::singleton.update();
}
