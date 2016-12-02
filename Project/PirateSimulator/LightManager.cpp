#include "LightManager.h"

#include <algorithm>

using namespace PirateSimulator;


struct LightGenerator
{
public:
    template<typename LightType, typename LightModality>
    static std::vector<LightRef> generate(size_t lightCount)
    {

    }

};




LightManager LightManager::singleton;


LightManager::LightManager() 
{

}


const std::vector<LightRef>& LightManager::getLights(Light::type type, Light::modality modality) const noexcept
{
    return m_lightArray.at(type).at(modality);
}

void LightManager::addLight(LightRef newLight) noexcept
{
    m_lightArray[newLight->m_type][newLight->m_modality].push_back(newLight);
}