#pragma once
#include "Singleton.h"
#include "dispositif.h" 

#include "Bloc.h"
#include "BlocEffet1.h"
#include "ObjetMesh.h"
#include "ChargeurOBJ.h"
#include "GestionnaireDeTextures.h"
#include "AfficheurSprite.h"
#include "AfficheurTexte.h"
#include "DIManipulateur.h"
#include "ChargeurAssimp.h"

#include "../../PirateSimulator/GameConfig.h"
#include "../../PirateSimulator/Mesh.h"
#include "../../PirateSimulator/Skybox.h"
#include "../../PirateSimulator/Plane.h"
#include "../../PirateSimulator/Terrain.h"
#include "../../PirateSimulator/LevelCameraBehaviour.h"
#include "../../PirateSimulator/FreeCameraBehaviour.h"
#include "../../PirateSimulator/ObjectCameraBehaviour.h"
#include "../../PirateSimulator/GameObject.h"
#include "../../PirateSimulator/PlayerBehaviour.h"
#include "../../PirateSimulator/ShapeComponent.h"
#include "../../PirateSimulator/VehicleShape.h"
#include "../../PirateSimulator/TerrainShape.h"

// Manager
#include "../../PirateSimulator/TimeManager.h"
#include "../../PirateSimulator/GameObjectManager.h"
#include "../../PirateSimulator/RendererManager.h"
#include "../../PirateSimulator/CameraManager.h"
#include "../../PirateSimulator/InputManager.h"
#include "../../PirateSimulator/BlocMesh.h"
#include "../../PirateSimulator/TaskManager.h"

// Tasks
#include "../../PirateSimulator/TimeTask.h"
#include "../../PirateSimulator/InputTask.h"
#include "../../PirateSimulator/PhysicsTask.h"
#include "../../PirateSimulator/RenderTask.h"
#include "../../PirateSimulator/PlayerTask.h"
#include "../../PirateSimulator/Piece.h"

//UI
#include "../../PirateSimulator/UIElement.h"
#include "../../PirateSimulator/UIMenu.h"
#include "../../PirateSimulator/UIHUD.h"

#include <thread>
#include <vector>
#include "../../PirateSimulator/Tunnel.h"




namespace PM3D
{

    const int IMAGESPARSECONDE = 60;

    //
    //   TEMPLATE : CMoteur
    //
    //   BUT : Template servant à construire un objet Moteur qui implantera les
    //         aspects "génériques" du moteur de jeu
    //
    //   COMMENTAIRES :
    //
    //        Comme plusieurs de nos objets représenteront des éléments uniques 
    //		  du système (ex: le moteur lui-même, le lien vers 
    //        le dispositif Direct3D), l'utilisation d'un singleton 
    //        nous simplifiera plusieurs aspects.
    //
    template <class T, class TClasseDispositif>
    class CMoteur : public CSingleton<T>
    {
        enum TasksOrder
        {
            TIMETASK,
            INPUTTASK,
            PHYSICSTASK,
            PLAYERTASK,
            RENDERTASK,
        };


    public:

        virtual void Run()
        {
            bool bBoucle = true;

            while (bBoucle)
            {
                // Propre à la plateforme - (Conditions d'arrêt, interface, messages)
                bBoucle = RunSpecific();

                PirateSimulator::TaskManager::GetInstance().update();

            }

        }

        virtual int Initialisations()
        {
            // Propre à la plateforme
            InitialisationsSpecific();

            // Création des tasks
            CreateTasks();

            bool resultUI = false;
            bool resultInit = false;

            std::vector<std::thread> beginThread;



            beginThread.emplace_back([this, &resultInit]() {
                // * Initialisation de la scène
                InitScene();
                resultInit = true;
            });

            for(size_t iter = 0; iter < beginThread.size(); ++iter)
            {
                beginThread[iter].join();
            }

            //PirateSimulator::UIBase titleScreen(PirateSimulator::UIRef(new PirateSimulator::UIMenu));

            while(true)
            {
                if(!RunSpecific())
                {
                    beginThread.front().detach();
                    return 1;
                }
                auto pDispositif = PirateSimulator::RendererManager::singleton.getDispositif();
                ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();
                ID3D11RenderTargetView* pRenderTargetView = pDispositif->GetRenderTargetView();

                // On efface la surface de rendu
                float Couleur[4] = {0.0f, 0.5f, 0.0f, 1.0f};  //  RGBA - Vert pour le moment
                pImmediateContext->ClearRenderTargetView(pRenderTargetView, Couleur);

                // On ré-initialise le tampon de profondeur
                ID3D11DepthStencilView* pDepthStencilView = pDispositif->GetDepthStencilView();
                pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
                if((resultInit))
                    break;
                pDispositif->Present();
            }

            //beginThread.front().detach();

            // Start the game time when all menu and loading screen are close to begin the game
            TimeManager::GetInstance().startGameTime();

            return 0;
        }

