#ifndef PIECEMESH_H_INCLUDED
#define PIECEMESH_H_INCLUDED



#include "Mesh.h"
#include "Material.h"
#include "Effect.h"
#include "..\PetitMoteur3D\PetitMoteur3D\dispositifD3D11.h"

#include <vector>
#include <DirectXMath.h>


namespace PM3D
{
    class IChargeur;
}

namespace PirateSimulator
{
    namespace ShaderPieceMesh
    {
        struct ShadersParams // toujours un multiple de 16 pour les constantes
        {
            DirectX::XMMATRIX matWorldViewProj;	// la matrice totale 
            DirectX::XMMATRIX matWorld;			// la matrice de transformation dans le monde 
            DirectX::XMVECTOR vLumiere; 			// la position de la source d'éclairage (Point)
            DirectX::XMVECTOR vCamera; 			// la position de la caméra
            DirectX::XMVECTOR vAEcl; 			// la valeur ambiante de l'éclairage
            DirectX::XMVECTOR vAMat; 			// la valeur ambiante du matériau
            DirectX::XMVECTOR vDEcl; 			// la valeur diffuse de l'éclairage 
            DirectX::XMVECTOR vDMat; 			// la valeur diffuse du matériau 
            

            ShadersParams()
            {
                vLumiere =  DirectX::XMVectorSet(-10.0f, 10.0f, -15.0f, 1.0f);
                vCamera =   DirectX::XMVectorSet(0.0f, 3.0f, -5.0f, 1.0f);
                vAEcl =     DirectX::XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
                vDEcl =     DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
            }

            ShadersParams(
                const DirectX::XMVECTOR& lumiere,
                const DirectX::XMVECTOR& camera,
                const DirectX::XMVECTOR& AEcl,
                const DirectX::XMVECTOR& AMat
            )
            {
                vLumiere = lumiere;
                vCamera = camera;
                vAEcl = AEcl;
                vDEcl = AMat;
            }
        };
    }

    class PieceMesh : public PirateSimulator::Mesh<ShaderPieceMesh::ShadersParams>
    {
    protected:
        // Pour le dessin
        PM3D::CDispositifD3D11* pDispositif;		// On prend en note le dispositif

        ID3D11Buffer* pVertexBuffer;
        ID3D11Buffer* pIndexBuffer;
        ID3D11InputLayout* pVertexLayout;

        // Les sous-objets
        int NombreSubset;				// Nombre de sous-objets dans le mesh
        std::vector<int> SubsetMaterialIndex;// Index des matériaux
        std::vector<int> SubsetIndex;		// Index des sous-objets

        std::vector<Material> m_materials;		// Vecteur des matériaux

                                                // Pour les effets et shaders
        Effect m_effect;


    public:
        PieceMesh(const ShaderPieceMesh::ShadersParams& shaderParameter, const std::wstring& shaderName, PM3D::IChargeur& chargeur);
        //CObjetMesh(string nomfichier, const ShaderCObjectMesh::ShadersParams& shaderParameter, IChargeur& chargeur);
        PieceMesh(const std::string& nomfichier, const std::wstring& shaderName, const ShaderPieceMesh::ShadersParams& shaderParameter);

        virtual ~PieceMesh();


    public:
        virtual void Draw();


    protected:
        void TransfertObjet(PM3D::IChargeur& chargeur);
        void InitEffet(const std::wstring& shaderName);
        //void EcrireFichierBinaire(IChargeur& chargeur, const string& nomFichier);
        void LireFichierBinaire(const std::string& nomFichier);

    };
}

#endif // !PIECEMESH_H_INCLUDED