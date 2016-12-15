#include "PhysicsTask.h"
#include "GameObjectManager.h"
#include "RendererManager.h"
#include "LightManager.h"
#include "PhysicsManager.h"

using namespace PirateSimulator;

void PhysicsTask::init()
{
    PhysicsManager::singleton.initialize();
    
}

void PhysicsTask::update()
{
    float elapsedTime = TimeManager::GetInstance().getElapsedTimeFrame();

    // On prépare la prochaine image
    GameObjectManager::singleton.animAllGameObject(elapsedTime);

    LightManager::singleton.update(elapsedTime);
	
    PhysicsManager::singleton.update(elapsedTime);
}

void PhysicsTask::cleanup()
{
    PhysicsManager::singleton.reset();
}
