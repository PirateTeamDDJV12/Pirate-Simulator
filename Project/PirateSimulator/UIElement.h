#ifndef _UIELEMENT_H_
#define _UIELEMENT_H_

#include <memory>
#include <map>

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


    public:
        template<class UIObject>
        constexpr UIObject* as() const
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
}

#endif // !_UIELEMENT_H_


