#ifndef COMPONENENT_H_INCLUDED
#define COMPONENENT_H_INCLUDED

#include <string>

//#include "GameObject.h"

namespace PirateSimulator
{
    class GameObject;

	class Component 
    {
    protected:
        GameObject *m_gameObject;
        bool        m_enabled;


    public:
        Component() : m_enabled{true}
        {}

        virtual void setGameObject(GameObject* parent)
        {
            m_gameObject = parent;
        }

        GameObject* getGameObject()
        {
            return m_gameObject;
        }

        bool isEnabled() const
        {
            return m_enabled;
        }

        void enable(bool state)
        {
            m_enabled = state;
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

#endif //!COMPONENENT_H_INCLUDED