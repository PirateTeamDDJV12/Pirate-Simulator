#pragma once
#include "Singleton.h"


namespace PM3D
{

	const int IMAGESPARSECONDE=20;

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
	template <class T> class CMoteur :public CSingleton<T>
	{
	public:

		virtual void Run()
		{
		bool bBoucle=true;

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
			// Propre � la plateforme
			InitialisationsSpecific();

			// * Initialisation du dispositif de rendu

			// * Initialisation de la sc�ne

			// * Initialisation des param�tres de l'animation et 
			//   pr�paration de la premi�re image
			//InitAnimation();

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
				//	pDispositif->Present(); // On enlevera �//� plus tard

				TempsEcoule=static_cast<float>(TempsCourant-TempsPrecedent) 
							* static_cast<float>(EchelleTemps);

				// On pr�pare la prochaine image
				// AnimeScene(TempsEcoule);

				// On rend l'image sur la surface de travail 
   				// (tampon d'arri�re plan)
				RenderScene();

				// Calcul du temps du prochain affichage
				TempsPrecedent=TempsCourant;
				TempsSuivant = TempsCourant + EcartTemps;
			}

			return true;
		}

	
    protected :

        // Constructeur par d�faut
		CMoteur(void){}
	
        // Destructeur
		~CMoteur(void){}

		// Sp�cifiques - Doivent �tre implant�s
		virtual bool RunSpecific()=0;
		virtual int InitialisationsSpecific()=0;

		virtual int InitAnimation()
		{
			TempsSuivant = GetTimeSpecific();
			EchelleTemps = 0.001; 
			EcartTemps = 1000/IMAGESPARSECONDE;
			TempsPrecedent = TempsSuivant; 

			// premi�re Image
			RenderScene();

			return true;
		}

	// Fonctions sp�cifiques au rendu et � la pr�sentation de la sc�ne
	virtual bool RenderScene()
	{
		return true;
	}

	virtual __int64 GetTimeSpecific()=0;


	protected:
		// Variables pour le temps de l'animation
		__int64 TempsSuivant;
		__int64 TempsPrecedent;
		unsigned long EcartTemps;
		double EchelleTemps;


    };


} // namespace PM3D

	