        void CreateTasks()
        {
            PirateSimulator::TaskManager* taskManager = &PirateSimulator::TaskManager::GetInstance();

            taskManager->addTask<PirateSimulator::TimeTask>(TasksOrder::TIMETASK);
            taskManager->addTask<PirateSimulator::InputTask>(TasksOrder::INPUTTASK);
            taskManager->addTask<PirateSimulator::RenderTask>(TasksOrder::RENDERTASK);
            taskManager->addTask<PirateSimulator::PhysicsTask>(TasksOrder::PHYSICSTASK);
            taskManager->addTask<PirateSimulator::PlayerTask>(TasksOrder::PLAYERTASK);
        }

        CGestionnaireDeTextures& GetTextureManager()
        {
            return TexturesManager;
        }

    protected:

        // Constructeur par défaut
        CMoteur(void)
        {}

        // Destructeur
        ~CMoteur(void)
        {
            Cleanup();
        }

        // Spécifiques - Doivent être implantés
        virtual bool RunSpecific() = 0;
        virtual int InitialisationsSpecific() = 0;
        virtual __int64 GetTimeSpecific() = 0;
        //virtual TClasseDispositif* CreationDispositifSpecific(const CDS_MODE cdsMode) = 0;
        virtual void BeginRenderSceneSpecific() = 0;
        virtual void EndRenderSceneSpecific() = 0;

        virtual void Cleanup()
        {
            // Vider les textures
            TexturesManager.Cleanup();
            //Vider les objets physiques
            PirateSimulator::PhysicsManager::singleton.reset();
        }

        virtual int InitScene()
        {
            auto camProjParameters = PirateSimulator::cameraModule::CameraProjectionParameters(
                XM_PI / 4,
                PirateSimulator::GameGlobals::CameraGlobals::NEAREST_PLANE,
                PirateSimulator::GameGlobals::CameraGlobals::FARTHEST_PLANE,
                PirateSimulator::RendererManager::singleton.getDispositif()->GetLargeur(),
                PirateSimulator::RendererManager::singleton.getDispositif()->GetHauteur()
            );

            auto camMovParameters = PirateSimulator::cameraModule::CameraMovingParameters(
                PirateSimulator::GameGlobals::CameraGlobals::LINEAR_SPEED,
                PirateSimulator::GameGlobals::CameraGlobals::ANGULAR_SPEED);

            PirateSimulator::RendererManager::singleton.setSortingMesh(true);
            PirateSimulator::RendererManager::singleton.setDetailLevel(PirateSimulator::RendererManager::DEEP_ARRANGEMENT);

            // Initialisation des matrices View et Proj
            // Dans notre cas, ces matrices sont fixes
            PirateSimulator::Transform cameraTransform = PirateSimulator::Transform();
            cameraTransform.setPosition(XMVectorSet(0.f, 0.f, -10.f, 0.f));
            cameraTransform.setUp(XMVectorSet(0.f, 1.f, 0.f, 0.f));
            cameraTransform.setForward(XMVectorSet(0.f, 0.f, 1.f, 0.f));

            PirateSimulator::CameraManager::singleton.createCamera(
                PirateSimulator::cameraModule::BaseCamera::type::OBJECT_CAMERA,
                cameraTransform,
                camProjParameters,
                camMovParameters,
                "mainCamera"
            );

            // Skybox
            PirateSimulator::CSkybox* skyBoxMesh = new PirateSimulator::CSkybox();
            m_skybox = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
                new PirateSimulator::GameObject(PirateSimulator::CameraManager::singleton.getMainCameraGO()->m_transform, "skybox")
            );
            m_skybox->addComponent<PirateSimulator::IMesh>(skyBoxMesh);
            skyBoxMesh->SetTexture(new CTexture(L"PirateSimulator/skybox.dds"));
            PirateSimulator::RendererManager::singleton.addAnObligatoryMeshToDrawBefore(skyBoxMesh);

            auto HUD = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(new PirateSimulator::UIHUD());

            // Initialisation des objets 3D - création et/ou chargement 
            if(!InitObjets()) return 1;

            return 0;
        }

        bool InitObjets()
        {
            CChargeurAssimp chargeur;

            /************************************************************************/
            /* Init Boat                                                            */
            /************************************************************************/

            PirateSimulator::Transform transformBoat({ 950,0,900,0 }, { 0,0,-1,0 }, { 0,1,0,0 }, { 1,0,0,0 });
            PirateSimulator::GameObjectRef vehicule = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
                new PirateSimulator::GameObject(transformBoat, "vehicule")
            );

