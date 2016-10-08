#pragma once
#include "chargeur.h"
#include <Importer.hpp>      // C++ importer interface
#include <scene.h>           // Output data structure

namespace PM3D
{

class CChargeurAssimp :
	public IChargeur
{
protected:
	// Objet de classe Importer (Assimp)
	Assimp::Importer* pImporter;
	const aiScene* scene;

public:
	CChargeurAssimp();
	~CChargeurAssimp();

	virtual void Chargement(const CParametresChargement& param);

	virtual int  GetNombreSubmesh();
	virtual int	 GetNombreSommetsSubmesh(unsigned int n);
	virtual XMFLOAT3 GetPosition(int noMesh, int NoSommet);
	virtual XMFLOAT2 GetCoordTex(int noMesh, int NoSommet);
	virtual XMFLOAT3 GetNormale(int noMesh, int NoSommet);
	virtual unsigned int GetIndice(int noMesh, int noPoly, int NoIndice);
	virtual int  GetNombrePolygonesSubmesh(int noMesh);

	virtual int  GetNombreMaterial();

	virtual void GetMaterial(int _i,
		string& _NomFichierTexture,
		string& _NomMateriau,
		XMFLOAT4& _Ambient,
		XMFLOAT4& _Diffuse,
		XMFLOAT4& _Specular,
		float& _Puissance);

	virtual int GetMaterialIndex(int i);

	// À VERIFIER

	





};

}