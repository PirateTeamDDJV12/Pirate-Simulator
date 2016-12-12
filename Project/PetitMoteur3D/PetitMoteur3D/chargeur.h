#pragma once

#include <vector>
using namespace std;
using namespace DirectX;

namespace PM3D
{

    class CParametresChargement
    {
    public:
        CParametresChargement()
        {
            bInverserCulling = false;
            bMainGauche = false;
        }

        CParametresChargement(string NomChemin, string NomFichier, bool bInverserCulling, bool bMainGauche) :
            NomChemin{ NomChemin }, NomFichier{ NomFichier }, bMainGauche{ bMainGauche }, bInverserCulling{ bInverserCulling }
        {}

        string NomChemin;
        string NomFichier;
        bool bMainGauche;
        bool bInverserCulling;
    };

    class IChargeur
    {
    public:
        IChargeur() {}
        virtual ~IChargeur(void) {}

        virtual void Chargement(const CParametresChargement& param) = 0;

        virtual int  GetNombreSubmesh() = 0;
        virtual int	 GetNombreSommetsSubmesh(unsigned int n) = 0;
        virtual XMFLOAT3 GetPosition(int noMesh, int NoSommet) = 0;
        virtual XMFLOAT2 GetCoordTex(int noMesh, int NoSommet) = 0;
        virtual XMFLOAT3 GetNormale(int noMesh, int NoSommet) = 0;
        virtual unsigned int GetIndice(int noMesh, int noPoly, int NoIndice) = 0;
        virtual int  GetNombrePolygonesSubmesh(int noMesh) = 0;

        virtual int  GetNombreMaterial() = 0;
        virtual void GetMaterial(int _i,
            string& _NomFichierTexture,
            string& _NomMateriau,
            XMFLOAT4& _Ambient,
            XMFLOAT4& _Diffuse,
            XMFLOAT4& _Specular,
            float& _Puissance) = 0;

        virtual int GetMaterialIndex(int i) = 0;

    protected:
        CParametresChargement parametres;


    };


}