#include "ShapeComponent.h"
#include "PhysicsManager.h"

#ifndef VEHICLE_SHAPE
#define  VEHICLE_SHAPE
namespace PirateSimulator
{
	class VehicleShape : public ShapeComponent
	{
	public:

       // void onContact(const physx::PxContactPair &aContactPair) override;

        

		static std::string typeId() noexcept
		{
			return "VehicleShape";
		}

		
		std::string getTypeId() const noexcept override
        {
			return VehicleShape::typeId();
		}

		void setGameObject(PirateSimulator::GameObject* parent) override;



	};
}

#endif
