#ifndef BLOCMESH_H_INCLUDED
#define BLOCMESH_H_INCLUDED

#include "Mesh.h"

#include "../PetitMoteur3D/PetitMoteur3D/dispositifD3D11.h"
#include "RendererManager.h"
#include "../PetitMoteur3D/PetitMoteur3D/Resource.h"

#include <directxmath.h>
#include <wtypes.h>
#include <d3d11.h>

namespace PirateSimulator
{
    namespace ShaderBloc
    {
        struct ShadersParams
        {
            DirectX::XMMATRIX matWorldViewProj;	// la matrice totale 
            DirectX::XMMATRIX matWorld;			// matrice de transformation dans le monde 
            DirectX::XMVECTOR vLumiere; 			// la position de la source d'éclairage (Point)
            DirectX::XMVECTOR vCamera; 			// la position de la caméra
            DirectX::XMVECTOR vAEcl; 			// la valeur ambiante de l'éclairage
            DirectX::XMVECTOR vAMat; 			// la valeur ambiante du matériau
            DirectX::XMVECTOR vDEcl; 			// la valeur diffuse de l'éclairage 
            DirectX::XMVECTOR vDMat; 			// la valeur diffuse du matériau 

            /*DirectX::XMVECTOR fill1;
            DirectX::XMVECTOR fill2;*/

            ShadersParams()
            {
                vLumiere = DirectX::XMVectorSet(-10.0f, 10.0f, -10.0f, 1.0f);
                vCamera = DirectX::XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f);
                vAEcl = DirectX::XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
                vAMat = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
                vDEcl = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
                vDMat = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
            }

