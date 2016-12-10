#ifndef GAMECONFIG_H_INCLUDED
#define GAMECONFIG_H_INCLUDED

#include "GameUtils.h"

namespace PirateSimulator
{
    namespace GameGlobals
    {
        struct SceneGlobals
        {
            enum
            {
                MAX_X = 4100,
                MAX_Z = 4100,

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

        struct SunGlobals
        {
        private:
            static constexpr float SUN_POWER = 1.0f;

            
        public:
            static constexpr float POWER = MathUtils::static_clamp(SUN_POWER, 0.f, 1.0f);

            static constexpr const float MIN_AMBIENT_LIGHT = 0.10f;
            static constexpr const float AMBIENT_LIGHT_COEFF = 0.15f;

            static constexpr float X_SUN = 10000.f;
            static constexpr float Y_SUN = 0.f;
        };
    }

//#define DEBUG_TEST_TERRAIN

#define DEBUG_PIRATE_SIMULATOR

#ifdef DEBUG_PIRATE_SIMULATOR
    extern size_t debugCount;
#endif
}


#endif //!GAMECONFIG_H_INCLUDED