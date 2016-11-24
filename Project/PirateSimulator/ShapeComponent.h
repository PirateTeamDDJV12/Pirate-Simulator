
#ifndef SHAPE_COMPONENT_H
#define SHAPE_COMPONENT_H

#include "Component.h"
#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
//#include "SimulationComponent.h"

#include "PhysicsManager.h"
namespace PirateSimulator
{
	class GameObject;
	class ShapeComponent : public Component
	{
	public:

	protected:
		//physx::PxActorShape m_actShape;
		//std::unique_ptr<physx::PxMaterial> m_material;
		physx::PxMaterial* m_material;
		physx::PxShape* m_shape;
	public:
		ShapeComponent() {};

		static std::string typeId() noexcept { return "ShapeComponent"; }
		std::string getTypeId() const noexcept override
		{
			return ShapeComponent::typeId();
		}
		void setGameObject(GameObject* parent)
		{
			PhysicsManager  PhysicsManager::singleton;
			m_gameObject = parent;
			m_material = PhysicsManager::singleton.physics().createMaterial(0.5f, 0.5f, 0.1f);
			//m_material = PhysicsManager::singleton.physics().createMaterial(0.2f,0.3f,0.5f);
			//m_shape = PhysicsManager::singleton.physics().createShape(physx::PxSphereGeometry(0.5f),m_material);
		
		}
	};



}
#endif // 
