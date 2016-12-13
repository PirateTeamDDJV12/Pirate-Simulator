#pragma once
#include <string>
#include "d3dx11effect.h"
#include "../../PirateSimulator/Mesh.h"
#include "DispositifD3D11.h"
#include "Texture.h"
#include <vector>

using namespace std;

namespace PM3D
{

    class CSommetSprite
    {
    public:
        CSommetSprite() {};
        CSommetSprite(DirectX::XMFLOAT3 _position, DirectX::XMFLOAT2 _coordTex)
        {
            position = _position; coordTex = _coordTex;
        }

    public:
        static UINT numElements;
        static D3D11_INPUT_ELEMENT_DESC layout[];

        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 coordTex;
    };

    namespace ShaderCAfficheurSprite
    {
        struct ShadersParams
        {
            DirectX::XMMATRIX matWVP;	// la matrice totale 
        };
    }

    class CAfficheurSprite : public PirateSimulator::Mesh<ShaderCAfficheurSprite::ShadersParams>
    {
    public:
        CAfficheurSprite();
        virtual ~CAfficheurSprite(void);
        virtual void Anime(float tempsEcoule) {};
        virtual	void Draw();

        void AjouterSprite(string NomTexture, int _x, int _y, int _dx = 0, int _dy = 0);
        void AjouterPanneau(string NomTexture, const DirectX::XMFLOAT3& _position,
                            float _dx = 0.0f, float _dy = 0.0f);
        void AjouterSpriteTexte(ID3D11ShaderResourceView* pTexture, int _x, int _y);

    protected:
        class CSprite
        {
        public:
            ID3D11ShaderResourceView* pTextureD3D;

            DirectX::XMMATRIX matPosDim;
            bool bPanneau;
            CSprite()
            {
                bPanneau = false;
            }
        };

        class CPanneau : public CSprite
        {
        public:
            DirectX::XMFLOAT3 position;
            DirectX::XMFLOAT2 dimension;

            CPanneau()
            {
                bPanneau = true;
            }
        };

    protected:
        static CSommetSprite sommets[6];
        ID3D11Buffer* pVertexBuffer;
        CDispositifD3D11* pDispositif;

        ID3D11Buffer* pConstantBuffer;
        ID3DX11Effect* pEffet;
        ID3DX11EffectTechnique* pTechnique;
        ID3DX11EffectPass* pPasse;
        ID3D11InputLayout* pVertexLayout;

        ID3D11SamplerState* pSampleState;

        // Tous nos sprites
        vector<CSprite*> tabSprites;




    protected:
        virtual	void InitEffet();



    };

}

