#pragma once


#include <set>
#include <memory>
#include "..\..\..\PirateSimulator\GameObject.h"
#include "../../PetitMoteur3D/stdafx.h"
#include "IManager.h"
namespace  PirateSimulator
{

	using GameObjectRef = std::shared_ptr<GameObject>;
class IGameObject;

class ISpawnManager: virtual public IManager::IManager
{
public:
	static const std::string& TypeId()
	{
		return "ISpawnManager";
	}

public:
	virtual void update() = 0;
	virtual void unspawn(const GameObjectRef &aActorToUnspawn) = 0;
	virtual void unspawnAll() = 0;
	virtual GameObjectRef spawn(const std::string& name, const physx::PxTransform &aPose) = 0;
	virtual const std::set<GameObjectRef>& gameObjects() = 0;

	template<class T>
	std::shared_ptr<T> spawn(const physx::PxTransform &aInitialPose)
	{	return std::dynamic_pointer_cast<T>(spawn(T::TypeId(), aInitialPose)); }
};

using GameSpawnerRef = std::shared_ptr<ISpawnManager>;
} // namespace engine
