#pragma once
#include "../../PirateSimulator/Mesh.h"
#include "DispositifD3D11.h"

#include <DirectXMath.h>

namespace PM3D
{
	const unsigned int index_bloc[36]={
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
	
    namespace ShaderCBloc
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


            ShadersParams()
            {
                vLumiere =  DirectX::XMVectorSet(-10.0f, 10.0f, -10.0f, 1.0f);
                vCamera =   DirectX::XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f);
                vAEcl =     DirectX::XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
                vAMat =     DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
                vDEcl =     DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
                vDMat =     DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
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

	//  Classe : CBloc
	//
	//  BUT : 	Classe de bloc
	//
	class CBloc : public PirateSimulator::Mesh<ShaderCBloc::ShadersParams>
	{
	public:
		CBloc(const float dx, const float dy, const float dz, 
      	CDispositifD3D11* pDispositif);

   		// Destructeur
		virtual ~CBloc(void);

		void Draw();

	protected :
		// Constructeur par défaut
		CBloc(void) :
            PirateSimulator::Mesh<ShaderCBloc::ShadersParams>(ShaderCBloc::ShadersParams())
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
		float rotation;

		


	};

}

	