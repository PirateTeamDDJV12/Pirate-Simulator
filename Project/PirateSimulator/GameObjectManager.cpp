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

    m_pieceAdministrator.update(elapsedTime);
}

void GameObjectManager::setSubscribingStrategy(SubsribingStrategy strategy) noexcept
{
    switch (strategy)
    {
    case GameObjectManager::PIECE:
        m_subscribeStrategy = &GameObjectManager::subscribingAPiece;
        break;

    case GameObjectManager::NONE:
    default:
        m_subscribeStrategy = &GameObjectManager::minimalSubscribingGameObject;
    }
}

void GameObjectManager::init()
{
    m_pieceAdministrator.init();
}