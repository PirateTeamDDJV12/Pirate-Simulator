

#ifndef PIECESHAPE_H
#define  PIECESHAPE_H

#include "ShapeComponent.h"
#include "PhysicsManager.h"

namespace PirateSimulator
{
    class Piece;
    class PieceShape : public ShapeComponent
    {
    public:
        Piece* m_piece;
        void onContact(const physx::PxContactPair &aContactPair) override
        {
            ShapeComponent* actor0 = static_cast<ShapeComponent*>(aContactPair.shapes[0]->getActor()->userData);
            ShapeComponent* actor1 = static_cast<ShapeComponent*>(aContactPair.shapes[1]->getActor()->userData);
            if (actor1->getTypeId()=="PieceShape")
            {
                //unspawn the piece
                static_cast<PieceShape*>(actor1)->getPiece()->destroyPiece();
            }
            else //La piece correspond à l'autre acteur
            {
               static_cast<PieceShape*>(actor0)->getPiece()->destroyPiece();
            }
        }

        void onTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape) override
        {
            auto actor0 = static_cast<ShapeComponent*>(contactShape->getActor()->userData);
            auto actor1 = static_cast<ShapeComponent*>(actorShape->getActor()->userData);
            //TODO: Set Behaviour
        }

        static std::string typeId() noexcept
        {
            return "PieceShape";
        }
        Piece* getPiece()
        {
            return m_piece;
        }

        std::string getTypeId() const noexcept override
        {
            return PieceShape::typeId();
        }

        void setGameObject(PirateSimulator::GameObject* parent, Piece* piece) ;



    };
}

#endif
