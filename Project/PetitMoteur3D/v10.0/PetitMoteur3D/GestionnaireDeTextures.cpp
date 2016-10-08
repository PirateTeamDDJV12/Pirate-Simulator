
#include "StdAfx.h"
#include "dispositifD3D11.h"
#include "GestionnaireDeTextures.h"

namespace PM3D
{

CGestionnaireDeTextures::CGestionnaireDeTextures(void)
{
}

CGestionnaireDeTextures::~CGestionnaireDeTextures(void)
{
	Cleanup();
}

CTexture* const CGestionnaireDeTextures::GetNewTexture(const wchar_t* filename_in,CDispositifD3D11* pDispositif)
{	
	CTexture* pTexture;

	// On vérifie si la texture est déjà dans notre liste
	pTexture = GetTexture(filename_in);

	// Si non, on la crée
	if (!pTexture)
	{
		pTexture = new CTexture(filename_in, pDispositif);

		// Puis, il est ajouté à la scène
		ListeTextures.push_back(pTexture);
	}

	return pTexture;
}

CTexture* const CGestionnaireDeTextures::GetTexture(const wchar_t* filename_in)
{
CTexture* pTexture = 0;
std::vector<CTexture*>::iterator It;
	
	It = ListeTextures.begin();

	while ( pTexture==0 && It != ListeTextures.end() )
	{
		if ( wcscmp( (*It)->GetFilename(), filename_in)==0)
		{
			pTexture = *It;
		}

		It++;
	}

	return pTexture;
}

void CGestionnaireDeTextures::Cleanup()
{
// détruire les objets
std::vector<CTexture*>::iterator It;

	for (It = ListeTextures.begin(); It != ListeTextures.end();It++)
	{
		delete (*It);
	}

	ListeTextures.clear();
}

} 

