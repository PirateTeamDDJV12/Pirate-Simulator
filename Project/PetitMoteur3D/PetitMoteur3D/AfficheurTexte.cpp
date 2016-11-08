#include "StdAfx.h"
#include "AfficheurTexte.h"


namespace PM3D
{

ULONG_PTR CAfficheurTexte::token = 0;

CAfficheurTexte::CAfficheurTexte(CDispositifD3D11* pDispositif, int largeur, int hauteur, Font* pPolice)
{
	// Prendre en note le dispositif, la largeur et la hauteur
	pDispo=pDispositif;
	TexWidth = largeur;
	TexHeight = hauteur;

	// Prendre en note la police
	pFont = pPolice;
 
	// Cr�er le bitmap et un objet GRAPHICS (un dessinateur)
    pCharBitmap = new Bitmap( TexWidth, TexHeight, PixelFormat32bppARGB );
    pCharGraphics = new Graphics( pCharBitmap );

	// Param�tres de l'objet Graphics
	pCharGraphics->SetCompositingMode( CompositingModeSourceOver );
    pCharGraphics->SetCompositingQuality( CompositingQualityHighSpeed );
    pCharGraphics->SetInterpolationMode( InterpolationModeHighQuality );
    pCharGraphics->SetPixelOffsetMode( PixelOffsetModeHighSpeed );
    pCharGraphics->SetSmoothingMode( SmoothingModeNone );
    pCharGraphics->SetPageUnit( UnitPixel );
    TextRenderingHint hint = TextRenderingHintAntiAlias; //TextRenderingHintSystemDefault;  
	pCharGraphics->SetTextRenderingHint( hint );

	// Un brosse noire pour le remplissage
	//		Notez que la brosse aurait pu �tre pass�e 
	//		en param�tre pour plus de flexibilit�
	pBlackBrush = new SolidBrush( Gdiplus::Color( 255, 0, 0, 0 ) );        
 
	// On efface le bitmap (notez le NOIR TRANSPARENT...)
	pCharGraphics->Clear( Gdiplus::Color( 0, 0, 0, 0) );

	// Nous pourrions ici �crire une valeur initiale sur le bitmap
	// wstring s=L"Valeur initiale";
	// pCharGraphics->DrawString( s.c_str(), s.size(), pFont, PointF( 0.0f, 0.0f ), pBlackBrush );  
	
	// Acc�der aux bits du bitmap
    Gdiplus::BitmapData bmData;
    pCharBitmap->LockBits( &Rect( 0, 0, TexWidth, TexHeight ), ImageLockModeRead, PixelFormat32bppARGB, &bmData ); 

	// Cr�ation d'une texture de m�me dimension sur la carte graphique
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width                = TexWidth;
	texDesc.Height               = TexHeight;
    texDesc.MipLevels            = 1;
    texDesc.ArraySize            = 1;
    texDesc.Format               = DXGI_FORMAT_B8G8R8A8_UNORM;
    texDesc.SampleDesc.Count     = 1;
    texDesc.SampleDesc.Quality   = 0;
    texDesc.Usage                = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags            = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags       = 0;
    texDesc.MiscFlags            = 0;

    D3D11_SUBRESOURCE_DATA data;        
    data.pSysMem            = bmData.Scan0;
    data.SysMemPitch        = TexWidth * 4;
    data.SysMemSlicePitch    = 0;

	// Cr�ation de la texture � partir des donn�es du bitmap
    HRESULT hr=pDispo->GetD3DDevice()->CreateTexture2D( &texDesc, &data, &pTexture );

	// Cr�ation d'un �resourve view� pour acc�der facilement � la texture
	//     (comme pour les sprites)
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format                      = DXGI_FORMAT_B8G8R8A8_UNORM;
    srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels         = 1;
    srvDesc.Texture2D.MostDetailedMip   = 0;

    hr=pDispositif->GetD3DDevice()->CreateShaderResourceView( pTexture,&srvDesc, &pTextureView );
	
    pCharBitmap->UnlockBits( &bmData );  

}

void CAfficheurTexte::Init()
{
	GdiplusStartupInput  startupInput( 0, TRUE, TRUE );
	GdiplusStartupOutput startupOutput;

	GdiplusStartup( &token, &startupInput, &startupOutput );
}

void CAfficheurTexte::Close()
{
		GdiplusShutdown( token );
}

CAfficheurTexte::~CAfficheurTexte(void)
{
	pTexture->Release();
	delete pBlackBrush;
	delete pCharGraphics;
	delete pCharBitmap;
}

void CAfficheurTexte::Ecrire(wstring s)
{
	// Effacer
	pCharGraphics->Clear( Gdiplus::Color( 0, 0, 0, 0) );

	// �crire le nouveau texte
	pCharGraphics->DrawString( s.c_str(), s.size(), pFont, PointF( 0.0f, 0.0f ), pBlackBrush );  

	// Transf�rer
	Gdiplus::BitmapData bmData;
	pCharBitmap->LockBits( &Rect( 0, 0, TexWidth, TexHeight ), ImageLockModeRead, PixelFormat32bppARGB, &bmData ); 

	pDispo->GetImmediateContext()->UpdateSubresource( pTexture, 0, 0, bmData.Scan0, TexWidth*4, 0 );

    pCharBitmap->UnlockBits( &bmData );  
}

}