#pragma once

#include <vector>
using namespace std;
using namespace DirectX;

namespace PM3D
{

class CParametresChargement
{
public:
	CParametresChargement()
	{
		bInverserCulling = false;
		bMainGauche = false;
	}
	
	string NomFichier; 
	string NomChemin;
	bool bInverserCulling;
	bool bMainGauche;
};

class IChargeur
{
public:
	IChargeur(){}
	virtual ~IChargeur(void){}

	virtual void Chargement( const CParametresChargement& param) = 0; 

	virtual int	 GetNombreSommets() = 0;
	virtual int  GetNombreIndex() = 0;
	virtual void* GetIndexData() = 0;
	virtual int  GetNombreSubset() = 0;
	virtual int  GetNombreMaterial() = 0;
	virtual void GetMaterial(int _i,
					string& _NomFichierTexture,
					string& _NomMateriau,
					XMFLOAT4& _Ambient,
					XMFLOAT4& _Diffuse,
					XMFLOAT4& _Specular,
					float& _Puissance) = 0;

	virtual string GetMaterialName(int i) = 0;

	virtual void CopieSubsetIndex(vector<int>& dest) = 0;

	virtual XMFLOAT3 GetPosition(int NoSommet) = 0;
	virtual XMFLOAT2 GetCoordTex(int NoSommet) = 0;
	virtual XMFLOAT3 GetNormale(int NoSommet) = 0;

};

}	