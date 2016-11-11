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
            static const float NEAREST_PLANE;
            static const float FARTHEST_PLANE;

            static const float LINEAR_SPEED;
            static const float ANGULAR_SPEED;
        };
    }
}


#endif //!GAMECONFIG_H_INCLUDED