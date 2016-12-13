#include "UIHUD.h"
#include "RendererManager.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"

PirateSimulator::UIHUD::UIHUD() : GameObject(Transform(), "HUDGO")
{
    // Create the police
    const FontFamily oFamily(L"Arial", NULL);
    m_police = new Font(&oFamily, 20.00, FontStyleBold, UnitPixel);

    // Create afficheur sprite
    m_afficheurSprite = new PM3D::CAfficheurSprite();

    // Add speed text
    m_speed = new PM3D::CAfficheurTexte(250, 50, m_police, Gdiplus::Color(255, 255, 255, 255));
    m_speed->Ecrire(L"0 noeud");

    // Add score text
    m_score = new PM3D::CAfficheurTexte(50, 50, m_police, Gdiplus::Color(255, 255, 255, 255));
    m_score->Ecrire(std::to_wstring(GameObjectManager::singleton.getPieceAdministrator()->getScore()));

    // Add time text
    m_time = new PM3D::CAfficheurTexte(150, 50, m_police, Gdiplus::Color(255, 255, 255, 255));
    std::chrono::milliseconds timeFromStart = TimeManager::GetInstance().getTimeFromStart();
    m_time->Ecrire(TimeUtils::timeToString(timeFromStart));

    // Add all text to the afficheur sprite
    m_afficheurSprite->AjouterSpriteTexte(m_speed->GetTextureView(), 850, 780);
    m_afficheurSprite->AjouterSpriteTexte(m_score->GetTextureView(), 50, 50);
    m_afficheurSprite->AjouterSpriteTexte(m_time->GetTextureView(), 480, 50);

    // Add the mesh to our game object
    this->addComponent<PirateSimulator::IMesh>(m_afficheurSprite);
    PirateSimulator::RendererManager::singleton.addAnObligatoryMeshToDrawAtEnd(m_afficheurSprite);
}

void PirateSimulator::UIHUD::anime(float elapsedTime)
{
    // Update time text
    std::chrono::milliseconds timeFromStart = TimeManager::GetInstance().getTimeFromStart();
    m_time->Ecrire(TimeUtils::timeToString(timeFromStart));

    m_score->Ecrire(std::to_wstring(GameObjectManager::singleton.getPieceAdministrator()->getScore()));
    // TODO - update speed text
}
