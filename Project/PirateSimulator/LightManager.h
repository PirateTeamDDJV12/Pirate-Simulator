#ifndef LIGHTMANAGER_H_INCLUDED
#define LIGHTMANAGER_H_INCLUDED

#include "Light.h"

#include <map>
#include <vector>


namespace PirateSimulator
{
    class LightManager
    {
    public:
        static LightManager singleton;


    private:
        std::map< Light::type, std::map< Light::modality, std::vector<LightRef> > > m_lightArray;
        

    public:
        LightManager();


    public:
        const std::vector<LightRef>& getLights(Light::type type, Light::modality modality) const noexcept;
        void addLight(LightRef newLight) noexcept;
    };
}

#endif // !LIGHTMANAGER_H_INCLUDED