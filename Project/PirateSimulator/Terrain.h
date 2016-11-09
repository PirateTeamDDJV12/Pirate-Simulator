#ifndef TERRAIN_H
#define TERRAIN_H

#include "Material.h"
#include "Effect.h"
#include "Vertex.h"
#include "Triangle.h"
#include "Mesh.h"
#include "../Chapitre 10/PetitMoteur3D/sommetbloc.h"
#include "../Chapitre 10/PetitMoteur3D/DispositifD3D11.h"
#include "../Chapitre 10/PetitMoteur3D/d3dx11effect.h"

#include <vector>
#include <string>

namespace PM3D
{
	class CTexture;
}

namespace PirateSimulator
{
    //  Classe : Terrain
    //
    //  BUT : 	Classe du terrain de notre jeu
    //
    class Terrain : public Mesh
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
        Terrain(void)
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
        XMMATRIX matWorld;
        float rotation;

		//Pour texture
        Material m_material;

        Effect m_textureEffect;
    };

}

#endif // TERRAIN_H