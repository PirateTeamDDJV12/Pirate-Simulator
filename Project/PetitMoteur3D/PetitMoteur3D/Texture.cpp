#include "strsafe.h"
#include "dispositifD3D11.h"
#include "Texture.h"
#include "resource.h"
#include "util.h"
#include "DDSTextureLoader.h"

#include "../../PirateSimulator/RendererManager.h"

using namespace UtilitairesDX;
using namespace DirectX;

namespace PM3D
{

CTexture::CTexture()
{
}

CTexture::~CTexture()
{
	DXRelacher(pTexture);
}

CTexture::CTexture(const wchar_t* filename_in)
{
	pTexture = 0;
	ID3D11Device* pDevice = PirateSimulator::RendererManager::singleton.getDispositif()->GetD3DDevice();

	StringCchCopy( filename, sizeof(filename), filename_in);  
 
	// Charger la texture en ressource
	DXEssayer( CreateDDSTextureFromFile( pDevice,
										 filename,
                                         nullptr,
										 &pTexture), 
                  DXE_FICHIERTEXTUREINTROUVABLE );

    DXRelacher(pDevice);
}

}