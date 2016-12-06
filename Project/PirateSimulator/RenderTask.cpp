#include "RenderTask.h"

#include "../PetitMoteur3D/PetitMoteur3D/dispositif.h"
#include "RendererManager.h"
#include "../PetitMoteur3D/PetitMoteur3D/MoteurWindows.h"

using namespace PirateSimulator;


void RenderTask::init()
{
    RendererManager::singleton.init(PM3D::CDS_FENETRE, PM3D::CMoteurWindows::GetInstance().getMainWindowHandle());
}

void RenderTask::update()
{
    auto pDispositif = PirateSimulator::RendererManager::singleton.getDispositif();
    ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();
    ID3D11RenderTargetView* pRenderTargetView = pDispositif->GetRenderTargetView();


    // On efface la surface de rendu
    float Couleur[4] = {0.0f, 0.5f, 0.0f, 1.0f};  //  RGBA - Vert pour le moment
    pImmediateContext->ClearRenderTargetView(pRenderTargetView, Couleur);

    // On ré-initialise le tampon de profondeur
    ID3D11DepthStencilView* pDepthStencilView = pDispositif->GetDepthStencilView();
    pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // On rend l'image sur la surface de travail 
    // (tampon d'arrière plan)
    // Appeler les fonctions de dessin de chaque objet de la scène
    PirateSimulator::RendererManager::singleton.draw();
}
