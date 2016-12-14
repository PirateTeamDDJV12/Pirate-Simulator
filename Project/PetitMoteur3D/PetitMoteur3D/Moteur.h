#pragma once
#include "Singleton.h"
#include "dispositif.h" 

#include "GestionnaireDeTextures.h"
#include "AfficheurSprite.h"
#include "AfficheurTexte.h"

#include "../../PirateSimulator/GameConfig.h"
#include "../../PirateSimulator/GameFabric.h"
#include "../../PirateSimulator/GameLogic.h"

// Manager
#include "../../PirateSimulator/TimeManager.h"
#include "../../PirateSimulator/GameObjectManager.h"
#include "../../PirateSimulator/RendererManager.h"
#include "../../PirateSimulator/InputManager.h"
#include "../../PirateSimulator/TaskManager.h"

//UI
#include "../../PirateSimulator/UIElement.h"
#include "../../PirateSimulator/UIMenu.h"

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
            PirateSimulator::GameLogic::createAllTask();

            bool resultInit = false;

            std::vector<std::thread> beginThread;


            beginThread.emplace_back([this, &resultInit]() {
                // * Initialisation de la scène
                InitScene();
                resultInit = true;
            });

            //for(size_t iter = 0; iter < beginThread.size(); ++iter)
            //{
            //    beginThread[iter].join();
            //}

            PirateSimulator::UIMainMenuLogic mainMenu;

            while(true)
            {
                // Update the window
                if(!RunSpecific())
                {
                    // Kill all thread if the user closes the window
                    beginThread.front().detach();
                    return 1;
                }
                // Get the Input
                PirateSimulator::InputManager::singleton.update();
                // Render
                auto pDispositif = PirateSimulator::RendererManager::singleton.getDispositif();
                ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();
                ID3D11RenderTargetView* pRenderTargetView = pDispositif->GetRenderTargetView();

                float Couleur[4] = {0.0f, 0.5f, 0.0f, 1.0f};  //  RGBA - Vert pour le moment
                pImmediateContext->ClearRenderTargetView(pRenderTargetView, Couleur);

                ID3D11DepthStencilView* pDepthStencilView = pDispositif->GetDepthStencilView();
                pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
                // Display the mainMenu
                if((mainMenu() && resultInit))
                    break;
                pDispositif->Present();

                std::this_thread::sleep_for(5ms);
            }

            beginThread.front().detach();

            // Start the game time when all menu and loading screen are close to begin the game
            TimeManager::GetInstance().startGameTime();

            return 0;
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
            // Initialisation des objets 3D - création et/ou chargement 
            if(!InitObjets()) return 1;

            return 0;
        }

        bool InitObjets()
        {
            using PirateSimulator::GameFabric;
            using PirateSimulator::Transform;

            // Initialisation des matrices View et Proj
            // Dans notre cas, ces matrices sont fixes
            Transform cameraTransform;

            cameraTransform.setPosition({ 0.f, 0.f, -10.f, 0.f }); //XMVectorSet before
            cameraTransform.setUp({ 0.f, 1.f, 0.f, 0.f }); //XMVectorSet before
            cameraTransform.setForward({ 0.f, 0.f, 1.f, 0.f }); //XMVectorSet before


            Transform transformBoat;

            transformBoat.setPosition(950.0f, 0.0f, 900.0f);
            transformBoat.setUp({0.0f, 1.0f, 0.0f, 0.0f});
            transformBoat.setForward({0.0f, 0.0f, -1.0f, 0.0f});

            
            Transform TransformTerrain;

            TransformTerrain.setPosition(0.0f, 0.0f, 0.0f);
            TransformTerrain.setUp({0.0f, 1.0f, 0.0f, 0.0f});
            TransformTerrain.setForward({0.0f, 0.0f, -1.0f, 0.0f});



            Transform transformTunnel;

            transformTunnel.setPosition(150.f, 0.f, 1000.f );
            transformTunnel.setUp({ 0.0f, 1.0f, 0.0f, 0.0f });
            transformTunnel.setForward({ 0.0f, 0.0f, -1.0f, 0.0f }); 

            

            GameFabric gameFabric;

            // Camera
            gameFabric.createCamera(cameraTransform);

            // Skybox
            gameFabric.createSkybox();

            // HUD
            gameFabric.createHUD();

            // Create our boat
            gameFabric.createBoat(transformBoat);

            // Add our water plane
            gameFabric.createWater(TransformTerrain);

            // Add our terrain
            gameFabric.createField(TransformTerrain);

            // Add Tunnel
            gameFabric.createTunnel(transformTunnel);


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

