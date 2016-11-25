#include "ShapeComponent.h"

#include "PhysicsManager.h"
#include "GameObject.h"

using namespace PirateSimulator;
using namespace physx;

void ShapeComponent::setGameObject(PirateSimulator::GameObject* parent)
{
	m_gameObject = parent;
	m_material = PhysicsManager::singleton.physics().createMaterial(0.5f, 0.5f, 0.1f);
	//m_material = PhysicsManager::singleton.physics().createMaterial(0.2f,0.3f,0.5f);
	//m_shape = PhysicsManager::singleton.physics().createShape(physx::PxSphereGeometry(0.5f),m_material);

}
