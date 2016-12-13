#include <DirectXMath.h>

#include "UIMenu.h"
#include "GameObjectManager.h"
#include "InputManager.h"

using namespace PirateSimulator;
using namespace DirectX;

UIMenu::UIMenu(UIMainMenuLogic *parent) : m_parent{parent}
{
    PirateSimulator::Transform transform;

    transform.setPosition({0,0,0,0});
    transform.setRight({1,0,0,0});
    transform.setUp({0,1,0,0});
    transform.setForward({0,0,-1,0});

    m_mainMenuImages = new PM3D::CAfficheurSprite();
    m_buttons[0] = createButton([this]() {m_parent->setGameState(GameState::Starting); });
    m_buttons[1] = createButton([this]() {m_parent->setGameState(GameState::Options); });
    m_buttons[2] = createButton([this]() {m_parent->setGameState(GameState::Credits); });
    m_buttons[3] = createButton([this]() {m_parent->setGameState(GameState::Quitting); });

    m_currentButton = 0;
    // ajout de panneaux 
    m_mainMenuImages->AjouterPanneau("Assets/UI/MainMenu/Background Image.dds",
                                     XMFLOAT3(-1.0f, -1.0f, 0.0f),
                                     2.0f, 2.0f);
    m_mainMenuImages->AjouterPanneau("Assets/UI/MainMenu/Titre.dds",
                                     XMFLOAT3(-0.7f, 0.5f, -1.0f));
    PM3D::CAfficheurSprite *startHighlightedSprites = new PM3D::CAfficheurSprite();
    PM3D::CAfficheurSprite *optionsHighlightedSprites = new PM3D::CAfficheurSprite();
    PM3D::CAfficheurSprite *creditsHighlightedSprites = new PM3D::CAfficheurSprite();
    PM3D::CAfficheurSprite *quitHighlightedSprites = new PM3D::CAfficheurSprite();
    PM3D::CAfficheurSprite *startSprite = new PM3D::CAfficheurSprite();
    PM3D::CAfficheurSprite *optionSprite = new PM3D::CAfficheurSprite();
    PM3D::CAfficheurSprite *creditsSprite = new PM3D::CAfficheurSprite();
    PM3D::CAfficheurSprite *quitSprite = new PM3D::CAfficheurSprite();


    // Start Button Highlighting
    startSprite->AjouterPanneau("Assets/UI/MainMenu/Start.dds",
                                XMFLOAT3(-0.7f, 0.2f, -1.0f));
    startHighlightedSprites->AjouterPanneau("Assets/UI/MainMenu/Highlighted Start.dds",
                                            XMFLOAT3(-1.1f, 0.15f, -1.0f));
    startHighlightedSprites->AjouterPanneau("Assets/UI/MainMenu/Separation.dds",
                                            XMFLOAT3(-0.7f, 0.2f, -1.0f));
    m_buttons[0]->setDefaultSprites(startSprite);
    m_buttons[0]->setSelectedSpriteDrawer(startHighlightedSprites);

    // Options Button Highlighting
    optionSprite->AjouterPanneau("Assets/UI/MainMenu/Options.dds",
                                 XMFLOAT3(-0.7f, 0.0f, -1.0f));
    optionsHighlightedSprites->AjouterPanneau("Assets/UI/MainMenu/Highlighted Options.dds",
                                              XMFLOAT3(-1.1f, -0.05f, -1.0f));
    optionsHighlightedSprites->AjouterPanneau("Assets/UI/MainMenu/Separation.dds",
                                              XMFLOAT3(-0.7f, 0.0f, -1.0f));
    m_buttons[1]->setDefaultSprites(optionSprite);
    m_buttons[1]->setSelectedSpriteDrawer(optionsHighlightedSprites);

    // Credits Button Highlighting
    creditsSprite->AjouterPanneau("Assets/UI/MainMenu/Credits.dds",
                                  XMFLOAT3(-0.7f, -0.2f, -1.0f));
    creditsHighlightedSprites->AjouterPanneau("Assets/UI/MainMenu/Highlighted Credits.dds",
                                              XMFLOAT3(-1.1f, -0.25f, -1.0f));
    creditsHighlightedSprites->AjouterPanneau("Assets/UI/MainMenu/Separation.dds",
                                              XMFLOAT3(-0.7f, -0.2f, -1.0f));

    m_buttons[2]->setDefaultSprites(creditsSprite);
    m_buttons[2]->setSelectedSpriteDrawer(creditsHighlightedSprites);

    // Quit Button Highlighting
    quitSprite->AjouterPanneau("Assets/UI/MainMenu/Quit.dds",
                               XMFLOAT3(-0.7f, -0.4f, -1.0f));
    quitHighlightedSprites->AjouterPanneau("Assets/UI/MainMenu/Highlighted Quit.dds",
                                           XMFLOAT3(-1.1f, -0.45f, -1.0f));
    quitHighlightedSprites->AjouterPanneau("Assets/UI/MainMenu/Separation.dds",
                                           XMFLOAT3(-0.7f, -0.4f, -1.0f));
    m_buttons[3]->setDefaultSprites(quitSprite);
    m_buttons[3]->setSelectedSpriteDrawer(quitHighlightedSprites);

    m_buttons[m_currentButton]->onSelect();
}

void UIMenu::initialize()
{}

bool UIMenu::update()
{
    //TODO
    CDIManipulateur& input = InputManager::singleton.getManipulator();

    if(input.getButtonDown(DIK_DOWNARROW))
    {
        if(m_currentButton < 3)
        {
            m_buttons[m_currentButton]->onUnselect();
            ++m_currentButton;
            m_buttons[m_currentButton]->onSelect();
        }
    }
    else if((input.getButtonDown(DIK_UPARROW)))
    {
        if(m_currentButton != 0)
        {
            m_buttons[m_currentButton]->onUnselect();
            --m_currentButton;
            m_buttons[m_currentButton]->onSelect();
        }
    }
    if(input.getButtonDown(DIK_RETURN))
        m_buttons[m_currentButton]->onPressed();

    // Draw sprites
    m_mainMenuImages->Draw();
    for(auto button : m_buttons)
        button->draw();
    return false;
}

bool UIMenu::select()
{
    //TODO
    return true;
}

bool UIMenu::cancel()
{
    //TODO
    return true;
}