#ifndef EFFECT_H
#define EFFECT_H


#include "Mesh.h"
#include "../PetitMoteur3D/PetitMoteur3D/dispositifD3D11.h"
#include "../PetitMoteur3D/PetitMoteur3D/d3dx11effect.h"



namespace PirateSimulator
{
    class Effect
    {
    public:
        ID3D11SamplerState* m_sampleState;
        ID3D11Buffer* m_constantBuffer;
        ID3DX11Effect* m_effect;
        ID3DX11EffectTechnique* m_technique;
        ID3DX11EffectPass* m_pass;
        ID3D11InputLayout* m_vertexLayout;


    public:
        Effect() :
            m_sampleState{ nullptr },
            m_constantBuffer{ nullptr },
            m_effect{ nullptr },
            m_technique{ nullptr },
            m_pass{ nullptr },
            m_vertexLayout{ nullptr }
        {}

        ~Effect()
        {
            if (m_sampleState)    m_sampleState->Release();
            if (m_constantBuffer) m_constantBuffer->Release();
            if (m_effect)         m_effect->Release();
            if (m_technique)      m_technique->Release();
            if (m_pass)           m_pass->Release();
            if (m_vertexLayout)   m_vertexLayout->Release();
        }
    };
}


#endif //!EFFECT_H