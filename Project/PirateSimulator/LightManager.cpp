#include "LightManager.h"

#include <algorithm>
#include <directxmath.h>

using namespace PirateSimulator;


struct LightGenerator
{
public:
    template<Light::type type, Light::modality modality>
    static std::vector<LightRef> generate(size_t lightCount, const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, float scope, float power = 1.f)
    {
        std::vector<LightRef> intermediary;
        intermediary.reserve(lightCount);

        DirectX::XMFLOAT3 position = start;

        const float stepX = (end.x - start.x) / static_cast<float>(lightCount);
        const float stepY = (end.y - start.y) / static_cast<float>(lightCount);
        const float stepZ = (end.z - start.z) / static_cast<float>(lightCount);

        for (size_t count = 0; count != lightCount; ++count)
        {
            intermediary.push_back(LightRef(new Light(position, type, modality, scope, power)));

            position.x += stepX;
            position.y += stepY;
            position.z += stepZ;
        }
        
        return std::move(intermediary);
    }

    static LightRef generateSun(float sunX, float sunY, float sunZ, float sunPower = 1.f)
    {
        float norm = sqrtf(sunX * sunX + sunY * sunY + sunZ * sunZ);

        DirectX::XMFLOAT3 sunLightTransform(sunX / norm, sunY / norm, sunZ / norm);
        float sunScope = 1000000.f;

        return LightRef(
            new Light(
                sunLightTransform,
                Light::type::DIRECTIONAL, 
                Light::modality::BRIGHT, 
                sunScope,
                sunPower
            )
        );
    }
};




LightManager LightManager::singleton;


LightManager::LightManager() 
{
    m_lightArray[Light::type::DIRECTIONAL][Light::modality::BRIGHT] = { LightGenerator::generateSun(10.f,-10.f,10.f) };

    m_lightArray[Light::type::DIRECTIONAL][Light::modality::DARKNESS] = {};


    DirectX::XMFLOAT3 pointLightStart   { 400.f, 400.f, 400.f };
    DirectX::XMFLOAT3 pointLightEnd     { 600.f, 600.f, 600.f };


    m_lightArray[Light::type::POINT][Light::modality::BRIGHT] = 
        std::move(LightGenerator::generate<Light::type::POINT, Light::modality::BRIGHT>(
                10, 
                pointLightStart, 
                pointLightEnd, 
                15.f, 
                1.f
            )
        );


    pointLightStart.x = 450.f;
    pointLightStart.y = 450.f;
    pointLightStart.z = 450.f;

    pointLightEnd.x = 550.f;
    pointLightEnd.y = 550.f;
    pointLightEnd.z = 550.f;

    m_lightArray[Light::type::POINT][Light::modality::DARKNESS] = 
        std::move(LightGenerator::generate<Light::type::POINT, Light::modality::DARKNESS>(
                4, 
                pointLightStart, 
                pointLightEnd, 
                15.f, 
                0.5f
            )
        );
}


const std::vector<LightRef>& LightManager::getLights(Light::type type, Light::modality modality) const noexcept
{
    return m_lightArray.at(type).at(modality);
}

void LightManager::addLight(LightRef newLight) noexcept
{
    m_lightArray[newLight->m_type][newLight->m_modality].push_back(newLight);
}