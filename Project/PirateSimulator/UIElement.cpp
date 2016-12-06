#include "UIElement.h"

using namespace PirateSimulator;

UIRef UIBase::getTitleScreen() const noexcept
{
    return m_baseMenu;
}

bool UIBase::operator()()
{
    // todo

    return true;
}
