#ifndef TERRAIN_H
#define TERRAIN_H

#include "Vertex.h"
#include "Triangle.h"
#include "../Chapitre 10/PetitMoteur3D/Objet3D.h"
#include "../Chapitre 10/PetitMoteur3D/sommetbloc.h"
#include "../Chapitre 10/PetitMoteur3D/DispositifD3D11.h"

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
    class Terrain : public PM3D::CObjet3D
    {
        std::vector<CSommetBloc> m_sommets;
        std::vector<unsigned int> m_index_bloc;
    public:
        Terrain(PM3D::CDispositifD3D11* pDispositif);

        // Destructeur
        virtual ~Terrain(void);

        virtual void Anime(float tempsEcoule);

        void Draw();

        void Init();
        
        void addSommet(Vertex v);
        void addTriangle(Triangle t);

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
		ID3D11ShaderResourceView* pTextureD3D;
    };

}

#endif // TERRAIN_H