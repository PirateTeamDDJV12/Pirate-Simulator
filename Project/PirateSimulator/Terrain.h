#ifndef TERRAIN_H
#define TERRAIN_H

#include "Material.h"
#include "Effect.h"
#include "Vertex.h"
#include "Triangle.h"
#include "Mesh.h"
#include "../PetitMoteur3D/PetitMoteur3D/sommetbloc.h"
#include "../PetitMoteur3D/PetitMoteur3D/DispositifD3D11.h"
#include "../PetitMoteur3D/PetitMoteur3D/d3dx11effect.h"

#include <vector>
#include <string>

namespace PM3D
{
	class CTexture;
}

namespace PirateSimulator
{
    namespace ShaderTerrain
    {
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

            float puissance;
            int bTex;					// Texture ou materiau 
            XMFLOAT2 remplissage;


            ShadersParams() :
                bTex{1}
            {
                vLumiere = XMVectorSet(130.0f, 130.0f, -50.0f, 1.0f);
                vAEcl = XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
                vAMat = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
                vDEcl = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
                vDMat = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
            }
            
            ShadersParams(
                    const XMVECTOR& lumiere,
                    const XMVECTOR& AEcl,
                    const XMVECTOR& AMat,
                    const XMVECTOR& DEcl,
                    const XMVECTOR& DMat
                ) :
                bTex{ 1 }
            {
                vLumiere = lumiere;
                vAEcl = AEcl;
                vAMat = AMat;
                vDEcl = DEcl;
                vDMat = DMat;
            }

            ShadersParams(
                const XMVECTOR& lumiere,
                const XMVECTOR& AEcl,
                const XMVECTOR& AMat,
                const XMVECTOR& DEcl,
                const XMVECTOR& DMat,
                const XMVECTOR& SEcl,
                const XMVECTOR& SMat
            ) :
                bTex{ 1 }
            {
                vLumiere = lumiere;
                vAEcl = AEcl;
                vAMat = AMat;
                vDEcl = DEcl;
                vDMat = DMat;
                vSEcl = SEcl;
                vSMat = SMat;
            }
        };
    }
    

    //  Classe : Terrain
    //
    //  BUT : 	Classe du terrain de notre jeu
    //
    class Terrain : public Mesh<ShaderTerrain::ShadersParams>
    {
    public:
        static D3D11_INPUT_ELEMENT_DESC Terrain::layout[];
        static UINT numElements;

    private:
        int m_terrainWidth;
        int m_terrainHeight;
        std::vector<std::vector<Vertex>> m_arraySommets;
        std::vector<CSommetBloc> m_sommets;
        std::vector<unsigned int> m_index_bloc;

    public:
        Terrain(PM3D::CDispositifD3D11* pDispositif);
        Terrain(PM3D::CDispositifD3D11* pDispositif, int h, int w, const std::string& fieldFileName, const std::string& textureFileName);

        // Destructeur
        virtual ~Terrain(void);

        virtual void Anime(float tempsEcoule);

        void Draw();

        void Init(const std::string& textureFileName);

        void addSommet(Vertex v);
        void addTriangle(Triangle t);
        float getHeight(XMVECTOR pos);

		void setTexture(PM3D::CTexture* texture);

		void loadTexture(const std::string& filename);

    protected:
        // Constructeur par défaut
        Terrain(void) :
            Mesh<ShaderTerrain::ShadersParams>(ShaderTerrain::ShadersParams())
        {}

        void InitShaders();

        PM3D::CDispositifD3D11* pDispositif;

        ID3D11Buffer* pVertexBuffer;
        ID3D11Buffer* pIndexBuffer;

        ID3D11VertexShader*  pVertexShader;
        ID3D11PixelShader*  pPixelShader;
        ID3D11InputLayout* pVertexLayout;

        // Définitions des valeurs d'animation
        ID3D11Buffer* pConstantBuffer;
        float rotation;

		//Pour texture
        Material m_material;

        Effect m_textureEffect;
    };

}

#endif // TERRAIN_H