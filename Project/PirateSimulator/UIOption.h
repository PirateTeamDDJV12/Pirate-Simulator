#ifndef UIOPTION_H_INCLUDED
#define UIOPTION_H_INCLUDED

#include "UIElement.h"
#include "UIOptionSelector.h"

#include <vector>
#include <memory>
#include <type_traits>

namespace PirateSimulator 
{
    class Selector{};
    struct OptionItem{};

    class OptionGraphic;


    class UIOption : public UIElementBase
    {
    public:
        using AlgorithmObject = Algorithm<UIOption, void(UIOption::*)()>;


    private:
        std::unique_ptr<UIOptionSelector<Selector, UIOption, AlgorithmObject>> m_selector;
        std::vector<OptionItem> m_items;

        std::unique_ptr<OptionGraphic> m_graphics;


    public:
        UIOption();


    public:
        template<class ... Args>
        void attachGraphics(Args&& ... graphicsArguments)
        {
            m_graphics = std::make_unique<OptionGraphic>(std::forward<Args...>(graphicsArguments...));
        }

        void attachItem(OptionItem& optionItem, AlgorithmObject algoCallback);

        OptionItem& getItem(size_t index);


    public:
        virtual void initialize();
        virtual bool update();
        virtual bool select();
        virtual bool cancel();

        void draw();


    public:
        UIOption& operator++() noexcept
        {
            ++(*m_selector);
            return *this;
        }

        UIOption& operator++(int) noexcept
        {
            return ++*this;
        }

        UIOption& operator--() noexcept
        {
            --(*m_selector);
            return *this;
        }
        
        UIOption& operator--(int) noexcept
        {
            return --*this;
        }
    };
}

#endif // !UIOPTION_H_INCLUDED


