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

#include "../../PirateSimulator/GameObjectManager.h"
#include "../../PirateSimulator/RendererManager.h"
#include "../../PirateSimulator/CameraManager.h"


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

            while(bBoucle)
            {
                // Propre à la plateforme - (Conditions d'arrêt, interface, messages)
                bBoucle = RunSpecific();

                // appeler la fonction d'animation
                if(bBoucle) bBoucle = Animation();
            }
        }

        virtual int Initialisations()
        {
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
            if(TempsCourant > TempsSuivant)
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

            /*for(auto It = ListeScene.begin(); It != ListeScene.end(); It++)
            {
                (*It)->draw();
            }*/

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

            for(It = ListeScene.begin(); It != ListeScene.end(); It++)
            {
                delete *It;
            }

            ListeScene.clear();*/

            // Détruire le dispositif
            if(pDispositif)
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

            

            // Initialisation des matrices View et Proj
            // Dans notre cas, ces matrices sont fixes

            m_camera = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
                createCamera(PirateSimulator::cameraModule::BaseCamera::type::LEVEL_CAMERA, camProjParameters, camMovParameters),
                "mainCamera"
            );
            
            
            

            // Skybox
            PirateSimulator::CSkybox* skyBoxMesh = new PirateSimulator::CSkybox(pDispositif);
            m_skybox = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
                new PirateSimulator::GameObject(m_camera->m_transform),
                "skybox");

            m_skybox->addComponent<PirateSimulator::IMesh>(skyBoxMesh);

            skyBoxMesh->SetTexture(new CTexture(L"PirateSimulator/skybox.dds", pDispositif));
            //ListeScene.push_back(m_skybox);

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
            
            /*PirateSimulator::GameObject *terrain;
            CObjetMesh* pMesh;
            CAfficheurSprite* pAfficheurSprite;*/

            PirateSimulator::Transform transform;

            transform.m_position = { 0,0,0,0 };
            transform.m_right = { 1,0,0,0 };
            transform.m_up = { 0,1,0,0 };
            transform.m_forward = { 0,0,-1,0 };


            // Constructeur avec format binaire
            //pMesh = new CObjetMesh(".\\modeles\\jin\\jin.OMB", pDispositif);
            auto personnage = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
                new PirateSimulator::GameObject(transform),
                "personnage");

            auto personageMesh = new CObjetMesh(".\\modeles\\jin\\jin.OMB", ShaderCObjectMesh::ShadersParams(), pDispositif);
            personnage->addComponent<PirateSimulator::IMesh>(personageMesh);
            personnage->addComponent<PirateSimulator::IBehaviour>(new PirateSimulator::TestBehaviour());


            int terrainH = 257;
            int terrainW = 257;

            PirateSimulator::GameObjectRef terrain = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
                new PirateSimulator::GameObject(transform),
                "terrain");
            
            auto fieldMesh = new PirateSimulator::Terrain(pDispositif, terrainH, terrainW, "PirateSimulator/heightmapOutput.txt", "PirateSimulator/textureTerrain.dds");

            terrain->addComponent<PirateSimulator::IMesh>(fieldMesh);

            
            PirateSimulator::cameraModule::BaseCamera* baseCam = m_camera->getComponent<PirateSimulator::cameraModule::BaseCamera>();

            if (baseCam->getCameraType() == PirateSimulator::cameraModule::BaseCamera::OBJECT_CAMERA)
            {
                baseCam->setTarget(personnage);
            }
            else if (baseCam->getCameraType() == PirateSimulator::cameraModule::BaseCamera::LEVEL_CAMERA)
            {
                m_camera->getComponent<PirateSimulator::cameraModule::LevelCameraBehaviour>()->setTerrain(terrain);
            }

            PirateSimulator::CameraManager::singleton.setMainCamera(m_camera);

            // Puis, il est ajouté à la scène
            PirateSimulator::RendererManager::singleton.addAnObligatoryMeshToDrawBefore(fieldMesh);
            PirateSimulator::RendererManager::singleton.addAStaticSortableMesh(personageMesh);
            //ListeScene.push_back(personnage);
            //ListeScene.push_back(terrain);

            //// Création de l'afficheur de sprites et ajout des sprites
            //pAfficheurSprite = new CAfficheurSprite(pDispositif);

            //// ajout de panneaux 
            //pAfficheurSprite->AjouterPanneau("grass_v1_basic_tex.dds",
            //                                 XMFLOAT3(1.0f, 0.0f, 1.0f));
            //pAfficheurSprite->AjouterPanneau("grass_v1_basic_tex.dds",
            //                                 XMFLOAT3(0.0f, 0.0f, -1.0f));
            //pAfficheurSprite->AjouterPanneau("grass_v1_basic_tex.dds",
            //                                 XMFLOAT3(-1.0f, 0.0f, 0.5f));
            //pAfficheurSprite->AjouterPanneau("grass_v1_basic_tex.dds",
            //                                 XMFLOAT3(-0.5f, 0.0f, 1.0f));
            //pAfficheurSprite->AjouterPanneau("grass_v1_basic_tex.dds",
            //                                 XMFLOAT3(-2.0f, 0.0f, 2.0f));

            


            //pAfficheurSprite->AjouterSprite("tree02s.dds", 200,400);
            //pAfficheurSprite->AjouterSprite("tree02s.dds", 500,500, 100, 100);
            //pAfficheurSprite->AjouterSprite("tree02s.dds", 800,200, 100, 100);

            //CAfficheurTexte::Init();
            //const FontFamily oFamily( L"Arial", NULL );
            //pPolice = new Font(&oFamily, 16.00, FontStyleBold, UnitPixel );
            //pTexte1 = new CAfficheurTexte(pDispositif, 256, 256, pPolice);

            //pAfficheurSprite->AjouterSpriteTexte(pTexte1->GetTextureView(), 0, 257);

            //pTexte1->Ecrire(L"Test du texte");

            //// Puis, il est ajouté à la scène
            //ListeScene.push_back(pAfficheurSprite);


            return true;
        }

        bool AnimeScene(float tempsEcoule)
        {

            // Prendre en note le statut du clavier
            GestionnaireDeSaisie.StatutClavier();

            // Prendre en note l'état de la souris
            GestionnaireDeSaisie.SaisirEtatSouris();

            m_camera->anime(0);

            /*for(auto It = ListeScene.begin(); It != ListeScene.end(); It++)
            {
                (*It)->anime(tempsEcoule);
            }*/

            PirateSimulator::GameObjectManager::singleton.animAllGameObject(tempsEcoule);

            PirateSimulator::RendererManager::singleton.update();

            return true;
        }

        PirateSimulator::GameObject* createCamera(PirateSimulator::cameraModule::BaseCamera::type cameraType,
            const PirateSimulator::cameraModule::CameraProjectionParameters &camProjParameters,
            const PirateSimulator::cameraModule::CameraMovingParameters &camMovParameters)
        {
            PirateSimulator::GameObject* camera;
            PirateSimulator::Transform cameraInitialPosition;

            cameraInitialPosition.m_position = XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f);
            cameraInitialPosition.m_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
            cameraInitialPosition.m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

            camera = new PirateSimulator::GameObject(cameraInitialPosition);

            
            switch (cameraType)
            {
            case PirateSimulator::cameraModule::BaseCamera::FREE_CAMERA:

                camera->addComponent<PirateSimulator::cameraModule::BaseCamera>(
                    new PirateSimulator::cameraModule::BaseCamera(
                        camProjParameters, camMovParameters
                    )
                );

                camera->addComponent<PirateSimulator::IBehaviour>(new PirateSimulator::cameraModule::FreeCameraBehaviour());

                break;

            case PirateSimulator::cameraModule::BaseCamera::LEVEL_CAMERA:
                camera->addComponent<PirateSimulator::cameraModule::BaseCamera>(
                    new PirateSimulator::cameraModule::BaseCamera(
                        camProjParameters, camMovParameters
                    )
                );

                camera->addComponent<PirateSimulator::IBehaviour>(new PirateSimulator::cameraModule::LevelCameraBehaviour());

                break;

            case PirateSimulator::cameraModule::BaseCamera::OBJECT_CAMERA:

                camera->addComponent<PirateSimulator::cameraModule::BaseCamera>(
                    new PirateSimulator::cameraModule::BaseCamera(
                        camProjParameters, camMovParameters
                    )
                );

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


        // La seule scène
        //std::vector<PirateSimulator::GameObject*> ListeScene;

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

