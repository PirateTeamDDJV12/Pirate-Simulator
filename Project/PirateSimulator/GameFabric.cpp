#include "GameFabric.h"

#include "Skybox.h"
#include "Plane.h"
#include "Terrain.h"

//#include "Transform.h"

#include "GameObjectManager.h"
#include "CameraManager.h"
#include "RendererManager.h"

#include "..\PetitMoteur3D\PetitMoteur3D\ObjetMesh.h"

#include "VehicleShape.h"


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

    Plane* waterMesh = new Plane("PirateSimulator/water.dds");
    water->addComponent<IMesh>(waterMesh);

    RendererManager::singleton.addAnObligatoryMeshToDrawBefore(waterMesh);
}

void GameFabric::createBoat(const Transform& boatTransform)
{
    GameObjectRef vehicule = GameObjectManager::singleton.subscribeAGameObject(
        new GameObject(boatTransform, "vehicule")
    );

    /*CObjetMesh().ConvertToOMB(".\\modeles\\Boat\\boat.obj", ".\\modeles\\Boat\\boat.OMB");*/
    /*CParametresChargement param;
    param.bInverserCulling = false;
    param.bMainGauche = true;
    param.NomChemin = ".\\modeles\\Boat\\";
    param.NomFichier = "boat.obj";
    CChargeurOBJ chargeur;
    chargeur.Chargement(param);*/

    //Mesh
    auto vehiculeMesh = new PM3D::CObjetMesh(".\\modeles\\Boat\\boat.OMB", PM3D::ShaderCObjectMesh::ShadersParams());
    vehicule->addComponent<IMesh>(vehiculeMesh);
    RendererManager::singleton.addAMovingSortableMesh(vehiculeMesh);
    
    //Behavior
    vehicule->addComponent<IBehaviour>(new PlayerBehaviour());
    
    //Shape
    auto vehicleShape = new VehicleShape();
    vehicule->addComponent<ShapeComponent>(vehicleShape);

    //Set the pairedTarget of the camera in case the camera is Object type camera
    CameraManager& cameraManager = CameraManager::singleton;

    if (cameraManager.getCameraType() == cameraModule::BaseCamera::OBJECT_CAMERA)
    {
        cameraManager.setPairedTarget(vehicule);
    }
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
    //terrain->addComponent<ShapeComponent>(fieldShape);


    //Set the pairedTarget of the camera in case the camera is Level type camera
    CameraManager& cameraManager = CameraManager::singleton;

    // Set the gameobject which is paired to the camera
    if (cameraManager.getCameraType() == cameraModule::BaseCamera::LEVEL_CAMERA)
    {
        cameraManager.setPairedTarget(field);
    }
}