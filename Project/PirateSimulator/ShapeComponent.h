
#ifndef SHAPE_COMPONENT_H
#define SHAPE_COMPONENT_H

#include "Component.h"
#include "PhysicsManager.h"
//#include "SimulationComponent.h"


namespace physx
{
	class PxMaterial;
	class PxShape;
}


namespace PirateSimulator
{

	class GameObject;
	class ShapeComponent : public Component
	{
	private:
		physx::PxMaterial* m_material;
		physx::PxShape* m_shape;
		physx::PxRigidDynamic* m_actor;

	public:
		ShapeComponent() {};

		static std::string typeId() noexcept { return "ShapeComponent"; }

		std::string getTypeId() const noexcept override
		{
			return ShapeComponent::typeId();
		}

		 void setGameObject(PirateSimulator::GameObject* parent) override;
	public:
		static physx::unique_ptr<physx::PxRigidDynamic> createPxActor();
		
		physx::PxRigidDynamic& pxActor();
		
		virtual physx::PxTransform pose();
		
		void setPose(const physx::PxTransform &iPose);
		

	};



}
#endif // 
