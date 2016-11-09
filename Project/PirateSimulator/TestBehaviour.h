#ifndef _TESTBEHAVIOUR_H_
#define _TESTBEHAVIOUR_H_

#include "IBehaviour.h"

namespace PirateSimulator
{

    class TestBehaviour : public IBehaviour
    {
    private:
        float rotation;
        float vitesse;

    public:

        TestBehaviour() :
            rotation{ 0.0f },
            vitesse{ 2.0f }
        {}

        virtual void anime(float ellapsedTime);

    };

}


#endif // !_TESTBEHAVIOUR_H_
