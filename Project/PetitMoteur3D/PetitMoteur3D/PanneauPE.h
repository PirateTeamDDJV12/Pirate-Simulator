#pragma once
#include "dispositifD3D11.h"
#include "d3dx11effect.h"


namespace PM3D
{
    class CSommetPanneauPE
    {
    public:
        CSommetPanneauPE() {};
        CSommetPanneauPE(DirectX::XMFLOAT3 _position, DirectX::XMFLOAT2 _coordTex)
        {
            position = _position; coordTex = _coordTex;
        }

    public:
        static UINT numElements;
        static D3D11_INPUT_ELEMENT_DESC layout[];
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 coordTex;
    };

    // Classe : CPanneauPE
    // BUT : Classe pour les post-effects
    class CPanneauPE
    {
    private:
        struct ShaderParameter
        {
            float darkening; // darkening effect to darken the final picture -> must be between 0.f and 1.f

            DirectX::XMFLOAT3 uselessFill;
        };


    public:
        static constexpr const float DARKENING_COEFFICIENT_VALUE = 1.0f;


    public:
        CPanneauPE(CDispositifD3D11* pDispositif_in);
        ~CPanneauPE(void);
        void Draw();
        void DebutPostEffect();
        void FinPostEffect();

    protected:
        // Constructeur par défaut
        CPanneauPE() {}
        virtual void InitEffet();

    protected:
        static CSommetPanneauPE sommets[6];
        ID3D11Buffer* pVertexBuffer;
        CDispositifD3D11* pDispositif;

        // Pour les effets
        ID3DX11Effect* pEffet;
        ID3DX11EffectTechnique* pTechnique;
        ID3DX11EffectPass* pPasse;
        ID3D11InputLayout* pVertexLayout;
        ID3D11SamplerState* pSampleState;

        // Texture de rendu pour effets
        ID3D11Texture2D* pTextureScene;
        ID3D11RenderTargetView* pRenderTargetView;
        ID3D11ShaderResourceView* pResourceView;
        ID3D11Texture2D* pDepthTexture;
        ID3D11DepthStencilView* pDepthStencilView;

        ID3D11RenderTargetView* pOldRenderTargetView;
        ID3D11DepthStencilView* pOldDepthStencilView;

        ID3D11Buffer* pConstantBuffer;
        ShaderParameter m_shaderParams;
    };
}