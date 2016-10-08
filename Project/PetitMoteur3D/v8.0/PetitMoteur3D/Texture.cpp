#include "StdAfx.h"
#include "strsafe.h"
#include "dispositifD3D11.h"
#include "Texture.h"
#include "resource.h"
#include "util.h"
#include "DDSTextureLoader.h"

using namespace UtilitairesDX;
using namespace DirectX;

namespace PM3D
{

CTexture::CTexture(void)
{
}

CTexture::~CTexture(void)
{
	DXRelacher(pTexture);
}

CTexture::CTexture(const wchar_t* filename_in,CDispositifD3D11* pDispositif)
{
	pTexture = nullptr;
	ID3D11Device* pDevice = pDispositif->GetD3DDevice();

	StringCchCopy( filename, sizeof(filename), filename_in);  
 
	// Charger la texture en ressource
	DXEssayer( CreateDDSTextureFromFile( pDevice,
										 filename,
                                         nullptr,
										 &pTexture), DXE_FICHIERTEXTUREINTROUVABLE );
			   
}


}