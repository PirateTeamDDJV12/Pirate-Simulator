#ifndef PIECEADMINISTRATOR_H_INCLUDED
#define PIECEADMINISTRATOR_H_INCLUDED

#include "Piece.h"

#include <vector>


namespace PirateSimulator
{
    class PieceAdministrator
    {
    public:
        enum
        {
            PIECE_COUNT_IN_A_COLUMN = 10,
            PIECE_COUNT_IN_A_ROW = 10,

            PIECE_COUNT = PIECE_COUNT_IN_A_COLUMN * PIECE_COUNT_IN_A_ROW,

            PIECE_RESPAWN_TIME = 3 * 1000
        };

        static constexpr const float PIECE_HEIGHT_FROM_SOIL = 5.f;


    private:
        std::vector<Piece> m_pieceArray;
        unsigned int m_currentScore;


    public:
        PieceAdministrator();


    public:
        unsigned int getScore() const
        {
            return m_currentScore;
        }
        void addScore()
        {
            ++m_currentScore;
        }
        void resetScore()
        {
            m_currentScore = 0;
        }
        void init();
        void update(float elapsedTime);
    };
}

#endif // !PIECEADMINISTRATOR_H_INCLUDED