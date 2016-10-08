#pragma once
#include "Singleton.h"
#include "dispositif.h" 

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
				// AnimeScene(TempsEcoule);

				// On rend l'image sur la surface de travail 
   				// (tampon d'arrière plan)
				RenderScene();

				// Calcul du temps du prochain affichage
				TempsPrecedent=TempsCourant;
				TempsSuivant = TempsCourant + EcartTemps;
			}

			return true;
		}

	
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
			// à suivre...

			EndRenderSceneSpecific();
			return true;
		}


		virtual void Cleanup()
		{
			// détruire les objets
			// à suivre
			
			// Détruire le dispositif
			if (pDispositif) 
			{
				delete pDispositif;
				pDispositif = NULL;
			}
		}



	protected:
		// Variables pour le temps de l'animation
		__int64 TempsSuivant;
		__int64 TempsPrecedent;
		unsigned long EcartTemps;
		double EchelleTemps;

		// Le dispositif de rendu
		TClasseDispositif* pDispositif;


    };


} // namespace PM3D

	