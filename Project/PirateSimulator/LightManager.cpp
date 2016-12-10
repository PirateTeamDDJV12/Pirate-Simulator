#include "LightManager.h"

#include "GameConfig.h"

#include <algorithm>
#include <directxmath.h>
#include "..\PetitMoteur3D\PetitMoteur3D\Config\Config.hpp"
#include "BezierCurve.h"

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

    template<Light::type type, Light::modality modality>
    static std::vector<LightRef> generateAsCubicBezierCurve(
        size_t lightCount, 
        const DirectX::XMFLOAT3& start, 
        const DirectX::XMFLOAT3& pointControl1, 
        const DirectX::XMFLOAT3& pointControl2, 
        const DirectX::XMFLOAT3& end, 
        float scope, 
        float power = 1.f)
    {
        std::vector<LightRef> intermediary;
        intermediary.reserve(lightCount);

        CubicBezierCurve bezierGenerator(
            start,
            pointControl1,
            pointControl2,
            end,
            lightCount
        );

        std::vector<DirectX::XMFLOAT3> bezierPosition = bezierGenerator.getTrajectory();

        for (size_t count = 0; count != lightCount; ++count)
        {
            intermediary.push_back(LightRef(new Light(bezierPosition[count], type, modality, scope, power)));
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

namespace PirateSimulator
{
    //Sun will move along the Z axis
    class Sun
    {
    private:
        float m_x;
        float m_y;
        DirectX::XMFLOAT3 m_center;

        DirectX::XMFLOAT3 m_vectorToCenter;

        float m_distanceToCenter;

        float m_angularVelocity;
        float m_angle;

        LightRef m_sunLight;


    public:
        Sun(float x, float y, const DirectX::XMFLOAT3& center, float angularVelocity) :
            m_x{ x },
            m_y{ y },
            m_center{ center },
            m_angularVelocity{ angularVelocity }
        {
            m_vectorToCenter.x = m_center.x - m_x;
            m_vectorToCenter.y = m_center.y - m_y;
            m_vectorToCenter.z = 0.f;

            m_distanceToCenter = sqrtf(m_vectorToCenter.x * m_vectorToCenter.x + m_vectorToCenter.y * m_vectorToCenter.y);

            m_angle = acosf(m_vectorToCenter.x / m_distanceToCenter);
        }


    public:
        float getAngularVelocity() const noexcept
        {
            return m_angularVelocity;
        }

        const DirectX::XMFLOAT3& getCenter() const noexcept
        {
            return m_center;
        }

        //get a non normalized vector to the center of rotation of the sun
        const DirectX::XMFLOAT3& getLightDirection() const noexcept
        {
            return m_vectorToCenter;
        }

        float getSunDistanceToSun() const noexcept
        {
            return m_distanceToCenter;
        }

        float getCurrentAngle() const noexcept
        {
            return m_angle;
        }

        LightRef ref() const noexcept
        {
            return m_sunLight;
        }

        void setSunLight(LightRef light)
        {
            m_sunLight = light;
        }

        float getAmbientLightApproximation() const noexcept
        {
            if (m_angle < DirectX::XM_PI) // the sun is below the field.
            {
                return GameGlobals::SunGlobals::MIN_AMBIENT_LIGHT;
            }

            // the max value must be when the angle is at 3 * Pi / 4 (Sun at its peak)
            return GameGlobals::SunGlobals::MIN_AMBIENT_LIGHT - sinf(m_angle) * GameGlobals::SunGlobals::AMBIENT_LIGHT_COEFF + abs(cosf(m_angle)) / 10000.f; 
        }

        float getDiffuseLightApproximation() const noexcept
        {
            return 1.0f; //TODO
        }


    public:
        void update(float elapsedTime)
        {
            float stepAngle = m_angularVelocity * elapsedTime;

            m_angle += stepAngle;

            if (m_angle > DirectX::XM_2PI) // step angle will be positive thus m_angle will increase so it is useless to check the m_angle < DirectX::XM_2PI
            {
                m_angle -= DirectX::XM_2PI;
            }

            float cosinus   = cosf(m_angle);
            float sinus     = sinf(m_angle);

            m_vectorToCenter.x = m_distanceToCenter * cosinus;
            m_vectorToCenter.y = m_distanceToCenter * sinus;

            m_x = m_center.x - m_vectorToCenter.x;
            m_y = m_center.y - m_vectorToCenter.y;

            m_sunLight->m_vector.x = cosinus;
            m_sunLight->m_vector.z = sinus; // the GPU is Right-Hand convention -> y = z
        }
    };
}


LightManager LightManager::singleton;


LightManager::LightManager() 
{
    Config* conf = Config::getInstance();

    float centerMapX = conf->getWidth() * conf->getMapScale() / 2.f;
    float centerMapZ = conf->getHeight() * conf->getMapScale() / 2.f;

    DirectX::XMFLOAT3 centerMap(centerMapX, conf->getOffset(), centerMapZ);

    m_sun = std::make_unique<Sun>(GameGlobals::SunGlobals::X_SUN, GameGlobals::SunGlobals::Y_SUN, centerMap, 8.f);

    m_lightArray[Light::type::DIRECTIONAL][Light::modality::BRIGHT] = { 
        LightGenerator::generateSun(
            m_sun->getLightDirection().x,
            m_sun->getLightDirection().y,
            m_sun->getLightDirection().z,
            GameGlobals::SunGlobals::POWER
        ) 
    };

    m_sun->setSunLight(m_lightArray[Light::type::DIRECTIONAL][Light::modality::BRIGHT][0]);

    m_lightArray[Light::type::DIRECTIONAL][Light::modality::DARKNESS] = {};


    DirectX::XMFLOAT3 pointLightStart   { 800.f, -20.f, 950.f };
    DirectX::XMFLOAT3 pointLightEnd     { 1100.f, -20.f, 950.f };
    DirectX::XMFLOAT3 firstControlPoint { 900, -20.f, 1200.f };
    DirectX::XMFLOAT3 secondControlPoint{ 1000.f, -20.f, 700.f };


    m_lightArray[Light::type::POINT][Light::modality::BRIGHT] =
        std::move(LightGenerator::generateAsCubicBezierCurve<Light::type::POINT, Light::modality::BRIGHT>(
                8,
                pointLightStart,
                firstControlPoint,
                secondControlPoint,
                pointLightEnd,
                16.f,
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

LightRef LightManager::getBrightSun() const noexcept
{
    return m_sun->ref();
}

LightRef LightManager::getDarkSun()   const noexcept
{
    return getLights(Light::type::DIRECTIONAL, Light::modality::BRIGHT)[0];
}

const std::vector<LightRef>& LightManager::getBrightPointsLights()  const noexcept
{
    return getLights(Light::type::POINT, Light::modality::BRIGHT);
}

const std::vector<LightRef>& LightManager::getDarkPointsLights()    const noexcept
{
    return getLights(Light::type::POINT, Light::modality::BRIGHT);
}

void LightManager::update(float elapsedTime)
{
    m_sun->update(elapsedTime);
}

float LightManager::getSunAngle() const noexcept
{
    return m_sun->getCurrentAngle();
}

float LightManager::getAmbientLightCoefficient() const noexcept
{
    return m_sun->getAmbientLightApproximation();
}

float LightManager::getDiffuseLightCoefficient() const noexcept
{
    return m_sun->getDiffuseLightApproximation();
}