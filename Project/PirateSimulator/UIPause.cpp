#include "UIPause.h"
#include "UIPauseLogic.h"

#include "InputManager.h"

#include "Transform.h"

#include "..\PetitMoteur3D\PetitMoteur3D\DIManipulateur.h"

#include <DirectXMath.h>


using namespace PirateSimulator;
using namespace DirectX;


UIPause::UIPause(UIPauseLogic* parent) : m_parent { parent }
{
    PirateSimulator::Transform transform;

    transform.setPosition({ 0,0,0,0 });
    transform.setRight({ 1,0,0,0 });
    transform.setUp({ 0,1,0,0 });
    transform.setForward({ 0,0,-1,0 });

    // Buttons for the pause menu
    m_mainMenuImages = new PM3D::CAfficheurSprite();
    m_buttons[0] = createButton([this]() {m_parent->setPauseState(UIPauseLogic::PauseState::Resuming); });
    m_buttons[1] = createButton([this]() {m_parent->setPauseState(UIPauseLogic::PauseState::Leaving); });

    m_currentButton = 0;

    // Sprites for the pause menu
    PM3D::CAfficheurSprite *resumeSprite = new PM3D::CAfficheurSprite();
    PM3D::CAfficheurSprite* resumeHighlightedSprites = new PM3D::CAfficheurSprite();

    PM3D::CAfficheurSprite *quitSprite = new PM3D::CAfficheurSprite();
    PM3D::CAfficheurSprite* quitHighlightedSprites = new PM3D::CAfficheurSprite();

    // Start Button Highlighting
    resumeSprite->AjouterPanneau("Assets/UI/MainMenu/Start.dds", XMFLOAT3(-0.7f, 0.2f, -1.0f));
    resumeHighlightedSprites->AjouterPanneau("Assets/UI/MainMenu/Highlighted Start.dds", XMFLOAT3(-1.1f, 0.15f, -1.0f));
    resumeHighlightedSprites->AjouterPanneau("Assets/UI/MainMenu/Separation.dds", XMFLOAT3(-0.7f, 0.2f, -1.0f));
    
    m_buttons[0]->setDefaultSprites(resumeSprite);
    m_buttons[0]->setSelectedSpriteDrawer(resumeHighlightedSprites);

    // Quit Button Highlighting
    quitSprite->AjouterPanneau("Assets/UI/MainMenu/Quit.dds", XMFLOAT3(-0.7f, 0.0f, -1.0f));
    quitHighlightedSprites->AjouterPanneau("Assets/UI/MainMenu/Highlighted Quit.dds", XMFLOAT3(-1.1f, -0.05f, -1.0f));
    quitHighlightedSprites->AjouterPanneau("Assets/UI/MainMenu/Separation.dds", XMFLOAT3(-0.7f, 0.0f, -1.0f));
    
    m_buttons[1]->setDefaultSprites(quitSprite);
    m_buttons[1]->setSelectedSpriteDrawer(quitHighlightedSprites);

    m_buttons[m_currentButton]->onSelect();
}

void UIPause::initialize()
{
    // TODO
}

bool UIPause::update()
{
    InputManager& inputManager = InputManager::singleton;
    //update to get the keys...
    inputManager.update();

    CDIManipulateur& input = inputManager.getManipulator();
    
    if (input.getButtonDown(DIK_DOWNARROW))
    {
        if (m_currentButton < 1)
        {
            m_buttons[m_currentButton]->onUnselect();
            ++m_currentButton;
            m_buttons[m_currentButton]->onSelect();
        }
    }
    else if ((input.getButtonDown(DIK_UPARROW)))
    {
        if (m_currentButton != 0)
        {
            m_buttons[m_currentButton]->onUnselect();
            --m_currentButton;
            m_buttons[m_currentButton]->onSelect();
        }
    }

    if (input.getButtonDown(DIK_RETURN))
    {
        m_buttons[m_currentButton]->onPressed();
    }

    // Draw sprites
    //m_mainMenuImages->Draw();
    for (auto button : m_buttons)
    {
        button->draw();
    }
    return true;
}

bool UIPause::select()
{
    // TODO
    return true;
}

bool UIPause::cancel()
{
    // TODO
    return true;
}