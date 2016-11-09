#include "StdAfx.h"
#include "InfoDispositif.h"

#include "util.h"
#include <vector>
using namespace UtilitairesDX;

namespace PM3D
{

CInfoDispositif::CInfoDispositif(void)
{
}


CInfoDispositif::~CInfoDispositif(void)
{
}

// Pour obtenir les informations à partir d'un numéro d'adaptateur
// 0 = défaut = ADAPTATEUR_DE_DEFAUT
CInfoDispositif::CInfoDispositif( int NoAdaptateur )
{
	IDXGIFactory* pFactory = NULL;
	IDXGIAdapter* pAdapter = NULL;
	IDXGIOutput* pOutput = NULL;	
	
	valide = false;

	// Créer un IDXGIFactory.
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);

	if(FAILED(pFactory->EnumAdapters(NoAdaptateur, &pAdapter))) return;

	// Obtenir les informations de la première sortie de l'adaptateur 
	// (Le moniteur)
	if(FAILED(pAdapter->EnumOutputs(0, &pOutput))) return;

	// Obtenir la description de l'état courant
	DXGI_OUTPUT_DESC outDesc;
	pOutput->GetDesc(&outDesc);

	largeur = outDesc.DesktopCoordinates.right - outDesc.DesktopCoordinates.left;
	hauteur = outDesc.DesktopCoordinates.bottom - outDesc.DesktopCoordinates.top;
	valide = true;

	DXGI_ADAPTER_DESC Desc;
	pAdapter->GetDesc(&Desc);

	// Mémoire dédiée (en megabytes).
	memoire = (int)(Desc.DedicatedVideoMemory / 1024 / 1024);

	// Nom de la carte video.
	wcscpy_s(nomcarte, 100, Desc.Description);
   	
	// Faire le ménage pour éviter les «memory leaks»
	DXRelacher(pOutput);
    DXRelacher(pAdapter);
	DXRelacher (pFactory);
}


CInfoDispositif::CInfoDispositif(DXGI_MODE_DESC modeDesc )
{
	// Énumération des adaptateurs
	IDXGIFactory * pFactory = NULL;

	CreateDXGIFactory(__uuidof(IDXGIFactory) ,(void**)&pFactory);

	IDXGIAdapter * pAdapter; 
	std::vector <IDXGIAdapter*> vAdapters;

	for ( UINT i = 0; 
		  pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i )
	{
		vAdapters.push_back(pAdapter); 
	} 

	// On travaille presque toujours avec vAdapters[0] 
	// à moins d'avoir plusieurs cartes non-collaboratives
	*this = CInfoDispositif(0);

	// Obtenir la sortie 0 - le moniteur principal
	IDXGIOutput* pOutput = NULL; 
	vAdapters[0]->EnumOutputs(0,&pOutput);
		
	// Obtenir le mode le plus intéressant
	pOutput->FindClosestMatchingMode( &modeDesc, &mode, NULL);
  
	// Faire le ménage pour éviter les «memory leaks»
	DXRelacher(pOutput);

	for (unsigned int i=0; i<vAdapters.size(); ++i)
	{
		DXRelacher(vAdapters[i]);
	}

	DXRelacher (pFactory);


}


}