            // Cr�ation du mesh du boat � partir d'un fichier .OBJ
            CParametresChargement paramBoat(".\\modeles\\Boat\\", "boat.obj", false, false);
            chargeur.Chargement(paramBoat);

            auto vehiculeMesh = new CObjetMesh(ShaderCObjectMesh::ShadersParams(), L"MiniPhong.fx", chargeur);

            vehicule->addComponent<PirateSimulator::IMesh>(vehiculeMesh);
            vehicule->addComponent<PirateSimulator::IBehaviour>(new PirateSimulator::PlayerBehaviour());
            auto vehicleShape = new PirateSimulator::VehicleShape();
            vehicule->addComponent<PirateSimulator::ShapeComponent>(vehicleShape);


            /************************************************************************/
            /* Init Tunnel                                                          */
            /************************************************************************/

            PirateSimulator::Transform transformTunnel({ 150,0,1000,0 }, { 0,0,-1,0 }, { 0,1,0,0 }, { 1,0,0,0 });
            PirateSimulator::GameObjectRef tunnel = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
                new PirateSimulator::GameObject(transformTunnel, "tunnel")
            );

            // Cr�ation du mesh du tunnel � partir d'un fichier .OBJ
            CParametresChargement paramTunnel(".\\modeles\\Tunnel\\", "tunnel.obj", false, true);
            chargeur.Chargement(paramTunnel);

            auto tunnelMesh = new PM3D::CObjetMesh(ShaderCObjectMesh::ShadersParams(), L"MiniPhongField.fx", chargeur);
            tunnelMesh->setBackFaceCulling(false);

            tunnel->addComponent<PirateSimulator::IMesh>(tunnelMesh);


            /************************************************************************/
            /* Init Terrain                                                         */
            /************************************************************************/

            PirateSimulator::Transform TransformTerrain({ 0,0,0,0 }, { 0,0,-1,0 }, { 0,1,0,0 }, { 1,0,0,0 });

            // Add our terrain
            PirateSimulator::GameObjectRef terrain = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
                new PirateSimulator::GameObject(TransformTerrain, "terrain")
            );
#ifdef DEBUG_TEST_TERRAIN        
            // If we want to test our own terrain and not the one int the config file

            int terrainH = 257;
            int terrainW = 257;
            int terrainScale = 4;

            auto fieldMesh = new PirateSimulator::Terrain(terrainH, terrainW, terrainScale, "PirateSimulator/heightmapOutput.txt", "PirateSimulator/textureTerrain.dds");
#else
            // Get all the information from the config file
            auto fieldMesh = new PirateSimulator::Terrain();
#endif
            terrain->addComponent<PirateSimulator::IMesh>(fieldMesh);
            // Add the shape for Terrain
            auto terrainShape = new PirateSimulator::TerrainShape();
            //terrain->addComponent<PirateSimulator::ShapeComponent>(terrainShape);

            // Add our water plane
            auto water = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
                new PirateSimulator::GameObject(TransformTerrain, "water")
            );
            PirateSimulator::Plane* waterMesh = new PirateSimulator::Plane("PirateSimulator/water.dds");
            water->addComponent<PirateSimulator::IMesh>(waterMesh);

            // Set the gameobject which is paired to the camera
            if (PirateSimulator::CameraManager::singleton.getCameraType() == PirateSimulator::cameraModule::BaseCamera::OBJECT_CAMERA)
            {
                PirateSimulator::CameraManager::singleton.setPairedTarget(vehicule);
            }
            else if (PirateSimulator::CameraManager::singleton.getCameraType() == PirateSimulator::cameraModule::BaseCamera::LEVEL_CAMERA)
            {
                PirateSimulator::CameraManager::singleton.setPairedTarget(terrain);
            }


            /************************************************************************/
            /* Render Mesh                                                          */
            /************************************************************************/

            // Puis, il est ajouté à la scène
            PirateSimulator::RendererManager::singleton.addAnObligatoryMeshToDrawBefore(fieldMesh);
            PirateSimulator::RendererManager::singleton.addAnObligatoryMeshToDrawBefore(waterMesh);
            PirateSimulator::RendererManager::singleton.addAMovingSortableMesh(vehiculeMesh);
            PirateSimulator::RendererManager::singleton.addAStaticSortableMesh(tunnelMesh);

            return true;
        }


    protected:
        PirateSimulator::GameObjectRef m_skybox;

        // Le gestionnaire de texture
        CGestionnaireDeTextures TexturesManager;

        // Pour le texte
        CAfficheurTexte* pTexte1;
        wstring str;

        Gdiplus::Font* pPolice;
    };


} // namespace PM3D

