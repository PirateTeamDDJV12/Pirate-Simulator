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
        struct PieceSpawnPosition
        {
        public:
            Transform m_spawnPos[PieceAdministrator::PIECE_COUNT];


        public:
            PieceSpawnPosition() {}


        public:
            void mapSpawn(unsigned int sceneWidth, unsigned int sceneHeight, float mapScale);
        };



    private:
        PieceSpawnPosition m_pieceSpawnPos;


    public:
        PieceAdministrator();


    public:
        void init();
        void update(GameObjectRef pieceObj, float elapsedTime);
    };
}

#endif // !PIECEADMINISTRATOR_H_INCLUDED