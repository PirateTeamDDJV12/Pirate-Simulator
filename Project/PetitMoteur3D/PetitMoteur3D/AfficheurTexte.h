#pragma once

#include <string.h>
#include "dispositifD3D11.h"
#include <Gdiplus.h>
#include <string>
using namespace Gdiplus;
using namespace std;
#pragma comment(lib, "gdiplus.lib")

namespace PM3D
{

class CAfficheurTexte
{
public:
	CAfficheurTexte(CDispositifD3D11* pDispositif, int largeur, int hauteur, Font* pPolice);
	~CAfficheurTexte();
	void Ecrire(wstring s);
	ID3D11ShaderResourceView* GetTextureView() {return pTextureView;}

	static void Init();
	static void Close();

protected:
	UINT TexWidth;
	UINT TexHeight;

	ID3D11Texture2D *pTexture;
	IDXGISurface1* pSurface;
	ID3D11ShaderResourceView* pTextureView;
	CDispositifD3D11* pDispo;

	Font* pFont;
	Bitmap* pCharBitmap;
	Graphics* pCharGraphics;
	SolidBrush* pBlackBrush;

	// Variables statiques pour GDI+
	static	ULONG_PTR token;

};

}
