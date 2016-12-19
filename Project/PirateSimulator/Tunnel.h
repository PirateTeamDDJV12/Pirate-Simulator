#ifndef TUNNEL_H
#define TUNNEL_H

#include "..\PetitMoteur3D\PetitMoteur3D\ObjetMesh.h"
#include "Material.h"
#include "Effect.h"
#include "..\PetitMoteur3D\PetitMoteur3D\sommetbloc.h"


namespace PM3D
{
    class CTexture;
}

namespace PirateSimulator
{
    class Tunnel : public PM3D::CObjetMesh
    {
    private:
        ID3D11VertexShader*  pVertexShader;
        ID3D11PixelShader*  pPixelShader;

        std::vector<CSommetBloc> m_csommetsArray;
        std::vector<unsigned int> m_index_bloc;

        //Pour texture
        Material m_material;
        Effect m_textureEffect;


    public:
        Tunnel(const PM3D::ShaderCObjectMesh::ShadersParams& shaderParameter,
            const std::wstring& shaderName,
            PM3D::IChargeur& chargeur,
            const std::string& textureFileName);


    public:
        void InitShaders();
        void Init(const std::string& textureFileName);
        void loadTexture(const std::string& filename);
        void setTexture(PM3D::CTexture* texture);
        void Draw();
    };
}

#endif // TUNNEL_H
