#ifndef PIECEADMINISTRATOR_H_INCLUDED
#define PIECEADMINISTRATOR_H_INCLUDED

#include "GameConfig.h"
#include "Piece.h"


namespace PirateSimulator
{
    class PieceAdministrator
    {
    public:
        enum
        {
            PIECE_COUNT = 30,
        };

        static constexpr const float PIECE_HEIGHT = 2.f;


    private:
        Piece pieceArray[PIECE_COUNT];


    public:
        PieceAdministrator(){}
    };
}

#endif // !PIECEADMINISTRATOR_H_INCLUDED