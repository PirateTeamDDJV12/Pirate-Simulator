#include "StdAfx.h"
#include "resource.h"
#include "DispositifD3D11.h"
#include "Util.h"
#include "InfoDispositif.h"

using namespace UtilitairesDX;

namespace PM3D
{
	CDispositifD3D11::CDispositifD3D11(void)
	{
	}

	CDispositifD3D11::~CDispositifD3D11(void)
	{
		pSwapChain->SetFullscreenState(FALSE, NULL); // passer en mode fenêtré

		DXRelacher (mSolidCullBackRS);
		DXRelacher (pDepthTexture);
		DXRelacher (pDepthStencilView );

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

		// Obtenir les informations de l'adaptateur de défaut
		CInfoDispositif Dispo0( ADAPTATEUR_COURANT );

		largeur = 1024;
		hauteur = 768;

		switch (cdsMode)
		{
		case CDS_FENETRE:
			sd.Windowed = TRUE;

			break;

		case CDS_PLEIN_ECRAN:
			sd.Windowed = FALSE;
			break;

		}	

		DXGI_MODE_DESC  desc;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.Height = hauteur;
		desc.Width = largeur;
		desc.RefreshRate.Numerator = 60;
		desc.RefreshRate.Denominator = 1;
		desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		CInfoDispositif DispoVoulu(desc);
		DispoVoulu.GetDesc(desc);

		largeur = desc.Width;
		hauteur = desc.Height;
		largeurEcran = float(largeur);
		hauteurEcran = float(hauteur);

		sd.BufferCount = 1;
		sd.BufferDesc.Width = desc.Width;
		sd.BufferDesc.Height = desc.Height;
		sd.BufferDesc.Format = desc.Format;
		sd.BufferDesc.RefreshRate.Numerator = desc.RefreshRate.Numerator;
		sd.BufferDesc.RefreshRate.Denominator = desc.RefreshRate.Denominator;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // Permettre l'échange plein écran

		// régler le problème no 1 du passage en mode fenêtré
		RECT rcClient, rcWindow;
		POINT ptDiff;
		GetClientRect(hWnd, &rcClient);
		GetWindowRect(hWnd, &rcWindow);
		ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
		ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
		MoveWindow(hWnd,rcWindow.left, rcWindow.top, largeur + ptDiff.x, hauteur + ptDiff.y, TRUE);

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

		InitDepthBuffer();

		pImmediateContext->OMSetRenderTargets( 1, &pRenderTargetView, pDepthStencilView );

		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)largeur;
		vp.Height = (FLOAT)hauteur;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		pImmediateContext->RSSetViewports( 1, &vp );

		// Création et initialisation des états
		D3D11_RASTERIZER_DESC rsDesc;
		ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_BACK;
		rsDesc.FrontCounterClockwise = false;
		pD3DDevice->CreateRasterizerState(&rsDesc, &mSolidCullBackRS);

		pImmediateContext->RSSetState(mSolidCullBackRS);

	}

	void CDispositifD3D11::PresentSpecific()
	{
		pSwapChain->Present( 0, 0 );
	}

	void CDispositifD3D11::InitDepthBuffer()
	{
		D3D11_TEXTURE2D_DESC depthTextureDesc;
		ZeroMemory( &depthTextureDesc, sizeof( depthTextureDesc ) );
		depthTextureDesc.Width = largeurEcran;
		depthTextureDesc.Height = hauteurEcran;
		depthTextureDesc.MipLevels = 1;
		depthTextureDesc.ArraySize = 1;
		depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTextureDesc.SampleDesc.Count = 1;
		depthTextureDesc.SampleDesc.Quality = 0;
		depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTextureDesc.CPUAccessFlags = 0;
		depthTextureDesc.MiscFlags = 0;

		DXEssayer( pD3DDevice->CreateTexture2D( &depthTextureDesc, NULL, &pDepthTexture ),DXE_ERREURCREATIONTEXTURE );

		// Création de la vue du tampon de profondeur (ou de stencil)
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSView;
		ZeroMemory( &descDSView, sizeof( descDSView ) );
		descDSView.Format = depthTextureDesc.Format;
		descDSView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSView.Texture2D.MipSlice = 0;
		DXEssayer( pD3DDevice->CreateDepthStencilView( pDepthTexture, &descDSView, &pDepthStencilView ), DXE_ERREURCREATIONDEPTHSTENCILTARGET  );
	
	}

}

