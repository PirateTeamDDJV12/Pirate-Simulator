#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED


#include <memory>


namespace PirateSimulator
{
    struct Light
    {
    public:
        enum type
        {
            POINT,
            DIRECTIONAL,

            TYPE_COUNT
        };

        enum modality
        {
            BRIGHT,
            DARKNESS,

            MODALITY_COUNT
        };


    public:
        DirectX::XMFLOAT3 m_vector;

        float m_power;
        float m_scope;

        type m_type;
        modality m_modality;


    public:
        Light(const DirectX::XMFLOAT3& relatedVector, type lightType, modality lightModality, float scope) :
            m_type{ lightType },
            m_modality{ lightModality },
            m_vector{ relatedVector },
            m_scope{ scope },
            m_power{ 1.f }
        {}

        Light(const DirectX::XMFLOAT3& relatedVector, type lightType, modality lightModality, float scope, float power) :
            m_type{ lightType },
            m_modality{ lightModality },
            m_vector{ relatedVector },
            m_scope{ scope },
            m_power{ power }
        {}
    };

    using LightRef = std::shared_ptr<Light>;
}


#endif //!LIGHT_H_INCLUDED