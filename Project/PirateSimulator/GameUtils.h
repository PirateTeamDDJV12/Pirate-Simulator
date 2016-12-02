#ifndef GAMEUTILS_H_INCLUDED
#define GAMEUTILS_H_INCLUDED

namespace PirateSimulator
{
    struct MathUtils
    {
        //clamp value between min and max included
        template<class Type>
        static constexpr Type static_clamp(Type value, Type min, Type max)
        {
            return (value < min) ? min : (value > max) ? max : value;
        }
    };
}

#endif //!GAMEUTILS_H_INCLUDED