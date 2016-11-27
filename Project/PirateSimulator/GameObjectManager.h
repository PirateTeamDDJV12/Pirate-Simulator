#ifndef GAMEOBJECTMANAGER_H_INCLUDED
#define GAMEOBJECTMANAGER_H_INCLUDED

#include "GameConfig.h"
#include "GameObject.h"
#include "PieceAdministrator.h"

#include <vector>
#include <string>


namespace PirateSimulator
{
    class GameObjectManager
    {
    public:
        static GameObjectManager singleton;


    private:
        std::vector<GameObjectRef> m_gameObjectArray;

        PieceAdministrator m_pieceAdministrator;

        GameObjectRef(GameObjectManager::* m_subscribeStrategy)(GameObject* newGameObject);


    private:
        GameObjectManager(){}


    private:
        GameObjectManager(const GameObjectManager&) = delete;
        GameObjectManager& operator=(const GameObjectManager&) = delete;


    public:
        GameObjectRef subscribeAGameObject(GameObject* newGameObject)
        {
            m_gameObjectArray.push_back(GameObjectRef(newGameObject));

            return m_gameObjectArray[m_gameObjectArray.size() - 1];
        }

        GameObjectRef getGameObjectByName(const std::string& name) const
        {
            for (auto iter = m_gameObjectArray.begin(); iter != m_gameObjectArray.end(); ++iter)
            {
                if ((*iter)->compareName(name))
                {
                    return *iter;
                }
            }
            return GameObjectRef();
        }

        void unspawnGameObject(const std::string& name)
        {
            for (auto iter = m_gameObjectArray.begin(); iter != m_gameObjectArray.end(); ++iter)
            {
                if ((*iter)->compareName(name))
                {
                    m_gameObjectArray.erase(iter);
                    return;
                }
            }
        }

        void animAllGameObject(float elapsedTime);

        void init();
    };
}

#endif // !GAMEOBJECTMANAGER_H_INCLUDED