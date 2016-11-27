#ifndef PIECE_H_INCLUDED
#define PIECE_H_INCLUDED

#include "GameObject.h"


namespace PirateSimulator
{
    class Piece : public GameObject
    {
    private:
        size_t m_pieceID;

        long long m_unspawnedTime;


    public:
        Piece(const Transform& spawnPosition, size_t pieceID);


    public:
        size_t getID() const noexcept { return m_pieceID; }
        const Transform& getSpawnTransform() const noexcept { return m_transform; }
        void setSpawnPosition(const Transform& spawnPosition) noexcept { m_transform = spawnPosition; }

        long long getUnspawnedTime() const noexcept;
    };

    struct PieceFabrik
    {
    public:
        static void createPiece(const Transform& pos, size_t pieceID);
    };
}


#endif // !PIECE_H_INCLUDED