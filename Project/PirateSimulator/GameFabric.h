#ifndef GAMEFABRIC_H_INCLUDED
#define GAMEFABRIC_H_INCLUDED

namespace PirateSimulator
{
    class Transform;

    class GameFabric
    {
    public:
        GameFabric() = default;

    public:
        void createSkybox();
        void createHUD();

        void createWater(const Transform& fieldTransform);
        void createField(const Transform& fieldTransform);
        void createTunnel(const Transform& tunnelTransform);

        void createCameraAndBoat(const Transform& cameraTransform, const Transform& boatTransform);

        void createPieces();
    };
}

#endif // GAMEFABRIC_H_INCLUDED