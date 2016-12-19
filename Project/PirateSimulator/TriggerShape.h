#ifndef TRIGGER_SHAPE_H
#define TRIGGER_SHAPE_H
#include "ShapeComponent.h"
namespace PirateSimulator
{
    class TriggerShape : public ShapeComponent
    {
    public:
        bool isBoat()
        {
            return false;
        }
        bool isPiece()
        {
            return false;
        }
        bool isTrigger()
        {
            return true;
        }

        static std::string typeId() noexcept
        {
            return "ShapeComponent";
        }


        std::string getTypeId() const noexcept override
        {
            return TriggerShape::typeId();
        }

        void setGameObject(PirateSimulator::GameObject* parent) override;
        
    };
}

#endif// ifndef TRIGGER_SHAPE_H