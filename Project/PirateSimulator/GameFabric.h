#ifndef GAMEFABRIC_H_INCLUDED
#define GAMEFABRIC_H_INCLUDED

namespace PirateSimulator
{
    class Transform;

    class GameFabric
    {
    public:
        static void createSkybox();
        static void createWater(const Transform& fieldTransform);
        static void createBoat(const Transform& fieldTransform);

    };
}

#endif // GAMEFABRIC_H_INCLUDED