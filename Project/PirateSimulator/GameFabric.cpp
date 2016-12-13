#include "GameFabric.h"

#include "Skybox.h"

//#include "Transform.h"

#include "GameObjectManager.h"
#include "CameraManager.h"
#include "RendererManager.h"


using namespace PirateSimulator;


void GameFabric::createSkybox()
{
    PirateSimulator::GameObjectRef m_skybox = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
        new PirateSimulator::GameObject(PirateSimulator::CameraManager::singleton.getMainCameraGO()->m_transform, "skybox")
    );

    PirateSimulator::CSkybox* skyBoxMesh = new PirateSimulator::CSkybox();
    m_skybox->addComponent<PirateSimulator::IMesh>(skyBoxMesh);

    skyBoxMesh->setTexture(L"PirateSimulator/skybox.dds");
    RendererManager::singleton.addAnObligatoryMeshToDrawBefore(skyBoxMesh);
}

void GameFabric::createWater(const Transform& fieldTransform)
{
    auto water = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
        new PirateSimulator::GameObject(fieldTransform, "water")
    );

    PirateSimulator::Plane* waterMesh = new PirateSimulator::Plane("PirateSimulator/water.dds");
    water->addComponent<PirateSimulator::IMesh>(waterMesh);

    PirateSimulator::RendererManager::singleton.addAnObligatoryMeshToDrawBefore(waterMesh);
}