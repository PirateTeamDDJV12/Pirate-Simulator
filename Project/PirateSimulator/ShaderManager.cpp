#include "stdafx.h"

#include "ShaderManager.h"

using namespace PirateSimulator;

void ShaderManager::initialize(PM3D::CDispositifD3D11 *device)
{
    m_dispositif = device;
    addShader(new Shader("MiniPhong", m_dispositif));
}

void ShaderManager::addShader(const std::string &path)
{

    m_shaders[path] = new Shader(path, m_dispositif);
}

void ShaderManager::addShader(Shader *shader)
{
    m_shaders[shader->getName()] = shader;
}

Shader *ShaderManager::getShader(const std::string name) const
{
    return m_shaders.at(name);
}