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
#include "TunnelAndFieldShaderParameter.h"

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
    class Terrain : public Mesh<ShaderTunnelAndField::ShadersParams>
    {
    public:
        static D3D11_INPUT_ELEMENT_DESC Terrain::layout[];
        static UINT numElements;

    protected:
        PM3D::CDispositifD3D11* pDispositif;

        ID3D11Buffer* pVertexBuffer;
        ID3D11Buffer* pIndexBuffer;

        ID3D11InputLayout* pVertexLayout;

        // D�finitions des valeurs d'animation
        ID3D11Buffer* pConstantBuffer;

        //Pour texture
        Material m_material;

        Effect m_textureEffect;

    private:
        int m_terrainWidth;
        int m_terrainHeight;
        float m_terrainScale;
        std::vector<Vertex> m_vertexArray;
        std::vector<CSommetBloc> m_csommetsArray;
        std::vector<unsigned int> m_index_bloc;

    public:
        // Get the configuration from the config file
        Terrain();
        // Override the configuration file
        Terrain(int h, int w, float s, const std::string& fieldFileName, const std::string& textureFileName);

        // Destructeur
        virtual ~Terrain();

    public:
        virtual void Anime(float tempsEcoule);

        void Draw();

        void Init(const std::string& textureFileName);

        void addSommet(Vertex v);
        void addTriangle(Triangle t);
        float getHeight(XMVECTOR pos);
        
        float getScale()
        {
            return m_terrainScale;
        }
        int getWidth()
        {
            return m_terrainWidth;
        }

        int getHeight()
        {
            return m_terrainHeight;
        }
        std::vector<CSommetBloc>& getSommetsArray()
        {
            return std::move(m_csommetsArray);
        }
        std::vector<unsigned int>& getIndex()
        {
            return std::move(m_index_bloc);
        }


		void setTexture(PM3D::CTexture* texture);

		void loadTexture(const std::string& filename);

    protected:
        // Constructeur par d�faut
        void InitShaders();
        void InitShaderParameter();
        void updateShaderParameter();
    };

}

#endif // TERRAIN_H