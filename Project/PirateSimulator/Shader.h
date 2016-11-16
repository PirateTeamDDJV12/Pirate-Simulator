#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
#include "../PetitMoteur3D/PetitMoteur3D/dispositifD3D11.h"
#include "../PetitMoteur3D/PetitMoteur3D/d3dx11effect.h"

namespace PirateSimulator
{
    class Shader
    {
    private:
        ID3D11VertexShader  *m_vertexShader;
        ID3D11PixelShader   *m_pixelShader;
        ID3DBlob            *m_VSBlob;
        ID3DBlob            *m_PSBlob;
        ID3D11Device        *m_D3DDevice;
        const std::string   m_name;
    public:
        Shader() = delete;
        
        // Works only for vertex and pixel shader with the same name and with extension .vhl and .phl
        // May be changed
        Shader(const std::string &path, PM3D::CDispositifD3D11 *dispositif);

        ~Shader();

        ID3D11VertexShader  *getVertexShader() const;
        ID3D11PixelShader   *getPixelShader() const;
        ID3DBlob            *getVSBlob() const;
        ID3DBlob            *getPSBlob() const;
        const std::string   &getName() const;
    };
}

#endif //!_SHADER_H_