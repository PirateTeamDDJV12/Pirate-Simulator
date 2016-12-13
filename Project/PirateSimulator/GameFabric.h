#ifndef GAMEFABRIC_H_INCLUDED
#define GAMEFABRIC_H_INCLUDED

namespace PirateSimulator
{
    class Transform;

    class GameFabric
    {
    public:
        static void createSkybox();
        static void createHUD();

        static void createWater(const Transform& fieldTransform);
        static void createBoat(const Transform& boatTransform);
        static void createField(const Transform& fieldTransform);

        static void createCamera(const Transform& cameraTransform);
    };
}

#endif // GAMEFABRIC_H_INCLUDED