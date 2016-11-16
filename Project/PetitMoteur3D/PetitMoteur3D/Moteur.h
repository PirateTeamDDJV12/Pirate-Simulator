#pragma once
#include "Singleton.h"
#include "dispositif.h" 

#include <vector>

#include "Bloc.h"
#include "BlocEffet1.h"
#include "ObjetMesh.h"
#include "ChargeurOBJ.h"
#include "GestionnaireDeTextures.h"
#include "AfficheurSprite.h"
#include "AfficheurTexte.h"
#include "DIManipulateur.h"

#include "../../PirateSimulator/GameConfig.h"
#include "../../PirateSimulator/Mesh.h"
#include "../../PirateSimulator/Skybox.h"
#include "../../PirateSimulator/Terrain.h"
#include "../../PirateSimulator/LevelCameraBehaviour.h"
#include "../../PirateSimulator/FreeCameraBehaviour.h"
#include "../../PirateSimulator/ObjectCameraBehaviour.h"
#include "../../PirateSimulator/GameObject.h"
#include "../../PirateSimulator/PlayerBehaviour.h"

// Manager
#include "../../PirateSimulator/TimeManager.h"
#include "../../PirateSimulator/GameObjectManager.h"
#include "../../PirateSimulator/RendererManager.h"
#include "../../PirateSimulator/CameraManager.h"
#include "../../PirateSimulator/TaskManager.h"

