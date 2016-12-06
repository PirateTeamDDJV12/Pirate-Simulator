#ifndef LIGHTMANAGER_H_INCLUDED
#define LIGHTMANAGER_H_INCLUDED

#include "Light.h"

#include <map>
#include <vector>
#include <memory>


namespace PirateSimulator
{
    class Sun;
    class LightManager
    {
    public:
        static LightManager singleton;


    private:
        std::map< Light::type, std::map< Light::modality, std::vector<LightRef> > > m_lightArray;
        std::unique_ptr<Sun> m_sun;


    public:
        LightManager();


    public:
        const std::vector<LightRef>& getLights(Light::type type, Light::modality modality) const noexcept;
        void addLight(LightRef newLight) noexcept;


    public:
        LightRef getBrightSun() const noexcept;
        LightRef getDarkSun()   const noexcept;
        const std::vector<LightRef>& getBrightPointsLights()  const noexcept;
        const std::vector<LightRef>& getDarkPointsLights()    const noexcept;

        float getSunAngle() const noexcept;

        // get the ambient light. Store it before use. Algorithm used inside
        float getAmbientLightCoefficient() const noexcept;

        // get the diffuse light. Store it before use. Algorithm used inside
        float getDiffuseLightCoefficient() const noexcept;


    public:
        void update(float elapsedTime);
    };
}

#endif // !LIGHTMANAGER_H_INCLUDED