#ifndef PIECESHAPE_H
#define  PIECESHAPE_H

#include "ShapeComponent.h"
#include "PhysicsManager.h"

namespace PirateSimulator
{
    class PieceShape : public ShapeComponent
    {
    public:
        
        void onContact(const physx::PxContactPair &aContactPair) override;

        void onTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape) override
        {
            //auto actor0 = static_cast<ShapeComponent*>(contactShape->getActor()->userData);
            //auto actor1 = static_cast<ShapeComponent*>(actorShape->getActor()->userData);
            //TODO: Set Behaviour
        }

        static std::string typeId() noexcept
        {
            return "PieceShape";
        }
        /*Piece* getPiece()
        {
            return m_piece;
        }*/

        std::string getTypeId() const noexcept override
        {
            return PieceShape::typeId();
        }

        void setGameObject(PirateSimulator::GameObject* parent);



    };
}

#endif
