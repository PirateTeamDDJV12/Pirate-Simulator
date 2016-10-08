#include "stdafx.h"

// Pour ASSIMP

#include <postprocess.h>     // Post processing flag

#include "ChargeurAssimp.h"

namespace PM3D
{

CChargeurAssimp::CChargeurAssimp()
{
	pImporter = nullptr;
}

void CChargeurAssimp::Chargement(const CParametresChargement& param)
{
	// Create an instance of the Importer class
	pImporter = new Assimp::Importer;

	parametres = param;
	string Fichier = param.NomChemin + param.NomFichier;

	int flags = 0;

	flags = aiProcess_CalcTangentSpace | aiProcess_FlipUVs |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType;

	if (param.bMainGauche) flags |= aiProcess_MakeLeftHanded;
	if (param.bInverserCulling) flags |= aiProcess_FlipWindingOrder;

	
	scene = pImporter->ReadFile(Fichier, flags);

	
}


CChargeurAssimp::~CChargeurAssimp()
{
}

int  CChargeurAssimp::GetNombreSubmesh()
{ 
	return scene->mNumMeshes;
}

int	 CChargeurAssimp::GetNombreSommetsSubmesh(unsigned int n)
{ 
	return scene->mMeshes[n]->mNumVertices; 
}

XMFLOAT3 CChargeurAssimp::GetPosition(int noMesh, int NoSommet)
{
	aiVector3D v = scene->mMeshes[noMesh]->mVertices[NoSommet];
	XMFLOAT3 r(v.x, v.y, v.z);

	return r;
}

XMFLOAT2 CChargeurAssimp::GetCoordTex(int noMesh, int NoSommet)
{
	scene->mMeshes[noMesh]->mTextureCoords[NoSommet]->x;
	XMFLOAT2 r( scene->mMeshes[noMesh]->mTextureCoords[0][NoSommet].x,
				scene->mMeshes[noMesh]->mTextureCoords[0][NoSommet].y);

	return r;
}

XMFLOAT3 CChargeurAssimp::GetNormale(int noMesh, int NoSommet)
{
	aiVector3D v = scene->mMeshes[noMesh]->mNormals[NoSommet];
	XMFLOAT3 r(v.x, v.y, v.z);

	return r;
}

unsigned int CChargeurAssimp::GetIndice(int noMesh, int noPoly, int NoIndice)
{
	
	return scene->mMeshes[noMesh]->mFaces[noPoly].mIndices[NoIndice];
}

int  CChargeurAssimp::GetNombrePolygonesSubmesh(int noMesh)
{
	return scene->mMeshes[noMesh]->mNumFaces;
}

int  CChargeurAssimp::GetNombreMaterial()
{
	return scene->mNumMaterials;
}

void CChargeurAssimp::GetMaterial(int _i,
	string& _NomFichierTexture,
	string& _NomMateriau,
	XMFLOAT4& _Ambient,
	XMFLOAT4& _Diffuse,
	XMFLOAT4& _Specular,
	float& _Puissance)
{
	aiString name;

	scene->mMaterials[_i]->Get(AI_MATKEY_NAME, name);
	_NomMateriau = string(name.C_Str());

	scene->mMaterials[_i]->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), name);
	_NomFichierTexture = string(name.C_Str());
	if (_NomFichierTexture == _NomMateriau) _NomFichierTexture = "";
	else _NomFichierTexture = parametres.NomChemin + _NomFichierTexture;

	aiColor3D color(0.f, 0.f, 0.f); 
	scene->mMaterials[_i]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	_Diffuse = XMFLOAT4(color.r, color.g, color.b, 1.0f);
	scene->mMaterials[_i]->Get(AI_MATKEY_COLOR_AMBIENT, color);
	_Ambient = XMFLOAT4(color.r, color.g, color.b, 1.0f);
	scene->mMaterials[_i]->Get(AI_MATKEY_COLOR_SPECULAR, color);
	_Specular = XMFLOAT4(color.r, color.g, color.b, 1.0f);
	float puissance;
	scene->mMaterials[_i]->Get(AI_MATKEY_SHININESS, puissance);
	_Puissance = puissance;

}

int CChargeurAssimp::GetMaterialIndex(int i)
{ 	
	return scene->mMeshes[i]->mMaterialIndex;
};


}
