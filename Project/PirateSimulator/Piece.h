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

        long long m_unspawnedTime;

        void (Piece::* m_pieceAnimation)(float elapsedTime);


    public:
        Piece(const Transform& spawnPosition, size_t pieceID);


    public:
        size_t getID() const noexcept { return m_pieceID; }
        const Transform& getSpawnTransform() const noexcept { return m_transform; }
        void setSpawnPosition(const Transform& spawnPosition) noexcept { m_transform = spawnPosition; }

        bool isEmpty() const noexcept { return m_pieceInstance == GameObjectRef(); }
        bool isInstanciated() const noexcept { return !isEmpty(); }

        long long getUnspawnedTime() const noexcept;

        void anim(float elapsedTime);


    public:
        GameObjectRef createPiece();
        void destroyPiece();


    private:
        void noAnimation(float elapsedTime) {}
        void doAnimation(float elapsedTime);
    };
}


#endif // !PIECE_H_INCLUDED