#ifndef EFFECT_H
#define EFFECT_H


#include "Mesh.h"
#include "../PetitMoteur3D/PetitMoteur3D/dispositifD3D11.h"
#include "../PetitMoteur3D/PetitMoteur3D/d3dx11effect.h"

#include "../PetitMoteur3D/PetitMoteur3D/util.h"
#include "../PetitMoteur3D/PetitMoteur3D/resource.h"


namespace PirateSimulator
{
    // This class is for shaders written in one file called effects files (.fx) this file combines vertex shader and the pixel shader codes in one file
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
        Effect(const std::string &path, PM3D::CDispositifD3D11 *dispositif) :
            m_sampleState{ nullptr },
            m_constantBuffer{ nullptr },
            m_effect{ nullptr },
            m_technique{ nullptr },
            m_pass{ nullptr },
            m_vertexLayout{ nullptr }
        {
            
            ID3D11Device* pD3DDevice = dispositif->GetD3DDevice();

            // Pour l'effet
            ID3DBlob* pFXBlob = NULL;

            // Creation du shader
            UtilitairesDX::DXEssayer(D3DCompileFromFile(L"MiniPhong.fx", 0, 0, 0,
                "fx_5_0", 0, 0, &pFXBlob, 0),
                DXE_ERREURCREATION_FX);

            D3DX11CreateEffectFromMemory(pFXBlob->GetBufferPointer(), pFXBlob->GetBufferSize(), 0, pD3DDevice, &m_effect);

            pFXBlob->Release();

            m_technique = m_effect->GetTechniqueByIndex(0);
            m_pass = m_technique->GetPassByIndex(0);

            //// Créer l'organisation des sommets pour le VS de notre effet
            //D3DX11_PASS_SHADER_DESC effectVSDesc;
            //m_pass->GetVertexShaderDesc(&effectVSDesc);

            //D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
            //effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex, &effectVSDesc2);

            //const void *vsCodePtr = effectVSDesc2.pBytecode;
            //unsigned vsCodeLen = effectVSDesc2.BytecodeLength;

            //pVertexLayout = NULL;

            //UtilitairesDX::DXEssayer(pD3DDevice->CreateInputLayout(Terrain::layout,
            //    Terrain::numElements,
            //    vsCodePtr,
            //    vsCodeLen,
            //    &pVertexLayout),
            //    DXE_CREATIONLAYOUT);

            // Initialisation des paramètres de sampling de la texture
            D3D11_SAMPLER_DESC samplerDesc;

            samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
            samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.MipLODBias = 0.0f;
            samplerDesc.MaxAnisotropy = 4;
            samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
            samplerDesc.BorderColor[0] = 0;
            samplerDesc.BorderColor[1] = 0;
            samplerDesc.BorderColor[2] = 0;
            samplerDesc.BorderColor[3] = 0;
            samplerDesc.MinLOD = 0;
            samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

            // Création de l'état de sampling
            pD3DDevice->CreateSamplerState(&samplerDesc, &m_sampleState);
        }
    };
}


#endif //!EFFECT_H