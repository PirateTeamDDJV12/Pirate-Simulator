#ifndef TERRAIN_H
#define TERRAIN_H
#include "../Objet3D.h"
#include "../d3dx11effect.h"

namespace PM3D
{
    class CDispositifD3D11;


    //  Classe : Terrain
    //
    //  BUT : 	Classe du terrain de notre jeu
    //
    class Terrain : public CObjet3D
    {
    public:
        Terrain(const float dx, const float dy,
                    CDispositifD3D11* pDispositif);

        // Destructeur
        virtual ~Terrain(void);

        virtual void Anime(float tempsEcoule);

        void Draw();

    protected:
        // Constructeur par défaut
        Terrain(void)
        {}

        void InitEffet();

        CDispositifD3D11* pDispositif;

        ID3D11Buffer* pVertexBuffer;
        ID3D11Buffer* pIndexBuffer;

        // Définitions des valeurs d'animation
        ID3D11Buffer* pConstantBuffer;
        XMMATRIX matWorld;
        float rotation;

        // Pour les effets
        ID3DX11Effect* pEffet;
        ID3DX11EffectTechnique* pTechnique;
        ID3DX11EffectPass* pPasse;
        ID3D11InputLayout* pVertexLayout;

    };

}

#endif // TERRAIN_H