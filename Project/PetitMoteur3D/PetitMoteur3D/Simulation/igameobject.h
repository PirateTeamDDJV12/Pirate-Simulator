#pragma once
//=============================================================================
// EXTERNAL DECLARATIONS
//=============================================================================
//#include "PhysicsAPI.forward.h"
#include <memory>

namespace PirateSimulator
{

//=============================================================================
// TYPE DEFINITIONS
//=============================================================================
using GameObjectRef = std::shared_ptr<IGameObject>;
using GameObjectWeakRef = std::weak_ptr<IGameObject>;


//=============================================================================
// INTERFACE IGameObject
//=============================================================================
// An IGameObject is a game entity (player, obstacle, landscape, trigger, etc.)
// It is created with no particular property and is specialized at runtime by
// decorating with different components.  The player GameObject will be 
// decorated with a PlayerComponent for instance, GameObjects with drawn 
// primitives will have a RenderComponent, etc...
//
// GameObjects are referenced using a shared_ptr so that other objects may
// keep reference to them usually using a weak_ptr.  An object may thus
// lock on another object using this weak_ptr and detect that the object
// has vanished by checking if the ptr is still valid.
//
// GameObjects should generally be spawned and unspawned at specific moments
// (ex. end of frame) so that the behaviour is not depedant on the order
// in which objects are being processed.  For this purpose, use a SpawnManager
// to defer unspawning and spawning.

class IGameObject//: virtual public IComponentObject<IComponent, IComponentInterface>
{
public:
	using IdType = const char*;

public:
	virtual ~IGameObject() = default;

	// called when the game object is spawned in the game
	virtual void onSpawn(const physx::PxTransform &aPose) = 0;

	// called when the game object is removed from the game
	virtual void onUnspawn() = 0;

	// returns the type of this GameObject
	virtual IdType typeId() const = 0;

	// returns a reference to this GameObject
	virtual GameObjectRef ref() = 0;
	
	// returns a printable name that identifies this GameObject
	virtual const char *debugName() const = 0;
};

} // namespace engine
