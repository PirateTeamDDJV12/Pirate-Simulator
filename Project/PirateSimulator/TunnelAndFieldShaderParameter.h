#ifndef TUNNELANDFIELDSHADERPARAMETER_H_INCLUDED
#define TUNNELANDFIELDSHADERPARAMETER_H_INCLUDED


#include <DirectXMath.h>


namespace PirateSimulator
{
    namespace ShaderTunnelAndField
    {
        struct ShadersParams
        {
        public:
            DirectX::XMMATRIX matWorldViewProj;	// la matrice totale 
            DirectX::XMMATRIX matWorld;			// matrice de transformation dans le monde 

            DirectX::XMVECTOR vLumiere; 			// la position de la source d'éclairage (Point)
            DirectX::XMVECTOR vCamera; 			// la position de la caméra

            DirectX::XMVECTOR vLightPoint1;
            DirectX::XMVECTOR vLightPoint2;
            DirectX::XMVECTOR vLightPoint3;
            DirectX::XMVECTOR vLightPoint4;
            DirectX::XMVECTOR mappedLightPointPower1;

            DirectX::XMVECTOR vLightPoint5;
            DirectX::XMVECTOR vLightPoint6;
            DirectX::XMVECTOR vLightPoint7;
            DirectX::XMVECTOR vLightPoint8;
            DirectX::XMVECTOR mappedLightPointPower2;

            DirectX::XMVECTOR vAEcl; 			// la valeur ambiante de l'éclairage
            DirectX::XMVECTOR vAMat; 			// la valeur ambiante du matériau
            DirectX::XMVECTOR vDEcl; 			// la valeur diffuse de l'éclairage 
            DirectX::XMVECTOR vDMat; 			// la valeur diffuse du matériau 
            DirectX::XMVECTOR vSMat; 			// la valeur spéculaire du matériau 

            float puissance;
            float sunPower;

            DirectX::XMFLOAT2 uselessFill;


        public:
            ShadersParams() :
                puissance{ 0.5f }
            {
                vLumiere = DirectX::XMVectorSet(130.0f, 130.0f, -50.0f, 1.0f);
                vAEcl = DirectX::XMVectorSet(0.6f, 0.6f, 0.6f, 1.0f);
                vAMat = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
                vDEcl = DirectX::XMVectorSet(0.8f, 0.8f, 0.8f, 0.8f);
                vDMat = DirectX::XMVectorSet(0.8f, 0.8f, 0.8f, 0.8f);
            }

            ShadersParams(
                const DirectX::XMVECTOR& lumiere,
                const DirectX::XMVECTOR& AEcl,
                const DirectX::XMVECTOR& AMat,
                const DirectX::XMVECTOR& DEcl,
                const DirectX::XMVECTOR& DMat
            ) :
                puissance{ 1.f }
            {
                vLumiere = lumiere;
                vAEcl = AEcl;
                vAMat = AMat;
                vDEcl = DEcl;
                vDMat = DMat;
            }

            ShadersParams(
                const DirectX::XMVECTOR& lumiere,
                const DirectX::XMVECTOR& AEcl,
                const DirectX::XMVECTOR& AMat,
                const DirectX::XMVECTOR& DEcl,
                const DirectX::XMVECTOR& DMat,
                const DirectX::XMVECTOR& SEcl,
                const DirectX::XMVECTOR& SMat
            )
            {
                vLumiere = lumiere;
                vAEcl = AEcl;
                vAMat = AMat;
                vDEcl = DEcl;
                vDMat = DMat;
                vSMat = SMat;
            }
        };
    }
}

#endif // !TUNNELANDFIELDSHADERPARAMETER_H_INCLUDED
