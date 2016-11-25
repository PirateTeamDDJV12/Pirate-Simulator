#include "PieceAdministrator.h"

#include "..\PetitMoteur3D\PetitMoteur3D\Config\Config.hpp"
#include "TimeManager.h"

#include <algorithm>


using namespace PirateSimulator;


struct PieceSpawnPosition
{
public:
    Transform m_spawnPos[PieceAdministrator::PIECE_COUNT];


public:
    PieceSpawnPosition(unsigned int sceneWidth, unsigned int sceneHeight, float mapScale)
    {
        mapSpawn(sceneWidth, sceneHeight, mapScale);
    }


public:
    void mapSpawn(unsigned int sceneWidth, unsigned int sceneHeight, float mapScale)
    {
        float xPos = 0.f;
        float zPos = 0.f;

        float realWidth = (sceneWidth * mapScale);
        float realHeight = (sceneHeight * mapScale);

        float separatorX = realWidth / static_cast<float>(PieceAdministrator::PIECE_COUNT_IN_A_ROW);
        float separatorZ = realHeight / static_cast<float>(PieceAdministrator::PIECE_COUNT_IN_A_COLUMN);

        std::for_each(
            std::begin(m_spawnPos),
            std::end(m_spawnPos),
            [&](Transform& pos) {
                pos.m_position.vector4_f32[0] = xPos;
                pos.m_position.vector4_f32[1] = PieceAdministrator::PIECE_HEIGHT_FROM_SOIL;
                pos.m_position.vector4_f32[2] = zPos;

                xPos += separatorX;

                if (xPos > (realWidth))
                {
                    zPos += separatorZ;
                    xPos = 0.f;
                }
            }
        );
    }
};


PieceAdministrator::PieceAdministrator()
{
    m_pieceArray.reserve(PIECE_COUNT);
}

void PieceAdministrator::init()
{
    PieceSpawnPosition pieceSpawnPos(
        Config::getInstance()->getWidth(),
        Config::getInstance()->getHeight(),
        Config::getInstance()->getMapScale()
    );

    for (size_t id = 0; id < PIECE_COUNT; ++id)
    {
        m_pieceArray.emplace_back(pieceSpawnPos.m_spawnPos[id], id);
        m_pieceArray[id].createPiece();
    }
}

void PieceAdministrator::update(float elapsedTime)
{
    std::for_each(
        m_pieceArray.begin(),
        m_pieceArray.end(),
        [&elapsedTime](Piece& piece) {
            if (piece.isEmpty())
            {
                if (piece.getUnspawnedTime() > PieceAdministrator::PIECE_RESPAWN_TIME)
                {
                    piece.createPiece();
                }
            }
            else
            {
                piece.anim(elapsedTime);
            }
        }
    );
}