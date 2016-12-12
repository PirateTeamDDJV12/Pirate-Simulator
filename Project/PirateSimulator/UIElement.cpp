#include "UIElement.h"
#include "..\PetitMoteur3D\PetitMoteur3D\AfficheurSprite.h"
#include <DirectXMath.h>
#include "GameObject.h"

using namespace PirateSimulator;
using namespace DirectX;

UIRef UIBase::getTitleScreen() const noexcept
{
    return m_baseMenu;
}

bool UIBase::operator()()
{
    // todo
    return m_baseMenu->update();
}
