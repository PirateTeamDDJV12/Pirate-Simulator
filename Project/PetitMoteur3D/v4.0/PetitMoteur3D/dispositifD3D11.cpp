#include "StdAfx.h"
#include "resource.h"
#include "DispositifD3D11.h"
#include "Util.h"

using namespace UtilitairesDX;

namespace PM3D
{
	CDispositifD3D11::CDispositifD3D11(void)
	{
	}

	CDispositifD3D11::~CDispositifD3D11(void)
	{
		if (pImmediateContext) pImmediateContext->ClearState();
		DXRelacher(pRenderTargetView);		
		DXRelacher(pImmediateContext);
		DXRelacher(pSwapChain);
		DXRelacher (pD3DDevice);
	}

	//  FONCTION : CDispositifD3D11, constructeur paramètré 
	//  BUT :	Constructeur de notre classe spécifique de dispositif 
	//  PARAMÈTRES:		
	//		cdsMode:	CDS_FENETRE application fenêtrée
	//					CDS_PLEIN_ECRAN application plein écran
	//
	//		hWnd:	Handle sur la fenêtre Windows de l'application,
	//    	nécessaire pour la fonction de création du 
	//				dispositif
	CDispositifD3D11::CDispositifD3D11(const CDS_MODE cdsMode,
                   const HWND hWnd)
	{

		UINT largeur;
		UINT hauteur;
		UINT createDeviceFlags = 0;

		#ifdef _DEBUG
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE( featureLevels );

		pImmediateContext = NULL;		
		pSwapChain = NULL;
		pRenderTargetView = NULL;

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory( &sd, sizeof(sd) );

		switch (cdsMode)
		{
		case CDS_FENETRE:
			RECT rc;
			GetClientRect( hWnd, &rc );
			largeur = rc.right - rc.left;
			hauteur = rc.bottom - rc.top;

			sd.BufferCount = 1;
			sd.BufferDesc.Width = largeur;
			sd.BufferDesc.Height = hauteur;
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.BufferDesc.RefreshRate.Numerator = 60;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.OutputWindow = hWnd;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.Windowed = TRUE;

			break;
		}			
		
		DXEssayer( D3D11CreateDeviceAndSwapChain(	0, 
													D3D_DRIVER_TYPE_HARDWARE, 
													NULL, 
													createDeviceFlags, 
													featureLevels, numFeatureLevels,
													D3D11_SDK_VERSION, 
													&sd, 
													&pSwapChain, 
													&pD3DDevice, 
													NULL, 
													&pImmediateContext ),
													DXE_ERREURCREATIONDEVICE) ;

		// Création d'un «render target view»
		ID3D11Texture2D *pBackBuffer;
		DXEssayer( pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&pBackBuffer ), DXE_ERREUROBTENTIONBUFFER ) ;
 
		DXEssayer(pD3DDevice->CreateRenderTargetView( pBackBuffer, NULL, &pRenderTargetView ), DXE_ERREURCREATIONRENDERTARGET);
		pBackBuffer->Release();

		pImmediateContext->OMSetRenderTargets( 1, &pRenderTargetView, NULL );

		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)largeur;
		vp.Height = (FLOAT)hauteur;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		pImmediateContext->RSSetViewports( 1, &vp );

	}

	void CDispositifD3D11::PresentSpecific()
	{
		pSwapChain->Present( 0, 0 );
	}



}

