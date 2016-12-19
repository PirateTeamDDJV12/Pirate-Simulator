#include "PieceAdministrator.h"

#include "GameConfig.h"
#include "..\PetitMoteur3D\PetitMoteur3D\Config\Config.hpp"
#include "TimeManager.h"
#include "FilesManager.h"

#include <algorithm>
#include <string>


using namespace PirateSimulator;


struct PieceSpawnPosition
{
public:
    std::vector<Transform> m_spawnPos;


public:
    PieceSpawnPosition(unsigned int sceneWidth, unsigned int sceneHeight, float mapScale)
    {
        m_spawnPos.reserve(PieceAdministrator::PIECE_COUNT);
        mapSpawn(sceneWidth, sceneHeight, mapScale);
    }

    PieceSpawnPosition(const std::string& fileName)
    {
        mapFromFile(fileName);
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
                pos.setPosition(xPos, PieceAdministrator::PIECE_HEIGHT_FROM_SOIL, zPos);

                xPos += separatorX;

                if (xPos > (realWidth))
                {
                    zPos += separatorZ;
                    xPos = 0.f;
                }
            }
        );
    }

    void mapFromFile(const std::string& fileName)
    {
        std::vector<float> position = FilesManager().readPiecePositionFile(fileName);

        m_spawnPos.resize(position.size() / 2);

        size_t count = 0;

        std::for_each(
            std::begin(m_spawnPos),
            std::end(m_spawnPos),
            [&](Transform& pos) {
                pos.setPosition(position[count], PieceAdministrator::PIECE_HEIGHT_FROM_SOIL, position[count + 1]);
                count += 2;
            }
        );
    }
};

PieceAdministrator::PieceAdministrator(): m_currentScore{0}
{
    m_pieceArray.reserve(PIECE_COUNT);
}


void PieceAdministrator::init()
{
    PieceSpawnPosition pieceSpawnPos("PirateSimulator/PiecePosition.bin");

    for (size_t id = 0; id < pieceSpawnPos.m_spawnPos.size(); ++id)
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

void PieceAdministrator::cleanUp()
{
    for(int iter = 0; iter < m_pieceArray.size(); ++iter)
    {
        m_pieceArray[iter].destroyPiece();
    }
}

unsigned int PieceAdministrator::getScore() const noexcept
{
    return m_currentScore;
}

void PieceAdministrator::addScore() noexcept
{
    ++m_currentScore;
    TimeManager::GetInstance().increaseTime(5s);
}

void PieceAdministrator::resetScore() noexcept
{
    m_currentScore = 0;
}