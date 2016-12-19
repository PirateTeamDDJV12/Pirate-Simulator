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
        enum SubsribingStrategy
        {
            NONE,
            PIECE
        };

    public:
        static GameObjectManager singleton;


    private:
        std::vector<GameObjectRef> m_gameObjectArray;
        std::vector<Piece*> m_coinsToDestroy;

        PieceAdministrator m_pieceAdministrator;

        GameObjectRef(GameObjectManager::* m_subscribeStrategy)(GameObject* newGameObject);


    private:
        GameObjectManager() :
            m_subscribeStrategy{ &GameObjectManager::minimalSubscribingGameObject }
        {}


    private:
        GameObjectManager(const GameObjectManager&) = delete;
        GameObjectManager& operator=(const GameObjectManager&) = delete;


    public:
        //set the subscribing strategy (wether it is a piece GO that we register or not)
        void setSubscribingStrategy(SubsribingStrategy strategy) noexcept;

        GameObjectRef subscribeAGameObject(GameObject* newGameObject)
        {
            return (this->*m_subscribeStrategy)(newGameObject);
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

        void cleanAllGameObjects();

        //add a coin to the destroy list
        void destroyCoin(Piece *coin)
        {
            m_coinsToDestroy.emplace_back(coin);
        }

        //at the end. Destroy everything
        void unspawnCoinsList()
        {
            for(auto coin : m_coinsToDestroy)
            {
                coin->destroyPiece();
            }
            m_coinsToDestroy.clear();
        }

        void animAllGameObject(float elapsedTime);

        void init();

        PieceAdministrator* getPieceAdministrator()
        {
            return &m_pieceAdministrator;
        }


    private:
        GameObjectRef minimalSubscribingGameObject(GameObject* newGameObject)
        {
            m_gameObjectArray.push_back(GameObjectRef(newGameObject));

            return m_gameObjectArray[m_gameObjectArray.size() - 1];
        }

        GameObjectRef subscribingAPiece(GameObject* newGameObject)
        {
            return GameObjectRef(newGameObject);
        }
    };
}

#endif // !GAMEOBJECTMANAGER_H_INCLUDED