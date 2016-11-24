#include "Component.h"
#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
#include "SimulationComponent.h"
#ifndef SHAPE_COMPONENT_H
#define SHAPE_COMPONENT_H
namespace PirateSimulator
{
	class GameObject;
	class IShapeComponent : public Component
	{
	protected:
		physx::PxActorShape m_actShape;
	public:
		static std::string typeId() noexcept { return "ShapeComponent"; }
		std::string getTypeId() const noexcept override
		{
			return IShapeComponent::typeId();
		}
		virtual void setGameObject(GameObject* parent)
		{
			m_gameObject = parent;
			std::shared_ptr<DynamicSimulationComponent> simulationComponent = createComponent<DynamicSimulationComponent>();
			PxRigidDynamic &pxActor = simulationComponent->pxActor();
			//TODO Récupérer Data pour créer  HB
			//auto meshData = (m_gameObject->getComponent<IMesh>());
			//meshData->getWorldMatrix()
		}
	};

	class ShapeComponent : public IShapeComponent
	{
	protected:
		
	public:
		~ShapeComponent() = default;

	};


}
#endif // 
