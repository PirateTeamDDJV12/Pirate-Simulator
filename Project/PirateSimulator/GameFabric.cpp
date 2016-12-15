#include "GameFabric.h"

//Manager
#include "GameObjectManager.h"
#include "CameraManager.h"
#include "RendererManager.h"

//Mesh
#include "..\PetitMoteur3D\PetitMoteur3D\ObjetMesh.h"
#include "Skybox.h"
#include "Plane.h"
#include "Terrain.h"

//Shape
#include "VehicleShape.h"
#include "TerrainShape.h"
#include "TunnelShape.h"
#include "TriggerShape.h"
//Behavior
#include "PlayerBehaviour.h"

//UI
#include "UIHUD.h"

//Loader
#include "..\PetitMoteur3D\PetitMoteur3D\ChargeurAssimp.h"

using namespace PirateSimulator;

void GameFabric::createSkybox()
{
    GameObjectRef m_skybox = GameObjectManager::singleton.subscribeAGameObject(
        new GameObject(CameraManager::singleton.getMainCameraGO()->m_transform, "skybox")
    );

    CSkybox* skyBoxMesh = new CSkybox();
    m_skybox->addComponent<IMesh>(skyBoxMesh);

    skyBoxMesh->setTexture(L"PirateSimulator/skybox.dds");
    RendererManager::singleton.addAnObligatoryMeshToDrawBefore(skyBoxMesh);
}

void GameFabric::createWater(const Transform& fieldTransform)
{
    auto water = GameObjectManager::singleton.subscribeAGameObject(
        new GameObject(fieldTransform, "water")
    );

    Plane* waterMesh = new Plane("PirateSimulator/water3.dds");
    water->addComponent<IMesh>(waterMesh);

    RendererManager::singleton.addAnObligatoryMeshToDrawBefore(waterMesh);
}

void GameFabric::createField(const Transform& fieldTransform)
{
    //GO
    GameObjectRef field = GameObjectManager::singleton.subscribeAGameObject(
        new GameObject(fieldTransform, "terrain")
    );

    //Mesh
#ifdef DEBUG_TEST_TERRAIN        
    // If we want to test our own terrain and not the one int the config file

    int fieldH = 257;
    int fieldW = 257;
    int fieldScale = 4;

    auto fieldMesh = new Terrain(fieldH, fieldW, fieldScale, "PirateSimulator/heightmapOutput.txt", "PirateSimulator/textureTerrain.dds");
#else
    // Get all the information from the config file
    auto fieldMesh = new Terrain();
#endif
    field->addComponent<IMesh>(fieldMesh);

    // Puis, il est ajouté à la scène
    RendererManager::singleton.addAnObligatoryMeshToDrawBefore(fieldMesh);

    // Add the shape for Terrain
    auto fieldShape = new TerrainShape();
    field->addComponent<ShapeComponent>(fieldShape);
}

void GameFabric::createCameraAndBoat(const Transform& cameraTransform, const Transform& boatTransform)
{
    RendererManager& rendererManager = RendererManager::singleton;

    auto camProjParameters = cameraModule::CameraProjectionParameters(
        XM_PI / 4,
        GameGlobals::CameraGlobals::NEAREST_PLANE,
        GameGlobals::CameraGlobals::FARTHEST_PLANE,
        rendererManager.getDispositif()->GetLargeur(),
        rendererManager.getDispositif()->GetHauteur()
    );

    GameObjectRef vehicule = GameObjectManager::singleton.subscribeAGameObject(
        new GameObject(boatTransform, "vehicule")
    );

    //Mesh
    PM3D::CChargeurAssimp chargeur;

    // Création du mesh du boat à partir d'un fichier .OBJ
    PM3D::CParametresChargement paramBoat(".\\modeles\\Boat\\", "boat.obj", false, false);
    chargeur.Chargement(paramBoat);

    auto vehiculeMesh = new PM3D::CObjetMesh(PM3D::ShaderCObjectMesh::ShadersParams(), L"MiniPhong.fx", chargeur);
    vehicule->addComponent<IMesh>(vehiculeMesh);
    RendererManager::singleton.addAMovingSortableMesh(vehiculeMesh);

    //Behavior
    vehicule->addComponent<IBehaviour>(new PlayerBehaviour());

    //Shape
    auto vehicleShape = new VehicleShape();
    vehicule->addComponent<ShapeComponent>(vehicleShape);

    CameraManager::singleton.createObjectCamera(
        vehicule,
        cameraTransform,
        camProjParameters,
        "mainCamera"
    );
}

void GameFabric::createHUD()
{
    GameObjectManager::singleton.subscribeAGameObject(new UIHUD());
}

void GameFabric::createTunnel(const Transform& tunnelTransform)
{
    PirateSimulator::GameObjectRef tunnel = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
        new PirateSimulator::GameObject(tunnelTransform, "tunnel")
    );
    //game object copy to create trigger shape (can't define two shapes for one go)
    PirateSimulator::GameObjectRef tunnelTrigger = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
        new PirateSimulator::GameObject(tunnelTransform, "tunnelTrigger")
    );


    PM3D::CChargeurAssimp chargeur;

    // Création du mesh du tunnel à partir d'un fichier .OBJ
    PM3D::CParametresChargement paramTunnel(".\\modeles\\Tunnel\\", "tunnel.obj", false, true);
    chargeur.Chargement(paramTunnel);

    auto tunnelMesh = new PM3D::CObjetMesh(PM3D::ShaderCObjectMesh::ShadersParams(), L"MiniPhongField.fx", chargeur);
    tunnelMesh->setBackFaceCulling(false);

    tunnel->addComponent<PirateSimulator::IMesh>(tunnelMesh);
    auto tunnelshape = new TunnelShape();
    tunnel->addComponent<ShapeComponent>(tunnelshape);
    PirateSimulator::RendererManager::singleton.addAStaticSortableMesh(tunnelMesh);

    auto triggerShape = new TriggerShape();
    tunnelTrigger->addComponent<ShapeComponent>(triggerShape);
}

void GameFabric::createPieces()
{
    GameObjectManager::singleton.init();
}