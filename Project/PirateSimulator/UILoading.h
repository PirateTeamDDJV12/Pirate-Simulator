#ifndef UILOADING_H_INCLUDED
#define UILOADING_H_INCLUDED

#include "UIElement.h"

#include <memory>
#include <gdiplus.h>
#include "..\PetitMoteur3D\PetitMoteur3D\AfficheurTexte.h"
#include "..\PetitMoteur3D\PetitMoteur3D\AfficheurSprite.h"


namespace PirateSimulator 
{
    class CustomLoadingScreenDisplayer : public PM3D::CAfficheurSprite
    {
    private:
        float m_angle;

        //Matrix to rotate at the exact center of the pannel.
        //Because the 0,0 of the pannel is on one of its vertax but we want the rotation on the middle...
        DirectX::XMMATRIX m_rotationCenterOffset;


    public:
        CustomLoadingScreenDisplayer() :
            m_angle{0.f},
            m_rotationCenterOffset{DirectX::XMMatrixTranslation(-0.5f, -0.5f, 0.f)},
            PM3D::CAfficheurSprite()
        {}


    public:
        float getAngle() const noexcept
        {
            return m_angle;
        }

        void setAngle(float angle)
        {
            m_angle = 0.f;
            rotate(angle);
        }

        void rotate(float angle)
        {
            m_angle += angle;

            while(m_angle < 0.f)
            {
                m_angle += DirectX::XM_2PI;
            }
            while(m_angle > DirectX::XM_2PI)
            {
                m_angle -= DirectX::XM_2PI;
            }

            for(auto iter = tabSprites.begin(); iter != tabSprites.end(); ++iter)
            {
                if((*iter)->bPanneau)
                {
                    auto spritePannel = static_cast<CAfficheurSprite::CPanneau*>((*iter));

                    (*iter)->matPosDim =
                        m_rotationCenterOffset *
                        DirectX::XMMatrixRotationZ(m_angle) *
                        DirectX::XMMatrixScaling(spritePannel->dimension.x, spritePannel->dimension.y, 1.f) *
                        DirectX::XMMatrixTranslation(spritePannel->position.x, spritePannel->position.y, spritePannel->position.z);
                }
            }
        }
    };

    class UILoading : public UIElementBase
    {
    public:
        enum
        {
            MINIMUM_LOADING_TIME_IN_MILLISECONDS = 3000,
            UPDATE_LOADING_MESSAGE_TIME_IN_MILLISECONDS = 450
        };

        static constexpr const float ROTATION_SPEED_RAD_PER_SEC = 0.02f;


    private:
        std::unique_ptr<PM3D::CAfficheurSprite> m_afficheurSprite;
        std::unique_ptr<CustomLoadingScreenDisplayer> m_customLoadingDisplayer;

        std::unique_ptr<PM3D::CAfficheurTexte> m_loadingMessage;
        std::unique_ptr<Gdiplus::Font> m_policeFont;

        long long m_currentTime;
        long long m_lastUpdateTime;
        long long m_launchTime;
        long long m_timeToIncreasePoints;

        unsigned char m_pointCount;


    public:
        UILoading();


    public:
        virtual void initialize();
        virtual bool update();
        virtual bool select();
        virtual bool cancel();


    private:
        void updateLoadingMessage();
    };
}

#endif // !UILOADING_H_INCLUDED


