#include "ShapeComponent.h"
#include "PhysicsManager.h"

#ifndef VEHICLE_SHAPE
#define  VEHICLE_SHAPE
namespace PirateSimulator
{
	class VehicleShape : public ShapeComponent
	{
	public:

        void onContact(const physx::PxContactPair &aContactPair) override
        {
            auto actor0 = static_cast<ShapeComponent*>(aContactPair.shapes[0]->getActor()->userData);
            auto actor1 = static_cast<ShapeComponent*>(aContactPair.shapes[1]->getActor()->userData);
            //Todo Set Behaviour
        }

        void onTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape) override
        {
            auto actor0 = static_cast<ShapeComponent*>(contactShape->getActor()->userData);
            auto actor1 = static_cast<ShapeComponent*>(actorShape->getActor()->userData);
            //TODO: Set Behaviour
        }

		static std::string typeId() noexcept
		{
			return "VehiculeShape";
		}

		
		std::string getTypeId() const noexcept override
        {
			return VehicleShape::typeId();
		}

		void setGameObject(PirateSimulator::GameObject* parent) override;



	};
}

#endif
