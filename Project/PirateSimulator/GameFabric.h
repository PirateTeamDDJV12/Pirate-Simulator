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
        void createBoat(const Transform& boatTransform);
        void createField(const Transform& fieldTransform);

        void createCamera(const Transform& cameraTransform);
    };
}

#endif // GAMEFABRIC_H_INCLUDED