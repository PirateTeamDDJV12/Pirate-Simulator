#pragma once
#include "objet3d.h"
#include "objet3d.h"
#include "dispositifD3D11.h"
#include "d3dx11effect.h"
#include "chargeur.h"

#include <vector>
using namespace std;

namespace PM3D
{
	class CObjetMesh : public CObjet3D
	{
	protected:
		class CSommetMesh
		{
		public:
			CSommetMesh(){};
			CSommetMesh(XMFLOAT3 _position, XMFLOAT3 _normal,
				XMFLOAT2 _coordTex = XMFLOAT2(0, 0));

		public:
			static UINT numElements;
			static D3D11_INPUT_ELEMENT_DESC layout[];

			XMFLOAT3 position;
			XMFLOAT3 normal;
			XMFLOAT2 coordTex;
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
				Puissance = 0;
				pTextureD3D = NULL;
				NomFichierTexture = "";
			}
		};

	public:
		CObjetMesh(IChargeur& chargeur, CDispositifD3D11* pDispositif);
		virtual ~CObjetMesh(void);

		void Draw();

		void Anime(float tempsEcoule);



	protected:
		CObjetMesh();

		void TransfertObjet(IChargeur& chargeur);

		void InitEffet();

	protected:

		struct ShadersParams
		{
			XMMATRIX matWorldViewProj;	// la matrice totale 
			XMMATRIX matWorld;			// matrice de transformation dans le monde 
			XMVECTOR vLumiere; 			// la position de la source d'éclairage (Point)
			XMVECTOR vCamera; 			// la position de la caméra
			XMVECTOR vAEcl; 			// la valeur ambiante de l'éclairage
			XMVECTOR vAMat; 			// la valeur ambiante du matériau
			XMVECTOR vDEcl; 			// la valeur diffuse de l'éclairage 
			XMVECTOR vDMat; 			// la valeur diffuse du matériau
			XMVECTOR vSEcl; 			// la valeur spéculaire de l'éclairage 
			XMVECTOR vSMat; 			// la valeur spéculaire du matériau 
			float puissance;			// la puissance de spécularité
			int bTex;					// Texture ou materiau 
			XMFLOAT2 remplissage;

		};

		// ****  Données membres
		XMMATRIX matWorld;				// Matrice de transformation dans le monde
		float rotation;

		// Pour le dessin
		CDispositifD3D11* pDispositif;		// On prend en note le dispositif

		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;

		// Les sous-objets
		int NombreSubmesh;				    // Nombre de sous-objets dans le mesh
		vector<int> SubmeshMaterialIndex;	// Index des matériaux
		vector<int> SubmeshIndex;			// Index des sous-objets
		vector<CMaterial> Material;			// Vecteur des matériaux

		// Pour les effets et shaders
		ID3D11SamplerState* pSampleState;
		ID3D11Buffer* pConstantBuffer;
		ID3DX11Effect* pEffet;
		ID3DX11EffectTechnique* pTechnique;
		ID3DX11EffectPass* pPasse;
		ID3D11InputLayout* pVertexLayout;

	};
}


