#include "UIElement.h"
#include "..\PetitMoteur3D\PetitMoteur3D\AfficheurSprite.h"

using namespace PirateSimulator;

UIRef UIBase::getTitleScreen() const noexcept
{
    return m_baseMenu;
}

bool UIBase::operator()()
{
    // todo
    return m_baseMenu->update();
}
