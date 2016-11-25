
#ifndef SHAPE_COMPONENT_H
#define SHAPE_COMPONENT_H

#include "Component.h"
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

		virtual void setGameObject(PirateSimulator::GameObject* parent) override;
	};



}
#endif // 
