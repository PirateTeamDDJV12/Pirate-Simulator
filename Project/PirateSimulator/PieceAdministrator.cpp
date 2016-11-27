#include "PieceAdministrator.h"

#include "GameConfig.h"
#include "..\PetitMoteur3D\PetitMoteur3D\Config\Config.hpp"
#include "TimeManager.h"

#include <algorithm>


using namespace PirateSimulator;


void PieceAdministrator::PieceSpawnPosition::mapSpawn(unsigned int sceneWidth, unsigned int sceneHeight, float mapScale)
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

PieceAdministrator::PieceAdministrator() {}

void PieceAdministrator::init()
{
    m_pieceSpawnPos.mapSpawn(
        Config::getInstance()->getWidth(),
        Config::getInstance()->getHeight(),
        Config::getInstance()->getMapScale()
    );

    for (size_t iter = 0; iter < PIECE_COUNT; ++iter)
    {
        PieceFabrik::createPiece(m_pieceSpawnPos.m_spawnPos[iter], iter);
    }
}

void PieceAdministrator::update(GameObjectRef pieceObj, float elapsedTime)
{
    if (pieceObj == GameObjectRef())
    {
        auto piece = std::static_pointer_cast<Piece>(pieceObj);

        if (piece->getUnspawnedTime() > PieceAdministrator::PIECE_RESPAWN_TIME)
        {
            PieceFabrik::createPiece(m_pieceSpawnPos.m_spawnPos[piece->getID()], piece->getID());
        }
    }
    else
    {
        pieceObj->anime(elapsedTime);
    }
}