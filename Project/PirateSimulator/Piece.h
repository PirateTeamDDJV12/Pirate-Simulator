#ifndef PIECE_H_INCLUDED
#define PIECE_H_INCLUDED

#include "GameObject.h"


namespace PirateSimulator
{
    struct Piece
    {
    private:
        Transform m_transform;
        size_t m_pieceID;

        GameObjectRef m_pieceInstance;


    public:
        Piece(const Transform& spawnPosition, size_t pieceID) :
            m_transform{ spawnPosition },
            m_pieceID{ pieceID }
        {}


    public:
        size_t getID() const noexcept { return m_pieceID; }
        const Transform& getSpawnTransform() const noexcept { return m_transform; }
        void setSpawnPosition(const Transform& spawnPosition) noexcept { m_transform = spawnPosition; }

        bool isEmpty() const noexcept { return m_pieceInstance == GameObjectRef(); }
        bool isInstanciated() const noexcept { return !isEmpty(); }


    public:
        GameObjectRef createPiece();
        void destroyPiece();
    };
}


#endif // !PIECE_H_INCLUDED