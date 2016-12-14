#ifndef UIOPTIONSELECTOR_H_INCLUDED
#define UIOPTIONSELECTOR_H_INCLUDED

#include <vector>

namespace PirateSimulator 
{
    template<class BoundEntity, class CallbackType>
    class Algorithm
    {
    private:
        CallbackType m_call;


    public:
        Algorithm(CallbackType call) :
            m_call{ call }
        {}


    public:
        template<class ... Args>
        void operator()(BoundEntity& bound, Args&& ... args)
        {
            m_call(bound, std::forward<Args...>(args...));
        }
    };


    template<class Parent, class Biding, class CallbackType>
    class UIOptionSelector : public Parent
    {
    private:
        std::vector<CallbackType> m_callback;
        size_t m_current;

        Biding& m_boundEntity;


    public:
        UIOptionSelector(Biding& toBound) :
            m_boundEntity{ toBound },
            m_callback{},
            m_current{ 0 }
        {}


    public:
        size_t getCursorPosition() const noexcept
        {
            return m_current;
        }

        void setCursorPosition(size_t pos) noexcept
        {
            m_current = pos;
        }

        void addACallback(CallbackType callback)
        {
            m_callback.push_back(callback);
        }

        void removeACallback(size_t index)
        {
            m_callback.erase(index);
        }


    public:
        UIOptionSelector& operator++() noexcept
        {
            ++m_current;
            return *this;
        }

        UIOptionSelector& operator++(int)
        {
            return ++*this;
        }

        UIOptionSelector& operator--() noexcept
        {
            --m_current;
            return *this;
        }

        UIOptionSelector& operator--(int)
        {
            return --*this;
        }

        template<class ... Args>
        void operator()(Args&& ... args)
        {
            (m_callback[m_current])(m_boundEntity.getItem(m_current), std::forward<Args...>(args...));
        }
    };
}

#endif // !UIOPTIONSELECTOR_H_INCLUDED


