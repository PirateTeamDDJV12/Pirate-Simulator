#include <DirectXMath.h>

#include "UIMenu.h"
#include "GameObjectManager.h"
#include "RendererManager.h"

using namespace PirateSimulator;
using namespace DirectX;

UIMenu::UIMenu()
{
    PirateSimulator::Transform transform;

    transform.setPosition({0,0,0,0});
    transform.setRight({1,0,0,0});
    transform.setUp({0,1,0,0});
    transform.setForward({0,0,-1,0});

    pAfficheurSprite = new PM3D::CAfficheurSprite();

    // ajout de panneaux 
    pAfficheurSprite->AjouterPanneau("Assets/UI/MainMenu/Background Image.dds",
                                     XMFLOAT3(-1.0f, -1.0f, 0.0f),
                                     2.0f, 2.0f);
    pAfficheurSprite->AjouterPanneau("Assets/UI/MainMenu/Titre.dds",
                                     XMFLOAT3(-0.7f, 0.5f, -1.0f));
    pAfficheurSprite->AjouterPanneau("Assets/UI/MainMenu/Start.dds",
                                     XMFLOAT3(-0.7f, 0.2f, -1.0f));
    pAfficheurSprite->AjouterPanneau("Assets/UI/MainMenu/Highlighted Start.dds",
                                     XMFLOAT3(-1.1f, 0.15f, -1.0f));
    pAfficheurSprite->AjouterPanneau("Assets/UI/MainMenu/Options.dds",
                                     XMFLOAT3(-0.7f, 0.0f, -1.0f));
    pAfficheurSprite->AjouterPanneau("Assets/UI/MainMenu/Credits.dds",
                                     XMFLOAT3(-0.7f, -0.2f, -1.0f));
    pAfficheurSprite->AjouterPanneau("Assets/UI/MainMenu/Quit.dds",
                                     XMFLOAT3(-0.7f, -0.4f, -1.0f));

    PirateSimulator::GameObjectRef Sprite = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
        new PirateSimulator::GameObject(transform, "sprite")
    );
    Sprite->addComponent<PirateSimulator::IMesh>(pAfficheurSprite);
    PirateSimulator::RendererManager::singleton.addAnObligatoryMeshToDrawBefore(pAfficheurSprite);
}

void UIMenu::initialize()
{}

bool UIMenu::update()
{
    //TODO
    pAfficheurSprite->Draw();
    return true;
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