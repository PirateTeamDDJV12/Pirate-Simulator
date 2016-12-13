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

#include "../../PirateSimulator/GameConfig.h"
#include "../../PirateSimulator/BlocMesh.h"
#include "../../PirateSimulator/Piece.h"
#include "../../PirateSimulator/GameFabric.h"

// Manager
#include "../../PirateSimulator/TimeManager.h"
#include "../../PirateSimulator/GameObjectManager.h"
#include "../../PirateSimulator/RendererManager.h"
#include "../../PirateSimulator/InputManager.h"
#include "../../PirateSimulator/TaskManager.h"

// Tasks
#include "../../PirateSimulator/TimeTask.h"
#include "../../PirateSimulator/InputTask.h"
#include "../../PirateSimulator/PhysicsTask.h"
#include "../../PirateSimulator/RenderTask.h"
#include "../../PirateSimulator/PlayerTask.h"
#include "../../PirateSimulator/SoundTask.h"

//UI
#include "../../PirateSimulator/UIElement.h"
#include "../../PirateSimulator/UIMenu.h"
#include "../../PirateSimulator/UIHUD.h"

#include <thread>
#include <vector>




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
            SOUNDTASK,
        };


    public:

        virtual void Run()
        {
            bool bBoucle = true;

            while(bBoucle)
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

            taskManager->addTask<PirateSimulator::TimeTask>(TIMETASK);
            taskManager->addTask<PirateSimulator::InputTask>(INPUTTASK);
            taskManager->addTask<PirateSimulator::RenderTask>(RENDERTASK);
            taskManager->addTask<PirateSimulator::PhysicsTask>(PHYSICSTASK);
            taskManager->addTask<PirateSimulator::PlayerTask>(PLAYERTASK);
            taskManager->addTask<PirateSimulator::SoundTask>(SOUNDTASK);
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
            using PirateSimulator::GameFabric;

            
            PirateSimulator::RendererManager::singleton.setSortingMesh(true);
            PirateSimulator::RendererManager::singleton.setDetailLevel(PirateSimulator::RendererManager::DEEP_ARRANGEMENT);

            // Initialisation des matrices View et Proj
            // Dans notre cas, ces matrices sont fixes
            PirateSimulator::Transform cameraTransform = PirateSimulator::Transform();
            cameraTransform.setPosition(XMVectorSet(0.f, 0.f, -10.f, 0.f));
            cameraTransform.setUp(XMVectorSet(0.f, 1.f, 0.f, 0.f));
            cameraTransform.setForward(XMVectorSet(0.f, 0.f, 1.f, 0.f));


            // create camera
            GameFabric::createCamera(cameraTransform);
            
            // Skybox
            GameFabric::createSkybox();

            auto HUD = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(new PirateSimulator::UIHUD());

            // Initialisation des objets 3D - création et/ou chargement 
            if(!InitObjets()) return 1;

            return 0;
        }

        bool InitObjets()
        {
            using PirateSimulator::GameFabric;
            using PirateSimulator::Transform;

            // TODO - Get this with a config

            Transform transformBoat;

            transformBoat.setPosition(950.0f, 0.0f, 900.0f);
            transformBoat.setUp({0.0f, 1.0f, 0.0f, 0.0f});
            transformBoat.setForward({0.0f, 0.0f, -1.0f, 0.0f});

            
            Transform TransformTerrain;

            TransformTerrain.setPosition(0.0f, 0.0f, 0.0f);
            TransformTerrain.setUp({0.0f, 1.0f, 0.0f, 0.0f});
            TransformTerrain.setForward({0.0f, 0.0f, -1.0f, 0.0f});


            // Create our boat
            GameFabric::createBoat(transformBoat);

            // Add our water plane
            GameFabric::createWater(TransformTerrain);

            // Add our terrain
            GameFabric::createField(TransformTerrain);

            return true;
        }


    protected:

        // Le gestionnaire de texture
        CGestionnaireDeTextures TexturesManager;

        // Pour le texte
        CAfficheurTexte* pTexte1;
        wstring str;

        Gdiplus::Font* pPolice;
    };


} // namespace PM3D

