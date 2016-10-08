#pragma once
#include "dispositif.h"

using namespace DirectX;

namespace PM3D
{
 	//  Classe : CObjet3D
	//
	//  BUT : 	Classe de base de tous nos objets 3D
	//
	class CObjet3D 
	{
	public:

   		// Destructeur
		virtual ~CObjet3D(void){}

		virtual void Anime(float tempsEcoule){};
		virtual void Draw()=0;

	protected :
		// Constructeur par défaut
		CObjet3D(void){}
	

	};

}
