#pragma once

#include <vector>
#include "chargeur.h"

using namespace std;
using namespace DirectX;

namespace PM3D
{

class CChargeurOBJ: public IChargeur
{
protected:
	class OBJVertexInfo
	{
	public:
		OBJVertexInfo() { PositionIndex = 0; TextCoordIndex = 0; NormalIndex = 0; }
		int PositionIndex;
		int TextCoordIndex;
		int NormalIndex;
	};

	class OBJMaterial
	{
	public:
		string NomFichierTexture;
		string NomMateriau;

		XMFLOAT4 Ambient;
		XMFLOAT4 Diffuse;
		XMFLOAT4 Specular;
		float Puissance;
		bool Transparent;

		OBJMaterial()
		{
			Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
			Transparent = false;
			Puissance = 1;
			NomFichierTexture = "";
		}
	};


public:
	CChargeurOBJ();

	void Chargement( const CParametresChargement& param );
	
	virtual ~CChargeurOBJ(void);

	int GetNombreSommets(){ return tabVertexInfo.size();}
	int GetNombreIndex(){ return tabIndex.size();}
	void* GetIndexData(){ return tabIndex.data();}
	int GetNombreSubset(){ return NombreSubset; }
	int GetNombreMaterial(){ return material.size();}
	void GetMaterial(int _i,
					string& _NomFichierTexture,
					string& _NomMateriau,
					XMFLOAT4& _Ambient,
					XMFLOAT4& _Diffuse,
					XMFLOAT4& _Specular,
					float& _Puissance);

	string GetMaterialName(int i){ return materialName[i]; };

	void CopieSubsetIndex(vector<int>& dest){ dest = SubsetIndex;}

	XMFLOAT3 GetPosition(int NoSommet) { return Position[tabVertexInfo[NoSommet].PositionIndex];}
	XMFLOAT2 GetCoordTex(int NoSommet) { return CoordTex[tabVertexInfo[NoSommet].TextCoordIndex];}
	XMFLOAT3 GetNormale(int NoSommet) { return Normale[tabVertexInfo[NoSommet].NormalIndex];}

protected:
	// FONCTIONS
	bool LireFichier(const string& nomFichier);
	bool LireFichierMateriel();
	void TraiterLigneOBJ(const string& str);
	void TraiterLigneMAT(const string& str);
	int TrouverVertex(const OBJVertexInfo& vertex);

protected:
	string cheminModele;

	// *** Les sommets 
	vector<XMFLOAT3> Position;				// Tableau des positions ( v )
	vector<XMFLOAT2> CoordTex;				// Tableau des coordonnées de texture ( vt )
	vector<XMFLOAT3> Normale;				// Tableau des normales ( vn )

	vector<OBJVertexInfo> tabVertexInfo;	// Sommets par index  no/no/no
	vector<int> tabIndex;					// Index pour les polygones (triangles)

	// *** Les sous-objets
	vector<int> SubsetIndex;			// index
	int NombreSubset;					// nombre de sous objets


	// *** Indicateurs
	bool bCoordTex;   // Il y a des coordonnées de texture
	bool bNormales;	  // Il y a des normales
	bool bInverserCulling;
	bool bMainGauche;

	// *** Materiaux - Dans le fichier OBJ
	string OBJMaterialLib;			// Nom de la bibliotheque materiaux
	int NombreMateriaux;			// Nombre de materiaux
	vector<string> materialName;	// Nom des materiaux

	// *** Materiaux - Dans le fichier MTL
	vector<OBJMaterial> material;



};

}	