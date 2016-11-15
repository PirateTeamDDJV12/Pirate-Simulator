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
#include "../../PirateSimulator/RessourceManager.h"
#include "../../PirateSimulator/LevelCameraBehaviour.h"
#include "../../PirateSimulator/FreeCameraBehaviour.h"
#include "../../PirateSimulator/ObjectCameraBehaviour.h"
#include "../../PirateSimulator/GameObject.h"
#include "../../PirateSimulator/TestBehaviour.h"
#include "../../PirateSimulator/VehicleBehaviour.h"

#include "../../PirateSimulator/TimeManager.h"

#include "../../PirateSimulator/GameObjectManager.h"
#include "../../PirateSimulator/RendererManager.h"
#include "../../PirateSimulator/CameraManager.h"
#include "../../PirateSimulator/BlocMesh.h"


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

                // appeler la fonction d'animation
                if (bBoucle) bBoucle = Animation();
            }
        }

        virtual int Initialisations()
        {
            // Initialisation du temps de jeu
            TimeManager::get()->startGameTime();

            // Propre à la plateforme
            InitialisationsSpecific();

            // * Initialisation du dispositif de rendu
            pDispositif = CreationDispositifSpecific(CDS_FENETRE);

            // * Initialisation de la scène
            InitScene();

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
                pDispositif->Present();

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

            // Détruire le dispositif
            if (pDispositif)
            {
                delete pDispositif;
                pDispositif = NULL;
            }
        }

        virtual int InitScene()
        {
            auto camProjParameters = PirateSimulator::cameraModule::CameraProjectionParameters(
                XM_PI / 4,
                PirateSimulator::GameGlobals::CameraGlobals::NEAREST_PLANE,
                PirateSimulator::GameGlobals::CameraGlobals::FARTHEST_PLANE,
                pDispositif->GetLargeur(), 
                pDispositif->GetHauteur()
            );

            auto camMovParameters = PirateSimulator::cameraModule::CameraMovingParameters(
                PirateSimulator::GameGlobals::CameraGlobals::LINEAR_SPEED, 
                PirateSimulator::GameGlobals::CameraGlobals::ANGULAR_SPEED);

            PirateSimulator::RendererManager::singleton.setSortingMesh(true);
            PirateSimulator::RendererManager::singleton.setDetailLevel(PirateSimulator::RendererManager::DEEP_ARRANGEMENT);
            
            // Initialisation des matrices View et Proj
            // Dans notre cas, ces matrices sont fixes

            m_camera = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
                createCamera(PirateSimulator::cameraModule::BaseCamera::type::OBJECT_CAMERA, camProjParameters, camMovParameters, "mainCamera")
            );

            // Skybox
            PirateSimulator::CSkybox* skyBoxMesh = new PirateSimulator::CSkybox(pDispositif);
            m_skybox = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
                new PirateSimulator::GameObject(m_camera->m_transform, "skybox")
            );

            m_skybox->addComponent<PirateSimulator::IMesh>(skyBoxMesh);

            skyBoxMesh->SetTexture(new CTexture(L"PirateSimulator/skybox.dds", pDispositif));
            

            PirateSimulator::RendererManager::singleton.addAnObligatoryMeshToDrawBefore(skyBoxMesh);

            // Initialisation des objets 3D - création et/ou chargement
            if (!InitObjets()) return 1;

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

            //CObjetMesh().ConvertToOMB(".\\modeles\\Boat\\boat.obj", ".\\modeles\\Boat\\boat.OMB");
            /*CParametresChargement param;
            param.bInverserCulling = false;
            param.bMainGauche = true;
            param.NomChemin = ".\\modeles\\Boat\\";
            param.NomFichier = "boat.obj";
            CChargeurOBJ chargeur;
            chargeur.Chargement(param);
            vehicule->addComponent<PirateSimulator::IMesh>(new CObjetMesh(".\\modeles\\Boat\\boat.OMB", ShaderCObjectMesh::ShadersParams(), chargeur, pDispositif));*/

            vehicule->addComponent<PirateSimulator::IMesh>(new CObjetMesh(".\\modeles\\Boat\\boat.OMB", ShaderCObjectMesh::ShadersParams(), pDispositif));
            vehicule->addComponent<PirateSimulator::IBehaviour>(new PirateSimulator::VehicleBehaviour());
            
            auto personnage = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
                new PirateSimulator::GameObject(transform, "personnage")
            );

            auto personageMesh = new CObjetMesh(".\\modeles\\jin\\jin.OMB", ShaderCObjectMesh::ShadersParams(), pDispositif);
            personnage->addComponent<PirateSimulator::IMesh>(personageMesh);
            personnage->addComponent<PirateSimulator::IBehaviour>(new PirateSimulator::TestBehaviour());



            int terrainH = 257;
            int terrainW = 257;

            PirateSimulator::GameObjectRef terrain = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
                new PirateSimulator::GameObject(transform, "terrain")
            );
            
            auto fieldMesh = new PirateSimulator::Terrain(pDispositif, terrainH, terrainW, "PirateSimulator/heightmapOutput.txt", "PirateSimulator/textureTerrain.dds");

            terrain->addComponent<PirateSimulator::IMesh>(fieldMesh);


            PirateSimulator::cameraModule::BaseCamera* baseCam = m_camera->getComponent<PirateSimulator::cameraModule::BaseCamera>();

            if (baseCam->getCameraType() == PirateSimulator::cameraModule::BaseCamera::OBJECT_CAMERA)
            {
                baseCam->setTarget(vehicule);
            }
            else if (baseCam->getCameraType() == PirateSimulator::cameraModule::BaseCamera::LEVEL_CAMERA)
            {
                m_camera->getComponent<PirateSimulator::cameraModule::LevelCameraBehaviour>()->setTerrain(terrain);
            }

            PirateSimulator::CameraManager::singleton.setMainCamera(m_camera);

            // Puis, il est ajouté à la scène
            PirateSimulator::RendererManager::singleton.addAnObligatoryMeshToDrawBefore(fieldMesh);
            PirateSimulator::RendererManager::singleton.addAStaticSortableMesh(personageMesh);


            PirateSimulator::Transform transformCube{};
            transformCube.m_position.vector4_f32[0] = 257.f;
            transformCube.m_position.vector4_f32[1] = 8.f;
            transformCube.m_position.vector4_f32[2] = 257.f;

            auto cube = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
                new PirateSimulator::GameObject(
                    transformCube, "Cube"
                )
            );

            cube->addComponent<PirateSimulator::IMesh>(
                new PirateSimulator::BlocMesh<PirateSimulator::BlocStructure>(
                    PirateSimulator::BlocStructure(transformCube, 2,2,2), pDispositif, PirateSimulator::ShaderBloc::ShadersParams()
                )
            );

            PirateSimulator::RendererManager::singleton.addAStaticSortableMesh(cube->getComponent<PirateSimulator::IMesh>());

            return true;
        }

        bool AnimeScene(float tempsEcoule)
        {

            // Prendre en note le statut du clavier
            GestionnaireDeSaisie.StatutClavier();

            // Prendre en note l'état de la souris
            GestionnaireDeSaisie.SaisirEtatSouris();

            m_camera->anime(0);

            PirateSimulator::GameObjectManager::singleton.animAllGameObject(tempsEcoule);

            PirateSimulator::RendererManager::singleton.update();

            return true;
        }

        PirateSimulator::GameObject* createCamera(PirateSimulator::cameraModule::BaseCamera::type cameraType,
            const PirateSimulator::cameraModule::CameraProjectionParameters &camProjParameters,
            const PirateSimulator::cameraModule::CameraMovingParameters &camMovParameters,
            const std::string& name)
        {
            PirateSimulator::GameObject* camera;
            PirateSimulator::Transform cameraInitialPosition;

            cameraInitialPosition.m_position = XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f);
            cameraInitialPosition.m_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
            cameraInitialPosition.m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

            camera = new PirateSimulator::GameObject(cameraInitialPosition, name);


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
                delete camera;
                return nullptr;
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

        // Le dispositif de rendu
        TClasseDispositif* pDispositif;


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

