#include "GameObjectManager.h"

#include <algorithm>

using namespace PirateSimulator;



GameObjectManager GameObjectManager::singleton;


void GameObjectManager::animAllGameObject(float elapsedTime)
{
    std::for_each(
        m_gameObjectArray.begin(),
        m_gameObjectArray.end(),
        [&elapsedTime](GameObjectRef GObject){
            GObject->anime(elapsedTime);
        }
    );
}