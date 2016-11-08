#pragma once
#include "d3dx11effect.h"
#include "Objet3D.h"
#include "DispositifD3D11.h"
#include "Bloc.h"   // Pour l'index du bloc
#include "Texture.h"

namespace PM3D
{
	
	
	//  Classe : CBlocEffet1
	//
	//  BUT : 	Classe de bloc avec effet version 1 section 6.5
	//
	class CBlocEffet1 : public CObjet3D 
	{
	public:
		CBlocEffet1(const float dx, const float dy, const float dz, 
      	CDispositifD3D11* pDispositif);

   		// Destructeur
		virtual ~CBlocEffet1(void);

		virtual void Anime(float tempsEcoule);

		void Draw();

		void SetTexture(CTexture* pTexture);

	protected :
		// Constructeur par défaut
		CBlocEffet1(void){}

		void InitEffet();

		CDispositifD3D11* pDispositif;

		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;

		// Définitions des valeurs d'animation
		ID3D11Buffer* pConstantBuffer;		
		XMMATRIX matWorld;		
		float rotation;

		// Pour les effets
		ID3DX11Effect* pEffet;
		ID3DX11EffectTechnique* pTechnique; 
		ID3DX11EffectPass* pPasse; 
		ID3D11InputLayout* pVertexLayout;

		ID3D11ShaderResourceView* pTextureD3D;
		ID3D11SamplerState* pSampleState;


	};

}

	