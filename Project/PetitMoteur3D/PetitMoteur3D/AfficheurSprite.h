#pragma once
#include <string>
#include "d3dx11effect.h"
#include "Objet3D.h"
#include "DispositifD3D11.h"
#include "Texture.h"
#include <vector>

using namespace std;

namespace PM3D
{

	class CSommetSprite
	{
	public:
		CSommetSprite (){};
		CSommetSprite ( XMFLOAT3 _position, XMFLOAT2 _coordTex)
		{   position = _position; coordTex = _coordTex; }

	public:
		static UINT numElements;
		static D3D11_INPUT_ELEMENT_DESC layout[];

		XMFLOAT3 position;
		XMFLOAT2 coordTex;
	};

class CAfficheurSprite  : 	public CObjet3D
{
public:
	CAfficheurSprite (CDispositifD3D11* _pDispositif);
	virtual ~CAfficheurSprite (void);
	virtual void Anime(float tempsEcoule){};
	virtual	void Draw();
	
	void AjouterSprite(string NomTexture, int _x, int _y, int _dx=0, int _dy=0);
	void AjouterPanneau(string NomTexture, const XMFLOAT3& _position, 
		                float _dx=0.0f, float _dy=0.0f);
	void AjouterSpriteTexte(ID3D11ShaderResourceView* pTexture, int _x, int _y);

protected:
	class CSprite
	{
	public:
		ID3D11ShaderResourceView* pTextureD3D;

		XMMATRIX matPosDim;
		bool bPanneau;
		CSprite()
		{
			bPanneau = false;
		}
	};

	class CPanneau: public CSprite
	{
	public:
		XMFLOAT3 position;
		XMFLOAT2 dimension;

		CPanneau()
		{
			bPanneau = true;
		}
	};

protected:
	static CSommetSprite sommets[6];
	ID3D11Buffer* pVertexBuffer;
	CDispositifD3D11* pDispositif;

	ID3D11Buffer* pConstantBuffer;		
	ID3DX11Effect* pEffet;
	ID3DX11EffectTechnique* pTechnique; 
	ID3DX11EffectPass* pPasse; 
	ID3D11InputLayout* pVertexLayout;

	ID3D11SamplerState* pSampleState;

	// Tous nos sprites
	vector<CSprite*> tabSprites;




protected:
	virtual	void InitEffet();



};

}

