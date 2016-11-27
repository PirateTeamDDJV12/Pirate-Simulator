#include "GameObjectManager.h"

#include <algorithm>

using namespace PirateSimulator;



GameObjectManager GameObjectManager::singleton;


void GameObjectManager::animAllGameObject(float elapsedTime)
{
    std::for_each(
        m_gameObjectArray.begin(),
        m_gameObjectArray.end(),
        [&](GameObjectRef GObject){
            auto maybeAPieceObj = std::dynamic_pointer_cast<Piece>(GObject);

            if (maybeAPieceObj == std::shared_ptr<Piece>())
            {
                GObject->anime(elapsedTime);
            }
            else
            {
                m_pieceAdministrator.update(maybeAPieceObj, elapsedTime);
            }
        }
    );
}

void GameObjectManager::init()
{
    m_pieceAdministrator.init();
}