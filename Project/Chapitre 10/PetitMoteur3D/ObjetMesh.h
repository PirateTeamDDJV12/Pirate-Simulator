#pragma once
#include "objet3d.h"
#include "dispositifD3D11.h"
#include "d3dx11effect.h"
#include "chargeur.h"

#include <vector>
using namespace std;

namespace PM3D
{

class CObjetMesh :	public CObjet3D
{
protected:
	class CSommetMesh
	{
	public:
		CSommetMesh(){};
		CSommetMesh(XMFLOAT3 _position, XMFLOAT3 _normal, XMFLOAT2 _coordTex = XMFLOAT2(0,0));

	public:
		static UINT numElements;
		static D3D11_INPUT_ELEMENT_DESC layout[];

		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 coordTex;
	};

	struct MaterialBlock
	{
		char NomFichierTexture[200];
		char NomMateriau[60];
		XMFLOAT4 Ambient;
		XMFLOAT4 Diffuse;
		XMFLOAT4 Specular;
		float Puissance;
		bool transparent;	
	};

	class CMaterial
	{
	public:
		string NomFichierTexture;
		string NomMateriau;
		ID3D11ShaderResourceView* pTextureD3D;

		XMFLOAT4 Ambient;
		XMFLOAT4 Diffuse;
		XMFLOAT4 Specular;
		float Puissance;
		bool transparent;

		CMaterial()
		{
			Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
			transparent = false;
			Puissance = 1;
			pTextureD3D = NULL;
			NomFichierTexture = "";
		}

		void MatToBlock( MaterialBlock& mb)
		{
			strcpy_s(mb.NomFichierTexture, NomFichierTexture.c_str());
			strcpy_s(mb.NomMateriau, NomMateriau.c_str());
			mb.Ambient = Ambient;
			mb.Diffuse = Diffuse;
			mb.Specular = Specular;
			mb.Puissance = Puissance;
			mb.transparent = transparent;
		
		}

		void BlockToMat( MaterialBlock& mb)
		{
			NomFichierTexture.append(mb.NomFichierTexture);
			NomMateriau.append(mb.NomMateriau);
			Ambient = mb.Ambient;
			Diffuse = mb.Diffuse;
			Specular = mb.Specular;
			Puissance = mb.Puissance;
			transparent = mb.transparent;
		}
	};


public:
	CObjetMesh(IChargeur& chargeur, CDispositifD3D11* pDispositif);
	CObjetMesh(IChargeur& chargeur, string nomfichier, CDispositifD3D11* _pDispositif);
	CObjetMesh(string nomfichier, CDispositifD3D11* _pDispositif);

	virtual ~CObjetMesh(void);

	virtual void Anime(float tempsEcoule);

	void Draw();

protected:
	// **** Fonctions
	void TransfertObjet(IChargeur& chargeur);
	void InitEffet();
	void EcrireFichierBinaire(IChargeur& chargeur, string nomFichier);
	void LireFichierBinaire(string nomFichier);

protected:
	// ****  Données membres

	// Pour le dessin
	CDispositifD3D11* pDispositif;		// On prend en note le dispositif

	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;

	// Les sous-objets
	int NombreSubset;				// Nombre de sous-objets dans le mesh
	vector<int> SubsetMaterialIndex;// Index des matériaux
	vector<int> SubsetIndex;		// Index des sous-objets

	vector<CMaterial> Material;		// Vecteur des matériaux

	// Pour les effets et shaders
	ID3D11SamplerState* pSampleState;
	ID3D11Buffer* pConstantBuffer;		
	ID3DX11Effect* pEffet;
	ID3DX11EffectTechnique* pTechnique; 
	ID3DX11EffectPass* pPasse; 
	ID3D11InputLayout* pVertexLayout;

};

}

