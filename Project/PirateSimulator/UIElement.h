#ifndef UIELEMENT_H_INCLUDED
#define UIELEMENT_H_INCLUDED

#include <memory>

namespace PirateSimulator 
{
    class UIElementBase;

    using UIRef = std::shared_ptr<UIElementBase>;


    class UIElementBase
    {
    public:
        virtual ~UIElementBase() = default;


    public:
        virtual void initialize() = 0;
        virtual bool update() = 0;
        virtual bool select() = 0;
        virtual bool cancel() = 0;
        

    public:
        template<class UIObject>
        constexpr UIObject* as()
        {
            static_assert(
                std::is_base_of<UIElementBase, UIObject>::value ||
                std::is_same<UIElementBase, UIObject>::value ||
                std::is_convertible<UIObject, UIElementBase>::value,
                "Not an UI Object"
            );

            return static_cast<UIObject*>(this);
        }
    };



    class UIBase
    {
    private:
        UIRef m_baseMenu;


    public:
        UIBase(UIRef titleScreenMenu) :
            m_baseMenu{ titleScreenMenu }
        {}


    public:
        UIRef getTitleScreen() const noexcept;


    public:
        bool operator()();
    };
}

#endif // !UIELEMENT_H_INCLUDED


