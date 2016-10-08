#pragma once

namespace PM3D
{
	// Constantes pour mode fenêtré ou plein écran
	enum CDS_MODE
	{
		 CDS_FENETRE,
		 CDS_PLEIN_ECRAN
	};

  	//
	//  Classe : CDispositif
	//
	//  BUT : 	Classe servant à construire un objet Dispositif 
	//				qui implantera les aspects "génériques" du dispositif de 
	//				rendu
	//
	class CDispositif 
	{
	public:
		virtual void Present()
		{
			PresentSpecific();
		};

		virtual void PresentSpecific()=0;

		float GetLargeur() {return largeurEcran;}
		float GetHauteur() {return hauteurEcran;}

  	protected :
   // Constructeur par défaut
		CDispositif(void){}
	
   	// Destructeur
		virtual ~CDispositif(void){}

		float largeurEcran;
		float hauteurEcran;


	};

}
