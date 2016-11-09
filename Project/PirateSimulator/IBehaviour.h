#ifndef _IBEHAVIOUR_H_
#define _IBEHAVIOUR_H_

#include "Component.h"


namespace PirateSimulator
{
    class IBehaviour : public Component
    {
    public:
        virtual void anime(float ellapsedTime) {} ;
    };
}

#endif // !_IBEHAVIOUR_H_
