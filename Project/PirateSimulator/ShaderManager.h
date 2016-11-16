#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include <map>

#include "Shader.h"

#include "../PetitMoteur3D/PetitMoteur3D/Singleton.h"
#include "../PetitMoteur3D/PetitMoteur3D/dispositifD3D11.h"

namespace PirateSimulator
{
    
    class ShaderManager : public PM3D::CSingleton<ShaderManager>
    {
    private:
        std::map<std::string, Shader*> m_shaders;
        PM3D::CDispositifD3D11 *m_dispositif;

        friend class PM3D::CSingleton<ShaderManager>;
        ShaderManager() = default;

    public:
        void initialize(PM3D::CDispositifD3D11 *device);

        void addShader(const std::string &path);
        void addShader(Shader *shader);

        Shader *getShader(const std::string name) const;
    };
}

#endif //_SHADER_MANAGER_H_