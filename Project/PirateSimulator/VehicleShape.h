#include "ShapeComponent.h"
#include "PhysicsManager.h"

#ifndef VEHICLE_SHAPE
#define  VEHICLE_SHAPE
namespace PirateSimulator
{
	class VehicleShape : public ShapeComponent
	{
	public:

        bool isBoat()
        {
            return true;
        };
        bool isPiece()
        {
            return false;
        };

		static std::string typeId() noexcept
		{
			return "ShapeComponent";
		}

		
		std::string getTypeId() const noexcept override
        {
			return VehicleShape::typeId();
		}
   
		void setGameObject(PirateSimulator::GameObject* parent) override;

	};
}

#endif
