#ifndef TUNNELMESH_H_INCLUDED
#define TUNNELMESH_H_INCLUDED

#include "Material.h"
#include "Effect.h"
#include "..\PetitMoteur3D\PetitMoteur3D\ObjetMesh.h"
#include "..\PetitMoteur3D\PetitMoteur3D\Texture.h"
#include "..\PetitMoteur3D\PetitMoteur3D\sommetbloc.h"
#include "terrain.h"
#include "Plane.h"


namespace PM3D
{
    class IChargeur;
}

namespace PirateSimulator
{
    class TunnelMesh : public Mesh<ShaderPlane::ShadersParams>
    {
    private:
        PM3D::CDispositifD3D11* pDispositif;		// On prend en note le dispositif

        ID3D11Buffer* pVertexBuffer;
        ID3D11Buffer* pIndexBuffer;
        ID3D11InputLayout* pVertexLayout;

        // Les sous-objets
        int NombreSubset;				// Nombre de sous-objets dans le mesh
        vector<int> SubsetMaterialIndex;// Index des matériaux
        vector<int> SubsetIndex;		// Index des sous-objets

        vector<PM3D::CObjetMesh::CMaterial> m_materials;		// Vecteur des matériaux

        Effect m_textureEffect;


    public:
        TunnelMesh(const ShaderPlane::ShadersParams& shaderParameter,
               const std::wstring& shaderName, 
               PM3D::IChargeur& chargeur
        );


    public:
        virtual void Draw();


    protected:
        void TransfertObjet(PM3D::IChargeur& chargeur);
        void InitEffet(const std::wstring& shaderName);
    };
}

#endif // !TUNNELMESH_H_INCLUDED