            ShadersParams(
                const DirectX::XMVECTOR& lumiere,
                const DirectX::XMVECTOR& camera,
                const DirectX::XMVECTOR& AEcl,
                const DirectX::XMVECTOR& AMat,
                const DirectX::XMVECTOR& DEcl,
                const DirectX::XMVECTOR& DMat
            )
            {
                vLumiere = lumiere;
                vCamera = camera;
                vAEcl = AEcl;
                vAMat = AMat;
                vDEcl = DEcl;
                vDMat = DMat;
            }
        };
    }


    template<class BlocInternalData>
    class BlocMesh : public Mesh<ShaderBloc::ShadersParams>, protected BlocInternalData
    {
    private:
        template <class Type>
        static void DXRelacher(Type& UnPointeur)
        {
            if (UnPointeur != NULL)
            {
                UnPointeur->Release();
                UnPointeur = NULL;
            }
        }

        // Plus pratique, essayer en envoyant un code spécifique 
        // comme résultat
        template <class Type1, class Type2>
        inline void DXEssayer(const Type1& Resultat, const Type2& unCode)
        {
            if (Resultat != S_OK)
                throw unCode;
        }

    public:
        static constexpr const unsigned int index_bloc[36] = {
            0,1,2,			// devant
            0,2,3,			// devant
            5,6,7,			// arrière
            5,7,4,			// arrière
            8,9,10,			// dessous
            8,10,11,		// dessous
            13,14,15,		// dessus
            13,15,12,		// dessus
            19,16,17,		// gauche
            19,17,18,		// gauche
            20,21,22,		// droite
            20,22,23		// droite
        };


    protected:
        PM3D::CDispositifD3D11* pDispositif;

        ID3D11Buffer* pVertexBuffer;
        ID3D11Buffer* pIndexBuffer;

        ID3D11VertexShader*  pVertexShader;
        ID3D11PixelShader*  pPixelShader;
        ID3D11InputLayout* pVertexLayout;

        // Définitions des valeurs d'animation
        ID3D11Buffer* pConstantBuffer;


    public:
        BlocMesh(const BlocInternalData& internalBlocDatas,
            const ShaderBloc::ShadersParams& shaderParams,
            const std::wstring& VSShaderFileName, const std::wstring& PSShaderFileName
        ) :
            Mesh<ShaderBloc::ShadersParams>(m_shaderParameter),
            BlocInternalData(internalBlocDatas)
        {
            pDispositif = RendererManager::singleton.getDispositif();
            // Création du vertex buffer et copie des sommets
            ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

            D3D11_BUFFER_DESC bd;
            ZeroMemory(&bd, sizeof(bd));

            bd.Usage = D3D11_USAGE_DEFAULT;
            bd.ByteWidth = sizeof(BlocPoint) * 24;
            bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bd.CPUAccessFlags = 0;

            D3D11_SUBRESOURCE_DATA InitData;
            ZeroMemory(&InitData, sizeof(InitData));
            InitData.pSysMem = m_apex;
            pVertexBuffer = NULL;

            pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer);

            // Création de l'index buffer et copie des indices
            ZeroMemory(&bd, sizeof(bd));

            bd.Usage = D3D11_USAGE_DEFAULT;
            bd.ByteWidth = sizeof(unsigned int) * 36;
            bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
            bd.CPUAccessFlags = 0;

            ZeroMemory(&InitData, sizeof(InitData));
            InitData.pSysMem = index_bloc;
            pIndexBuffer = NULL;

            pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer);


            // Initialisation des shaders
            InitShaders(VSShaderFileName, PSShaderFileName);
            DXRelacher(pD3DDevice);
        }

        BlocMesh(const float dx, const float dy, const float dz,
            const ShaderBloc::ShadersParams& shaderParams, 
            const std::wstring& VSShaderFileName, const std::wstring& PSShaderFileName
        ) :
            Mesh<ShaderBloc::ShadersParams>(shaderParams),
            BlocInternalData(dx, dy, dz)
        {
            pDispositif = RendererManager::singleton.getDispositif();
            // Création du vertex buffer et copie des sommets
            ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

            D3D11_BUFFER_DESC bd;
            ZeroMemory(&bd, sizeof(bd));

            bd.Usage = D3D11_USAGE_DEFAULT;
            bd.ByteWidth = sizeof(BlocPoint) * 24;
            bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bd.CPUAccessFlags = 0;

            D3D11_SUBRESOURCE_DATA InitData;
            ZeroMemory(&InitData, sizeof(InitData));
            InitData.pSysMem = m_apex;
            pVertexBuffer = NULL;

            DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);

            // Création de l'index buffer et copie des indices
            ZeroMemory(&bd, sizeof(bd));

            bd.Usage = D3D11_USAGE_DEFAULT;
            bd.ByteWidth = sizeof(unsigned int) * 36;
            bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
            bd.CPUAccessFlags = 0;

            ZeroMemory(&InitData, sizeof(InitData));
            InitData.pSysMem = index_bloc;
            pIndexBuffer = NULL;

            DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer),
                DXE_CREATIONINDEXBUFFER);


            // Initialisation des shaders
            InitShaders(VSShaderFileName, PSShaderFileName);
            DXRelacher(pD3DDevice);
        }

        // Destructeur
        virtual ~BlocMesh()
        {
            BlocMesh::DXRelacher(pPixelShader);
            BlocMesh::DXRelacher(pConstantBuffer);
            BlocMesh::DXRelacher(pVertexLayout);
            BlocMesh::DXRelacher(pVertexShader);
            BlocMesh::DXRelacher(pIndexBuffer);
            BlocMesh::DXRelacher(pVertexBuffer);
        }


    public:
        void Draw()
        {
            // Obtenir le contexte
            ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

            // Choisir la topologie des primitives
            pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            // Source des sommets
            UINT stride = sizeof(CSommetBloc);
            UINT offset = 0;
            pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

            // Source des index
            pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

            // Activer le VS
            pImmediateContext->VSSetShader(pVertexShader, NULL, 0);

            // input layout des sommets
            pImmediateContext->IASetInputLayout(pVertexLayout);

            // Initialiser et sélectionner les «constantes» du VS
            XMMATRIX viewProj = CameraManager::singleton.getMatViewProj();

            m_shaderParameter.matWorldViewProj = XMMatrixTranspose(m_matWorld * viewProj);
            m_shaderParameter.matWorld = XMMatrixTranspose(m_matWorld);

            pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &m_shaderParameter, 0, 0);

            pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);

            // Pas de Geometry Shader
            pImmediateContext->GSSetShader(NULL, NULL, 0);

            // Activer le PS
            pImmediateContext->PSSetShader(pPixelShader, NULL, 0);
            pImmediateContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

            // **** Rendu de l'objet	   
            pImmediateContext->DrawIndexed(36, 0, 0);
        }


    protected:
        void InitShaders(const std::wstring& VSShaderFileName, const std::wstring& PSShaderFileName)
        {
            // Compilation et chargement du vertex shader
            ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

            ID3DBlob* pVSBlob = NULL;
            /*D3DCompileFromFile(L"MiniPhong.vhl",
                NULL, NULL,
                "MiniPhongVS",
                "vs_4_0",
                D3DCOMPILE_ENABLE_STRICTNESS,
                0,
                &pVSBlob, nullptr);*/
            DXEssayer(D3DCompileFromFile(VSShaderFileName.c_str(),
                NULL, NULL,
                "MiniPhongVS",
                "vs_4_0",
                D3DCOMPILE_ENABLE_STRICTNESS,
                0,
                &pVSBlob, nullptr), DXE_FICHIER_VS);

            DXEssayer(pD3DDevice->CreateVertexShader(pVSBlob->GetBufferPointer(),
                pVSBlob->GetBufferSize(),
                NULL,
                &pVertexShader),
                DXE_CREATION_VS);

            // Créer l'organisation des sommets
            pVertexLayout = NULL;
            DXEssayer(pD3DDevice->CreateInputLayout(BlocPoint::layout,
                BlocPoint::numElements,
                pVSBlob->GetBufferPointer(),
                pVSBlob->GetBufferSize(),
                &pVertexLayout),
                DXE_CREATIONLAYOUT);

            pVSBlob->Release(); //  On n'a plus besoin du blob

                                // Création d'un tampon pour les constantes du VS
            D3D11_BUFFER_DESC bd;
            ZeroMemory(&bd, sizeof(bd));

            bd.Usage = D3D11_USAGE_DEFAULT;
            bd.ByteWidth = sizeof(ShaderBloc::ShadersParams);
            bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            bd.CPUAccessFlags = 0;
            pD3DDevice->CreateBuffer(&bd, NULL, &pConstantBuffer);

            // Compilation et chargement du pixel shader
            ID3DBlob* pPSBlob = NULL;
            /*DXEssayer(D3DCompileFromFile(L"MiniPhong.phl",
                NULL, NULL,
                "MiniPhongPS",
                "ps_4_0",
                D3DCOMPILE_ENABLE_STRICTNESS,
                0,
                &pPSBlob, NULL), 
                DXE_FICHIER_PS);*/
            DXEssayer(D3DCompileFromFile(PSShaderFileName.c_str(),
                NULL, NULL,
                "MiniPhongPS",
                "ps_4_0",
                D3DCOMPILE_ENABLE_STRICTNESS,
                0,
                &pPSBlob, NULL),
                DXE_FICHIER_PS);

            DXEssayer(pD3DDevice->CreatePixelShader(pPSBlob->GetBufferPointer(),
                pPSBlob->GetBufferSize(),
                NULL,
                &pPixelShader),
                DXE_CREATION_PS);

            pPSBlob->Release(); //  On n'a plus besoin du blob
        }
    };
}

#endif //!BLOCMESH_H_INCLUDED