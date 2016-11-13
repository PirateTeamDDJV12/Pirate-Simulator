#ifndef MOVES_H_INCLUDED
#define MOVES_H_INCLUDED

namespace PirateSimulator
{
    namespace Move
    {
        namespace Translation
        {
            enum Direction
            {
                NONE,
                FORWARD,
                BACKWARD,
                LEFT,
                RIGHT,
                UP,
                DOWN
            };
        }
        
        namespace Rotation
        {
            enum Direction
            {
                NONE,
                X_CLOCKWISE,
                X_INVERT_CLOCKWISE,
                Y_CLOCKWISE,
                Y_INVERT_CLOCKWISE,
                Z_CLOCKWISE,
                Z_INVERT_CLOCKWISE,
            };
        }
    }
}


#endif //!MOVES_H_INCLUDED