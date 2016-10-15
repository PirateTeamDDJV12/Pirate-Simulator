#ifndef TERRAIN_H
#define TERRAIN_H
#include "../Objet3D.h"
#include "../d3dx11effect.h"
#include "../sommetbloc.h"
#include <vector>
#include "Vertex.h"
#include "Triangle.h"

namespace PM3D
{
    class CDispositifD3D11;


    //  Classe : Terrain
    //
    //  BUT : 	Classe du terrain de notre jeu
    //
    class Terrain : public CObjet3D
    {
        std::vector<CSommetBloc> m_sommets;
        std::vector<unsigned int> m_index_bloc;
    public:
        Terrain(CDispositifD3D11* pDispositif);

        // Destructeur
        virtual ~Terrain(void);

        virtual void Anime(float tempsEcoule);

        void Draw();

        void Init();
        
        void addSommet(PirateSimulator::Vertex v);
        void addTriangle(PirateSimulator::Triangle t);

    protected:
        // Constructeur par défaut
        Terrain(void)
        {}

        void InitShaders();

        CDispositifD3D11* pDispositif;

        ID3D11Buffer* pVertexBuffer;
        ID3D11Buffer* pIndexBuffer;

        ID3D11VertexShader*  pVertexShader;
        ID3D11PixelShader*  pPixelShader;
        ID3D11InputLayout* pVertexLayout;

        // Définitions des valeurs d'animation
        ID3D11Buffer* pConstantBuffer;
        XMMATRIX matWorld;
        float rotation;

    };

}

#endif // TERRAIN_H