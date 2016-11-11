#ifndef GAMEOBJECTMANAGER_H_INCLUDED
#define GAMEOBJECTMANAGER_H_INCLUDED

#include "GameConfig.h"
#include "GameObject.h"

#include <map>
#include <string>


namespace PirateSimulator
{
    class GameObjectManager
    {
    public:
        static GameObjectManager singleton;


    private:
        std::map<std::string, GameObjectRef> m_gameObjectArray;


    private:
        GameObjectManager() {}


    private:
        GameObjectManager(const GameObjectManager&) = delete;
        GameObjectManager& operator=(const GameObjectManager&) = delete;


    public:
        GameObjectRef trySubscribeAGameObject(GameObject* newGameObject, const std::string& name)
        {
            auto gameObjectIter = m_gameObjectArray.find(name);

            if (gameObjectIter != m_gameObjectArray.end())
            {
                GameObjectRef intermediary(newGameObject);

                m_gameObjectArray.emplace(std::pair<std::string, GameObjectRef>(name, intermediary));

                return intermediary;
            }

            delete newGameObject;

            return GameObjectRef();
        }

        GameObjectRef subscribeAGameObject(GameObject* newGameObject, const std::string& name)
        {
            GameObjectRef intermediary(newGameObject);

            m_gameObjectArray.emplace(std::pair<std::string, GameObjectRef>(name, intermediary));

            return intermediary;
        }

        GameObjectRef tryGettingGameObjectByName(const std::string& name) const
        {
            auto gameObjectIter = m_gameObjectArray.find(name);

            if (gameObjectIter != m_gameObjectArray.end())
            {
                return gameObjectIter->second;
            }

            return GameObjectRef();
        }

        GameObjectRef getGameObjectByName(const std::string& name) const
        {
            return m_gameObjectArray.at(name);
        }
    };
}

#endif // !GAMEOBJECTMANAGER_H_INCLUDED