#ifndef UILOADING_H_INCLUDED
#define UILOADING_H_INCLUDED

#include "UIElement.h"

#include <memory>

namespace PM3D
{
    class CAfficheurSprite;
    class CAfficheurTexte;
}



namespace PirateSimulator 
{
    class CustomLoadingScreenDisplayer;

    class UILoading : public UIElementBase
    {
    public:
        enum
        {
            MINIMUM_LOADING_TIME_IN_MILLISECONDS = 3000
        };

        static constexpr const float ROTATION_SPEED_RAD_PER_SEC = 0.02f;


    private:
        std::unique_ptr<PM3D::CAfficheurSprite> m_afficheurSprite;
        std::unique_ptr<CustomLoadingScreenDisplayer> m_customLoadingDisplayer;

        std::unique_ptr<PM3D::CAfficheurTexte> m_loadingMessage;

        long long m_currentTime;
        long long m_lastUpdateTime;
        long long m_launchTime;


    public:
        UILoading();


    public:
        virtual void initialize();
        virtual bool update();
        virtual bool select();
        virtual bool cancel();
    };
}

#endif // !UILOADING_H_INCLUDED


