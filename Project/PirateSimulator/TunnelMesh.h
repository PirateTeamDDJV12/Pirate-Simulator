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
    namespace ShaderTunnel
    {
        struct ShadersParams
        {
        public:
            DirectX::XMMATRIX matWorldViewProj;	// la matrice totale 
            DirectX::XMMATRIX matWorld;			// matrice de transformation dans le monde 

            DirectX::XMVECTOR vLumiere; 			// la position de la source d'éclairage (Point)
            DirectX::XMVECTOR vCamera; 			// la position de la caméra

            DirectX::XMVECTOR vLightPoint1;
            DirectX::XMVECTOR vLightPoint2;
            DirectX::XMVECTOR vLightPoint3;
            DirectX::XMVECTOR vLightPoint4;
            DirectX::XMVECTOR mappedLightPointPower1;

            DirectX::XMVECTOR vLightPoint5;
            DirectX::XMVECTOR vLightPoint6;
            DirectX::XMVECTOR vLightPoint7;
            DirectX::XMVECTOR vLightPoint8;
            DirectX::XMVECTOR mappedLightPointPower2;

            DirectX::XMVECTOR vAEcl; 			// la valeur ambiante de l'éclairage
            DirectX::XMVECTOR vAMat; 			// la valeur ambiante du matériau
            DirectX::XMVECTOR vDEcl; 			// la valeur diffuse de l'éclairage 
            DirectX::XMVECTOR vDMat; 			// la valeur diffuse du matériau 
            DirectX::XMVECTOR vSMat; 			// la valeur spéculaire du matériau 

            float puissance;
            float sunPower;

            DirectX::XMFLOAT2 uselessFill;


        public:
            ShadersParams() :
                puissance{ 1.f }
            {
                vLumiere = DirectX::XMVectorSet(130.0f, 130.0f, -50.0f, 1.0f);
                vAEcl = DirectX::XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
                vAMat = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
                vDEcl = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
                vDMat = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
            }

            ShadersParams(
                const DirectX::XMVECTOR& lumiere,
                const DirectX::XMVECTOR& AEcl,
                const DirectX::XMVECTOR& AMat,
                const DirectX::XMVECTOR& DEcl,
                const DirectX::XMVECTOR& DMat
            ) :
                puissance{ 1.f }
            {
                vLumiere = lumiere;
                vAEcl = AEcl;
                vAMat = AMat;
                vDEcl = DEcl;
                vDMat = DMat;
            }

            ShadersParams(
                const DirectX::XMVECTOR& lumiere,
                const DirectX::XMVECTOR& AEcl,
                const DirectX::XMVECTOR& AMat,
                const DirectX::XMVECTOR& DEcl,
                const DirectX::XMVECTOR& DMat,
                const DirectX::XMVECTOR& SEcl,
                const DirectX::XMVECTOR& SMat
            )
            {
                vLumiere = lumiere;
                vAEcl = AEcl;
                vAMat = AMat;
                vDEcl = DEcl;
                vDMat = DMat;
                vSMat = SMat;
            }
        };
    }

    class TunnelMesh : public Mesh<ShaderTunnel::ShadersParams>
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
        TunnelMesh(const ShaderTunnel::ShadersParams& shaderParameter,
               const std::wstring& shaderName, 
               PM3D::IChargeur& chargeur
        );


    public:
        virtual void Draw();


    protected:
        void TransfertObjet(PM3D::IChargeur& chargeur);
        void InitEffet(const std::wstring& shaderName);
        void InitShaderParameter();
        void updateShaderParameter();
    };
}

#endif // !TUNNELMESH_H_INCLUDED
