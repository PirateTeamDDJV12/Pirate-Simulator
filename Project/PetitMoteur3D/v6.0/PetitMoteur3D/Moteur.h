#pragma once
#include "Singleton.h"
#include "dispositif.h" 

#include <vector>
#include "Objet3D.h"
#include "Bloc.h"
#include "BlocEffet1.h"

namespace PM3D
{

	const int IMAGESPARSECONDE=20;

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
	template <class T, class TClasseDispositif> class CMoteur :public CSingleton<T>
	{
	public:

		virtual void Run()
		{
		bool bBoucle=true;

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
			// Propre à la plateforme
			InitialisationsSpecific();

			// * Initialisation du dispositif de rendu
			pDispositif = CreationDispositifSpecific( CDS_FENETRE );

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

				TempsEcoule=static_cast<float>(TempsCourant-TempsPrecedent) 
							* static_cast<float>(EchelleTemps);

				// On prépare la prochaine image
				AnimeScene(TempsEcoule);

				// On rend l'image sur la surface de travail 
   				// (tampon d'arrière plan)
				RenderScene();

				// Calcul du temps du prochain affichage
				TempsPrecedent=TempsCourant;
				TempsSuivant = TempsCourant + EcartTemps;
			}

			return true;
		}

		XMMATRIX GetMatView() { return matView; }
		XMMATRIX GetMatProj() { return matProj; }
		XMMATRIX GetMatViewProj() { return matViewProj; }

    protected :

        // Constructeur par défaut
		CMoteur(void){}
	
        // Destructeur
		~CMoteur(void)
		{
			Cleanup();
		}

		// Spécifiques - Doivent être implantés
		virtual bool RunSpecific()=0;
		virtual int InitialisationsSpecific()=0;
		virtual __int64 GetTimeSpecific()=0;
		virtual TClasseDispositif* CreationDispositifSpecific(const CDS_MODE cdsMode)=0;
		virtual void BeginRenderSceneSpecific()=0;
		virtual void EndRenderSceneSpecific()=0;

		// Autres fonctions
		virtual int InitAnimation()
		{
			TempsSuivant = GetTimeSpecific();
			EchelleTemps = 0.001; 
			EcartTemps = 1000/IMAGESPARSECONDE;
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
			std::vector<CObjet3D*>::iterator It;

			for (It = ListeScene.begin(); It != ListeScene.end(); It++)
			{
					(*It)->Draw();
			}

			EndRenderSceneSpecific();
			return true;
		}


		virtual void Cleanup()
		{
			// détruire les objets
			std::vector<CObjet3D*>::iterator It;

			for (It = ListeScene.begin(); It != ListeScene.end();It++)
			{
				delete *It;
			}

			ListeScene.clear();
			
			// Détruire le dispositif
			if (pDispositif) 
			{
				delete pDispositif;
				pDispositif = NULL;
			}
		}

	virtual int InitScene()
	{
		// Initialisation des objets 3D - création et/ou chargement
		if (!InitObjets()) return 1;
	
		// Initialisation des matrices View et Proj
		// Dans notre cas, ces matrices sont fixes
		matView = XMMatrixLookAtLH( XMVectorSet( 0.0f, 0.0f,-10.0f, 1.0f ),
       								XMVectorSet( 0.0f, 0.0f, 0.0f, 1.0f ),
                     				XMVectorSet( 0.0f, 1.0f, 0.0f, 1.0f ) );

		float champDeVision = XM_PI/4; 	// 45 degrés
		float ratioDAspect = pDispositif->GetLargeur()/pDispositif->GetHauteur();		
		float planRapproche = 2.0;
		float planEloigne = 20.0;
		
		matProj = XMMatrixPerspectiveFovLH( 
									champDeVision,
									ratioDAspect,
									planRapproche,
									planEloigne );

		// Calcul de VP à l'avance
		matViewProj = matView * matProj;

		return 0;
	}

	bool InitObjets()
	{
	CBlocEffet1* pBloc;

		// Création d'un cube de 2 X 2 X 2 unités
		// Le bloc est créé dans notre programme et sur le dispositif
		pBloc = new CBlocEffet1( 2, 2, 2, pDispositif );
		
		// Puis, il est ajouté à la scène
		ListeScene.push_back(pBloc);
		
		return true;
	}

	bool AnimeScene(float tempsEcoule)
	{
	std::vector<CObjet3D*>::iterator It;

	    for (It = ListeScene.begin(); It != ListeScene.end();It++)
		{
			(*It)->Anime(tempsEcoule);
		}

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


		// La seule scène
		std::vector<CObjet3D*> ListeScene;

		// Les matrices
		XMMATRIX matView;
		XMMATRIX matProj;
		XMMATRIX matViewProj;

    };


} // namespace PM3D

	