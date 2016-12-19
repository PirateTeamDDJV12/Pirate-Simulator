#ifndef PIECE_H_INCLUDED
#define PIECE_H_INCLUDED

#include "GameObject.h"


namespace PirateSimulator
{
    //moderator class for a coin. Own the instance of a coin and is responsible for its creation and destuction.
    class Piece
    {
    private:
        Transform m_transform;
        size_t m_pieceID;

        GameObjectRef m_pieceInstance;

        long long m_unspawnedTime;


    public:
        Piece(const Transform& spawnPosition, size_t pieceID);


    public:
        size_t getID() const noexcept { return m_pieceID; }
        const Transform& getSpawnTransform() const noexcept { return m_transform; }
        void setSpawnPosition(const Transform& spawnPosition) noexcept { m_transform = spawnPosition; }

        //the piece isn't alive
        bool isEmpty() const noexcept { return m_pieceInstance == GameObjectRef(); }

        //the coin is alive
        bool isInstanciated() const noexcept { return !isEmpty(); }

        long long getUnspawnedTime() const noexcept;

        void anim(float elapsedTime);


    public:
        //fabric => create the piece with the good values
        GameObjectRef createPiece();

        //destroy the piece
        void destroyPiece();
    };
}


#endif // !PIECE_H_INCLUDED