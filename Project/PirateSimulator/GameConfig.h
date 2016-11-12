#ifndef GAMECONFIG_H_INCLUDED
#define GAMECONFIG_H_INCLUDED


namespace PirateSimulator
{
    namespace GameGlobals
    {
        struct SceneGlobals
        {
            enum
            {
                MAX_X = 400,
                MAX_Z = 400,

                MIN_X = -100,
                MIN_Z = -100
            };
        };


        struct CameraGlobals
        {
            static constexpr float NEAREST_PLANE = 1.f;
            static constexpr float FARTHEST_PLANE = 3000.f;

            static constexpr float LINEAR_SPEED = 0.33f;
            static constexpr float ANGULAR_SPEED = 0.02f;
        };
    }



#define DEBUG_PIRATE_SIMULATOR

#ifdef DEBUG_PIRATE_SIMULATOR
    extern size_t debugCount;
#endif
}


#endif //!GAMECONFIG_H_INCLUDED