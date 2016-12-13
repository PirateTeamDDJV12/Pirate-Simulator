#include "GameFabric.h"

#include "Skybox.h"
#include "Plane.h"

//#include "Transform.h"

#include "GameObjectManager.h"
#include "CameraManager.h"
#include "RendererManager.h"

#include "..\PetitMoteur3D\PetitMoteur3D\ObjetMesh.h"

#include "VehicleShape.h"


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

void GameFabric::createBoat(const Transform& fieldTransform)
{
    PirateSimulator::GameObjectRef vehicule = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
        new PirateSimulator::GameObject(fieldTransform, "vehicule")
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
    PirateSimulator::RendererManager::singleton.addAMovingSortableMesh(vehiculeMesh);
    
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