/*#pragma once
#include "Mesh.h"
#include "..\PetitMoteur3D\PetitMoteur3D\dispositifD3D11.h"
#include "..\PetitMoteur3D\PetitMoteur3D\d3dx11effect.h"
#include "..\PetitMoteur3D\PetitMoteur3D\chargeur.h"

#include <vector>
#include <DirectXMath.h>
#include "..\PetitMoteur3D\PetitMoteur3D\ObjetMesh.h"

using namespace std;

class PlaneMesh : public PM3D::CObjetMesh
{
	PlaneMesh(unsigned int xMin, unsigned int xMax, unsigned int zMin, unsigned int zMax, float y, PM3D::CDispositifD3D11* _pDispositif):
		Mesh<ShaderCObjectMesh::ShadersParams>(shaderParameter)
	{
		// prendre en note le dispositif
		pDispositif = _pDispositif;

		// Placer l'objet sur la carte graphique
		TransfertObjet(chargeur);

		// Initialisation de l'effet
		InitEffet();
	}
};

*/
