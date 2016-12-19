#ifndef _AFFICHEURTEXTE_H_
#define _AFFICHEURTEXTE_H_


#include <string.h>
#include <Gdiplus.h>
#include <string>
using namespace Gdiplus;
using namespace std;
#pragma comment(lib, "gdiplus.lib")

namespace PM3D
{
    class CDispositifD3D11;

    class CAfficheurTexte
    {
    public:
        CAfficheurTexte(int largeur, int hauteur, Gdiplus::Font* pPolice,
            Gdiplus::Color couleurTexte = Gdiplus::Color(255, 255, 255, 255), Gdiplus::Color couleurTex = Gdiplus::Color(0, 0, 0, 0));
        ~CAfficheurTexte();
        void Ecrire(wstring s);
        ID3D11ShaderResourceView* GetTextureView()
        {
            return pTextureView;
        }

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

#endif
