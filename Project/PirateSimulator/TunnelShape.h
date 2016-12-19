#ifndef TUNNEL_SHAPE_H
#define TUNNEL_SHAPE_H
#include "ShapeComponent.h"

namespace PirateSimulator
{
    class TunnelShape : public ShapeComponent
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
            return false;
        }

        static std::string typeId() noexcept
        {
            return "ShapeComponent";
        }


        std::string getTypeId() const noexcept override
        {
            return TunnelShape::typeId();
        }

        void setGameObject(PirateSimulator::GameObject* parent) override;

    };
}

#endif //TUNNEL_SHAPE_H