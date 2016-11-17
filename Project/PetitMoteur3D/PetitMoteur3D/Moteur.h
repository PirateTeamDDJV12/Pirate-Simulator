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
#include "../../PirateSimulator/PlayerBehaviour.h"

#include "../../PirateSimulator/TimeManager.h"

#include "../../PirateSimulator/GameObjectManager.h"
#include "../../PirateSimulator/RendererManager.h"
#include "../../PirateSimulator/CameraManager.h"
#include "../../PirateSimulator/InputManager.h"


namespace PM3D
{

    const int IMAGESPARSECONDE = 60;

    //
    //   TEMPLATE�: CMoteur
    //
    //   BUT�: Template servant � construire un objet Moteur qui implantera les
    //         aspects "g�n�riques" du moteur de jeu
    //
    //   COMMENTAIRES�:
    //
    //        Comme plusieurs de nos objets repr�senteront des �l�ments uniques 
    //		  du syst�me (ex: le moteur lui-m�me, le lien vers 
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
                // Propre � la plateforme - (Conditions d'arr�t, interface, messages)
                bBoucle = RunSpecific();

                // appeler la fonction d'animation
                if (bBoucle) bBoucle = Animation();
            }
        }

        virtual int Initialisations()
        {
            // Initialisation du temps de jeu
            TimeManager::get()->startGameTime();

            // Propre � la plateforme
            InitialisationsSpecific();

            // * Initialisation du dispositif de rendu
            pDispositif = CreationDispositifSpecific(CDS_FENETRE);

            // * Initialisation de la sc�ne
            InitScene();

            // * Initialisation des param�tres de l'animation et 
            //   pr�paration de la premi�re image
            InitAnimation();

            return 0;
        }

        virtual bool Animation()
        {
            __int64 TempsCourant;
            float TempsEcoule;

            // m�thode pour lire l'heure et calculer le 
            // temps �coul�
            TempsCourant = GetTimeSpecific();

            // Est-il temps de rendre l'image?
            if (TempsCourant > TempsSuivant)
            {
                // Affichage optimis� 
                pDispositif->Present();

                TempsEcoule = static_cast<float>(TempsCourant - TempsPrecedent)
                    * static_cast<float>(EchelleTemps);

                // On pr�pare la prochaine image
                AnimeScene(TempsEcoule);

                // On rend l'image sur la surface de travail 
                // (tampon d'arri�re plan)
                RenderScene();

                // Calcul du temps du prochain affichage
                TempsPrecedent = TempsCourant;
                TempsSuivant = TempsCourant + EcartTemps;
            }

            return true;
        }

        CGestionnaireDeTextures& GetTextureManager()
        {
            return TexturesManager;
        }
        CDIManipulateur& GetGestionnaireDeSaisie()
        {
            return GestionnaireDeSaisie;
        }

    protected:

        // Constructeur par d�faut
        CMoteur(void)
        {}

        // Destructeur
        ~CMoteur(void)
        {
            Cleanup();
        }

        // Sp�cifiques - Doivent �tre implant�s
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

            // premi�re Image
            RenderScene();

            return true;
        }

        // Fonctions de rendu et de pr�sentation de la sc�ne
        virtual bool RenderScene()
        {
            BeginRenderSceneSpecific();

            // Appeler les fonctions de dessin de chaque objet de la sc�ne

            PirateSimulator::RendererManager::singleton.draw();

            EndRenderSceneSpecific();
            return true;
        }


        virtual void Cleanup()
        {

            // Vider les textures
            TexturesManager.Cleanup();

            // D�truire le dispositif
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

            PirateSimulator::Transform cameraTransform = PirateSimulator::Transform();
            cameraTransform.m_position = XMVectorSet(0.f, 0.f, -10.f, 0.f);
            cameraTransform.m_forward = XMVectorSet(0.f, 0.f, 1.f, 0.f);
            cameraTransform.m_up = XMVectorSet(0.f, 1.f, 0.f, 0.f);

            PirateSimulator::CameraManager::singleton.createCamera(
                PirateSimulator::cameraModule::BaseCamera::type::OBJECT_CAMERA,
                cameraTransform,
                camProjParameters, 
                camMovParameters, 
                "mainCamera"
            );

            // Skybox
            PirateSimulator::CSkybox* skyBoxMesh = new PirateSimulator::CSkybox(pDispositif);
            m_skybox = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
                new PirateSimulator::GameObject(PirateSimulator::CameraManager::singleton.getMainCameraGO()->m_transform, "skybox")
            );

            m_skybox->addComponent<PirateSimulator::IMesh>(skyBoxMesh);

            skyBoxMesh->SetTexture(new CTexture(L"PirateSimulator/skybox.dds", pDispositif));

            PirateSimulator::RendererManager::singleton.addAnObligatoryMeshToDrawBefore(skyBoxMesh);

            // Initialisation des objets 3D - cr�ation et/ou chargement
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
            vehicule->addComponent<PirateSimulator::IMesh>(new CObjetMesh(".\\modeles\\Boat\\boat.OMB", ShaderCObjectMesh::ShadersParams(), chargeur, pDispositif));*/

            auto vehiculeMesh = new CObjetMesh(".\\modeles\\Boat\\boat.OMB", ShaderCObjectMesh::ShadersParams(), pDispositif);
            vehicule->addComponent<PirateSimulator::IMesh>(vehiculeMesh);
            vehicule->addComponent<PirateSimulator::IBehaviour>(new PirateSimulator::PlayerBehaviour());

            /*          auto personnage = PirateSimulator::GameObjectManager::singleton.subscribeAGameObject(
            new PirateSimulator::GameObject(transform, "personnage")
            );
            auto personageMesh = new CObjetMesh(".\\modeles\\jin\\jin.OMB", ShaderCObjectMesh::ShadersParams(), pDispositif);
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

            auto fieldMesh = new PirateSimulator::Terrain(pDispositif, terrainH, terrainW, terrainScale, "PirateSimulator/heightmapOutput.txt", "PirateSimulator/textureTerrain.dds");
#else
            // Get all the information from the config file
            auto fieldMesh = new PirateSimulator::Terrain(pDispositif);
#endif
            terrain->addComponent<PirateSimulator::IMesh>(fieldMesh);


            if (PirateSimulator::CameraManager::singleton.getCameraType() == PirateSimulator::cameraModule::BaseCamera::OBJECT_CAMERA)
            {
                PirateSimulator::CameraManager::singleton.setPairedTarget(vehicule);
            }
            else if (PirateSimulator::CameraManager::singleton.getCameraType() == PirateSimulator::cameraModule::BaseCamera::LEVEL_CAMERA)
            {
                PirateSimulator::CameraManager::singleton.setPairedTarget(terrain);
            }

            
            // Puis, il est ajout� � la sc�ne
            PirateSimulator::RendererManager::singleton.addAnObligatoryMeshToDrawBefore(fieldMesh);
            PirateSimulator::RendererManager::singleton.addAMovingSortableMesh(vehiculeMesh);
            //PirateSimulator::RendererManager::singleton.addAStaticSortableMesh(personageMesh);

            return true;
        }

        bool AnimeScene(float tempsEcoule)
        {

            // Prendre en note le statut du clavier
            GestionnaireDeSaisie.StatutClavier();

            // Prendre en note l'�tat de la souris
            GestionnaireDeSaisie.SaisirEtatSouris();


            PirateSimulator::GameObjectManager::singleton.animAllGameObject(tempsEcoule);

            PirateSimulator::RendererManager::singleton.update();
            PirateSimulator::InputManager::singleton.update();

#ifdef DEBUG_PIRATE_SIMULATOR
            PirateSimulator::InputManager::singleton.setKey(PirateSimulator::InputManager::KEY1, DIK_W);
            OutputDebugStringA(
                LPCSTR(
                    ((PirateSimulator::InputManager::singleton.getKey(PirateSimulator::InputManager::KEY1) ?
                    "W is Pressed\n" : "W not pressed\n") + to_string(++PirateSimulator::debugCount)).c_str()
                )
            );
#endif //DEBUG_PIRATE_SIMULATOR

            PirateSimulator::CameraManager::singleton.animMainCamera();

            return true;
        }

        
    protected:
        // Variables pour le temps de l'animation
        __int64 TempsSuivant;
        __int64 TempsPrecedent;
        unsigned long EcartTemps;
        double EchelleTemps;

        // Le dispositif de rendu
        TClasseDispositif* pDispositif;

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

