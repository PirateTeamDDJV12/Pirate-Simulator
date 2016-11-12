#ifndef GAMEOBJECTMANAGER_H_INCLUDED
#define GAMEOBJECTMANAGER_H_INCLUDED

#include "GameConfig.h"
#include "GameObject.h"

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


    private:
        GameObjectManager() {}


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

        void animAllGameObject(float elapsedTime);
    };
}

#endif // !GAMEOBJECTMANAGER_H_INCLUDED