#ifndef UILOADING_H_INCLUDED
#define UILOADING_H_INCLUDED

#include "UIElement.h"

#include <memory>

namespace PM3D
{
    class CAfficheurSprite;
    class CAfficheurTexte;
}

namespace Gdiplus
{
    class Font;
}


namespace PirateSimulator 
{
    class CustomLoadingScreenDisplayer;

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


