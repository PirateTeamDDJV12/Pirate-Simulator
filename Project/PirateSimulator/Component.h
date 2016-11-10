/*
Created by Sun-lay Gagneux
*/
#ifndef COMPONENENT_H
#define COMPONENENT_H

#include <string>

//#include "GameObject.h"

namespace PirateSimulator
{
    class GameObject;

	class Component 
    {
    protected:
        GameObject *m_gameObject;


    public:
        void setGameObject(GameObject* parent)
        {
            m_gameObject = parent;
        }

        GameObject* getGameObject()
        {
            return m_gameObject;
        }

        virtual std::string getTypeId() const noexcept = 0;

    public:
        static bool sameTypeIdAs(const std::string& fromCompare, const std::string& toCompare) noexcept
        {
            return fromCompare.size() == toCompare.size() && fromCompare == toCompare;
        }


    public:
        template<class Origin>
        Origin* as()
        {
            static_assert(std::is_convertible<Origin*, Component*>::value, "Casting component not valid");

            return static_cast<Origin*>(this);
        }
    };
}

#endif //!COMPONENENT_H