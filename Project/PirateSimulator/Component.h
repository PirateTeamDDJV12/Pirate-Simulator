/*
Created by Sun-lay Gagneux
*/
#ifndef COMPONENENT_H
#define COMPONENENT_H

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

    };
}

#endif //!COMPONENENT_H