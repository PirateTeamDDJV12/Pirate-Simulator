#ifndef PIECESHAPE_H
#define  PIECESHAPE_H

#include "ShapeComponent.h"
#include "PhysicsManager.h"

namespace PirateSimulator
{
    class PieceShape : public ShapeComponent
    {
    public:
        
        bool isBoat()
        {
            return false;
        };

        bool isPiece()
        {
            return true;
        };
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
            return PieceShape::typeId();
        }

        void setGameObject(PirateSimulator::GameObject* parent);

        void setPiece(Piece * piece)
        {
            m_piece = piece;
        }

    };
}

#endif // ifndef PIECE_SHAPE_H