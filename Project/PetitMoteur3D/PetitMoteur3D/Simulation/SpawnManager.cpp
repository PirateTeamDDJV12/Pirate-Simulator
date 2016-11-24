//=============================================================================
// EXTERNAL DECLARATIONS
//=============================================================================
#include "Precompiled.h"
#include "SpawnManager.h"
#include "Core/Logging/Logger.h"
#include "Core/GameObjects/IGameObject.h"
#include "Core/GameObjects/GameObjectFactory.h"

#include <set>

using namespace physx;

namespace PirateSimulator {
	
	//=============================================================================
	// CLASS SpawnManager::SpawnManagerImp
	//=============================================================================
	class SpawnManager::SpawnManagerImp
	{
	public:
		std::set<GameObjectRef> _unspawnList;
		std::set<GameObjectRef> _existingGameObjects;

	};

	//=============================================================================
	// CLASS SpawnManager
	//=============================================================================
	SpawnManager::SpawnManager()
		: _imp(new SpawnManagerImp)
	{}

	//-----------------------------------------------------------------------------
	//
	SpawnManager::~SpawnManager()
	{
		delete _imp;
	}

	//-----------------------------------------------------------------------------
	//
	void SpawnManager::update()
	{
		// copy, in case something is unspawned while we unspawn...
		std::set<GameObjectRef> unspawnList = _imp->_unspawnList;
		_imp->_unspawnList.clear();

		for (auto iter = unspawnList.begin(); iter != unspawnList.end(); ++iter)
		{
			(*iter)->onUnspawn();
			(*iter)->removeAllComponents();
		}
	}

	//-----------------------------------------------------------------------------
	//
	void SpawnManager::unspawn(const GameObjectRef &aGameObjectToUnspawn)
	{
		_imp->_unspawnList.insert(aGameObjectToUnspawn);
		auto foundIter = _imp->_existingGameObjects.find(aGameObjectToUnspawn);
		if (foundIter != _imp->_existingGameObjects.end())
		{
			LOG_INFO("[go:0x%p] UNSPAWNED", foundIter->get());
			_imp->_existingGameObjects.erase(foundIter);
		}
	}

	//-----------------------------------------------------------------------------
	//
	void SpawnManager::unspawnAll()
	{
		_imp->_unspawnList.insert(_imp->_existingGameObjects.begin(), _imp->_existingGameObjects.end());
		_imp->_existingGameObjects.clear();
	}

	//-----------------------------------------------------------------------------
	//
	GameObjectRef SpawnManager::spawn(const IGameObject::IdType &aGameObjectID, const PxTransform &aPose)
	{
		GameObjectRef wGameObject(GameObjectFactory::createInstance(aGameObjectID));
		wGameObject->onSpawn(aPose);
		_imp->_existingGameObjects.insert(wGameObject);
		LOG_INFO("[go:0x%p] SPAWNED %s", wGameObject.get(), aGameObjectID);
		return wGameObject;
	}

	//-----------------------------------------------------------------------------
	//
	const std::set<GameObjectRef>& SpawnManager::gameObjects()
	{
		return _imp->_existingGameObjects;
	}

	//-----------------------------------------------------------------------------
	//
	IComponent::IdType SpawnManager::TypeId()
	{
		return "SpawnManager";
	}
} // namespace engine

#include "Engine/Managers/GameManagerFactory.h"

namespace engine {

	RegisterGameManagerType<SpawnManager> gSpawnManagerRegistration;
} // namespace engine