// Tasks
#include "../../PirateSimulator/TimeTask.h"
#include "../../PirateSimulator/InputTask.h"
#include "../../PirateSimulator/PhysicsTask.h"
#include "../../PirateSimulator/RenderTask.h"


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
        enum
        {
            TIMETASK,
            INPUTTASK,
            PHYSICSTASK,
            RENDERTASK,
        };
    public:

        virtual void Run()
        {
            // TODO - Changer la boucle de jeu pour faire l'update du TaskManager (qui s'occupera de faire l'update des Tasks dans le bonne ordre)
            bool bBoucle = true;

            while (bBoucle)
            {
                // Propre à la plateforme - (Conditions d'arrêt, interface, messages)
                bBoucle = RunSpecific();

                if (bBoucle) bBoucle = Animation();
            }
        }

        virtual int Initialisations()
        {
            // Création des tasks
            CreateTasks();
            
            // Propre à la plateforme
            InitialisationsSpecific();

            // TODO - Deplacer cela dans le rendererManager pour le rendre dispo pour tous et faire l'init dans l'init du renderTask
            // * Initialisation du dispositif de rendu
            PirateSimulator::RendererManager::singleton.setDispositif(CreationDispositifSpecific(CDS_FENETRE));

            // * Initialisation de la scène
            InitScene();

            // TODO - Ne pas faire cela ici, le TimeManager va s'occuper de préparer le temps pour les animes et le premier rendu ne doit pas se faire tout de suite, il faut le faire dans le premier tour de boucle du jeu !!
            // * Initialisation des paramètres de l'animation et 
            //   préparation de la première image
            InitAnimation();

            return 0;
        }

        virtual bool Animation()
        {
            __int64 TempsCourant;
            float TempsEcoule;

            // méthode pour lire l'heure et calculer le 
            // temps écoulé
            TempsCourant = GetTimeSpecific();

            // Est-il temps de rendre l'image?
            if (TempsCourant > TempsSuivant)
            {
                // Affichage optimisé 
                PirateSimulator::RendererManager::singleton.getDispositif()->Present();

                TempsEcoule = static_cast<float>(TempsCourant - TempsPrecedent)
                    * static_cast<float>(EchelleTemps);

                // On prépare la prochaine image
                AnimeScene(TempsEcoule);

                // On rend l'image sur la surface de travail 
                // (tampon d'arrière plan)
                RenderScene();

                // Calcul du temps du prochain affichage
                TempsPrecedent = TempsCourant;
                TempsSuivant = TempsCourant + EcartTemps;
            }

            return true;
        }

        void CreateTasks()
        {
            PirateSimulator::TaskManager* taskManager = &PirateSimulator::TaskManager::GetInstance();

            taskManager->addTask<PirateSimulator::TimeTask>(TIMETASK);
            taskManager->addTask<PirateSimulator::InputTask>(INPUTTASK);
            taskManager->addTask<PirateSimulator::PhysicsTask>(PHYSICSTASK);
            taskManager->addTask<PirateSimulator::RenderTask>(RENDERTASK);
        }

        XMMATRIX GetMatView()
        {
            return m_camera->getComponent<PirateSimulator::cameraModule::BaseCamera>()->getViewMatrix();
        }
        XMMATRIX GetMatProj()
        {
            return m_camera->getComponent<PirateSimulator::cameraModule::BaseCamera>()->getProjMatrix();
        }
        XMMATRIX GetMatViewProj()
        {
            return m_camera->getComponent<PirateSimulator::cameraModule::BaseCamera>()->getViewProjMatrix();
        }

        CGestionnaireDeTextures& GetTextureManager()
        {
            return TexturesManager;
        }
        CDIManipulateur& GetGestionnaireDeSaisie()
        {
            return GestionnaireDeSaisie;
        }

        PirateSimulator::GameObjectRef getCamera()
        {
            return m_camera;
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
        virtual TClasseDispositif* CreationDispositifSpecific(const CDS_MODE cdsMode) = 0;
        virtual void BeginRenderSceneSpecific() = 0;
        virtual void EndRenderSceneSpecific() = 0;

        // Autres fonctions
        virtual int InitAnimation()
        {
            TempsSuivant = GetTimeSpecific();
            EchelleTemps = 0.001;
            EcartTemps = 1000 / IMAGESPARSECONDE;
            TempsPrecedent = TempsSuivant;

            // première Image
            RenderScene();

            return true;
        }

        // Fonctions de rendu et de présentation de la scène
        virtual bool RenderScene()
        {
            BeginRenderSceneSpecific();

            // Appeler les fonctions de dessin de chaque objet de la scène
            PirateSimulator::RendererManager::singleton.draw();

            EndRenderSceneSpecific();
            return true;
        }


        virtual void Cleanup()
        {

            // Vider les textures
            TexturesManager.Cleanup();

            // détruire les objets
            /*std::vector<PirateSimulator::GameObject*>::iterator It;

            for (It = ListeScene.begin(); It != ListeScene.end(); It++)
            {
                delete *It;
            }

            ListeScene.clear();*/
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

            m_camera = createCamera(PirateSimulator::cameraModule::BaseCamera::type::OBJECT_CAMERA, camProjParameters, camMovParameters, "mainCamera");

            // Skybox
            PirateSimulator::CSkybox* skyBoxMesh = new PirateSimulator::CSkybox();
            m_skybox = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
                new PirateSimulator::GameObject(m_camera->m_transform, "skybox")
            );

            m_skybox->addComponent<PirateSimulator::IMesh>(skyBoxMesh);

            skyBoxMesh->SetTexture(new CTexture(L"PirateSimulator/skybox.dds"));


            PirateSimulator::RendererManager::singleton.addAnObligatoryMeshToDrawBefore(skyBoxMesh);

            // Initialisation des objets 3D - création et/ou chargement
            if(!InitObjets()) return 1;

            return 0;
        }

        bool InitObjets()
        {
            /*
            * Init the terrain
            */
            // TODO - Get this with a config

            PirateSimulator::Transform transform;

            transform.m_position = { 0,0,0,0 };
            transform.m_right = { 1,0,0,0 };
            transform.m_up = { 0,1,0,0 };
            transform.m_forward = { 0,0,-1,0 };


            // Constructeur avec format binaire
            PirateSimulator::GameObjectRef vehicule = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
                new PirateSimulator::GameObject(transform, "vehicule")
            );

            /*CObjetMesh().ConvertToOMB(".\\modeles\\Boat\\boat.obj", ".\\modeles\\Boat\\boat.OMB");*/
            /*CParametresChargement param;
            param.bInverserCulling = false;
            param.bMainGauche = true;
            param.NomChemin = ".\\modeles\\Boat\\";
            param.NomFichier = "boat.obj";
            CChargeurOBJ chargeur;
            chargeur.Chargement(param);
            vehicule->addComponent<PirateSimulator::IMesh>(new CObjetMesh(".\\modeles\\Boat\\boat.OMB", ShaderCObjectMesh::ShadersParams(), chargeur));*/

            auto vehiculeMesh = new CObjetMesh(".\\modeles\\Boat\\boat.OMB", ShaderCObjectMesh::ShadersParams());
            vehicule->addComponent<PirateSimulator::IMesh>(vehiculeMesh);
            vehicule->addComponent<PirateSimulator::IBehaviour>(new PirateSimulator::PlayerBehaviour());

            /*          auto personnage = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
            new PirateSimulator::GameObject(transform, "personnage")
            );
            auto personageMesh = new CObjetMesh(".\\modeles\\jin\\jin.OMB", ShaderCObjectMesh::ShadersParams());
            personnage->addComponent<PirateSimulator::IMesh>(personageMesh);
            personnage->addComponent<PirateSimulator::IBehaviour>(new PirateSimulator::TestBehaviour());*/

            PirateSimulator::GameObjectRef terrain = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
                new PirateSimulator::GameObject(transform, "terrain")
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


            PirateSimulator::cameraModule::BaseCamera* baseCam = m_camera->getComponent<PirateSimulator::cameraModule::BaseCamera>();

            if (baseCam->getCameraType() == PirateSimulator::cameraModule::BaseCamera::OBJECT_CAMERA)
            {
                m_camera->getComponent<PirateSimulator::cameraModule::ObjectCameraBehaviour>()->setTarget(vehicule);
            }
            else if (baseCam->getCameraType() == PirateSimulator::cameraModule::BaseCamera::LEVEL_CAMERA)
            {
                m_camera->getComponent<PirateSimulator::cameraModule::LevelCameraBehaviour>()->setTerrain(terrain);
            }

            PirateSimulator::CameraManager::singleton.setMainCamera(m_camera);

            // Puis, il est ajouté à la scène
            PirateSimulator::RendererManager::singleton.addAnObligatoryMeshToDrawBefore(fieldMesh);
            PirateSimulator::RendererManager::singleton.addAMovingSortableMesh(vehiculeMesh);
            //PirateSimulator::RendererManager::singleton.addAStaticSortableMesh(personageMesh);


            return true;
        }

        bool AnimeScene(float tempsEcoule)
        {

            // Prendre en note le statut du clavier
            GestionnaireDeSaisie.StatutClavier();

            // Prendre en note l'état de la souris
            GestionnaireDeSaisie.SaisirEtatSouris();


            PirateSimulator::GameObjectManager::singleton.animAllGameObject(tempsEcoule);

            PirateSimulator::RendererManager::singleton.update();
            m_camera->anime(0);

            return true;
        }

        PirateSimulator::GameObjectRef createCamera(PirateSimulator::cameraModule::BaseCamera::type cameraType,
            const PirateSimulator::cameraModule::CameraProjectionParameters &camProjParameters,
            const PirateSimulator::cameraModule::CameraMovingParameters &camMovParameters,
            const std::string& name)
        {
            PirateSimulator::GameObjectRef camera;
            PirateSimulator::Transform cameraInitialPosition;

            cameraInitialPosition.m_position = XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f);
            cameraInitialPosition.m_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
            cameraInitialPosition.m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

            camera = PirateSimulator::GameObjectRef(new PirateSimulator::GameObject(cameraInitialPosition, name));


            camera->addComponent<PirateSimulator::cameraModule::BaseCamera>(
                new PirateSimulator::cameraModule::BaseCamera(
                    camProjParameters, camMovParameters
                )
                );

            switch (cameraType)
            {
            case PirateSimulator::cameraModule::BaseCamera::FREE_CAMERA:
                camera->addComponent<PirateSimulator::IBehaviour>(new PirateSimulator::cameraModule::FreeCameraBehaviour());
                break;

            case PirateSimulator::cameraModule::BaseCamera::LEVEL_CAMERA:
                camera->addComponent<PirateSimulator::IBehaviour>(new PirateSimulator::cameraModule::LevelCameraBehaviour());
                break;

            case PirateSimulator::cameraModule::BaseCamera::OBJECT_CAMERA:
                camera->addComponent<PirateSimulator::IBehaviour>(new PirateSimulator::cameraModule::ObjectCameraBehaviour());
                break;

            default:
                return PirateSimulator::GameObjectRef();
            }

            camera->getComponent<PirateSimulator::cameraModule::BaseCamera>()->setCameraType(cameraType);

            return camera;
        }


    protected:
        // Variables pour le temps de l'animation
        __int64 TempsSuivant;
        __int64 TempsPrecedent;
        unsigned long EcartTemps;
        double EchelleTemps;

        PirateSimulator::GameObjectRef m_camera;
        PirateSimulator::GameObjectRef m_skybox;

        // Le gestionnaire de texture
        CGestionnaireDeTextures TexturesManager;

        // Pour le texte
        CAfficheurTexte* pTexte1;
        wstring str;

        Gdiplus::Font* pPolice;

        CDIManipulateur GestionnaireDeSaisie;

    };


} // namespace PM3D

