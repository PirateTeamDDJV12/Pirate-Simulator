#include "UILoading.h"

#include "GameObjectManager.h"
#include "RendererManager.h"
#include "TimeManager.h"
#include "../PetitMoteur3D/PetitMoteur3D/AfficheurTexte.h"
#include "../PetitMoteur3D/PetitMoteur3D/AfficheurSprite.h"

#include <Gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace PirateSimulator;


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
            m_angle{ 0.f },
            m_rotationCenterOffset{ DirectX::XMMatrixTranslation(-0.5f, -0.5f, 0.f) },
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

            for (auto iter = tabSprites.begin(); iter != tabSprites.end(); ++iter)
            {
                if ((*iter)->bPanneau)
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
}


UILoading::UILoading() :
    m_pointCount{1}
{
    m_afficheurSprite = std::make_unique<PM3D::CAfficheurSprite>();
    m_customLoadingDisplayer = std::make_unique<CustomLoadingScreenDisplayer>();

    m_policeFont = std::make_unique<Gdiplus::Font>(&Gdiplus::FontFamily{ L"Edwardian Script ITC", NULL }, 70.0, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    m_loadingMessage = std::make_unique<PM3D::CAfficheurTexte>(260, 90, m_policeFont.get(), Gdiplus::Color(255, 255, 255, 255));


    // ajout de panneaux 
    m_afficheurSprite->AjouterPanneau(
        "PirateSimulator/PirateFlag.dds",
        { -1.0f, -1.0f, 0.0f },
        2.0f, 2.0f
    );

    m_loadingMessage->Ecrire(L"Loading");

    auto dispositif = RendererManager::singleton.getDispositif();

    int loadingMessageXPos = 0.37f * dispositif->GetLargeur();
    int loadingMessageYPos = 0.96f * dispositif->GetHauteur();

    m_afficheurSprite->AjouterSpriteTexte(m_loadingMessage->GetTextureView(), loadingMessageXPos, loadingMessageYPos);

    m_customLoadingDisplayer->AjouterPanneau(
        "PirateSimulator/ShipWheel.dds",
        { 0.75f, -0.75f, 0.0f },
        0.5f, 0.5f
    );
}

void UILoading::initialize()
{
    m_launchTime = TimeManager::GetInstance().msNow().count();
    m_lastUpdateTime = m_launchTime;
    m_timeToIncreasePoints = m_launchTime + UILoading::UPDATE_LOADING_MESSAGE_TIME_IN_MILLISECONDS;
}

bool UILoading::update()
{
    m_currentTime = TimeManager::GetInstance().msNow().count();

    const float speedDegree = UILoading::ROTATION_SPEED_RAD_PER_SEC * (DirectX::XM_PI / 180.f);

    m_customLoadingDisplayer->rotate(((m_currentTime - m_lastUpdateTime) % 360) * speedDegree);

    long long deltaTime = m_currentTime - m_timeToIncreasePoints;

    if (deltaTime > 0)
    {
        this->updateLoadingMessage();
        m_timeToIncreasePoints = m_currentTime + UILoading::UPDATE_LOADING_MESSAGE_TIME_IN_MILLISECONDS - deltaTime;
    }

    m_afficheurSprite->Draw();
    m_customLoadingDisplayer->Draw();

    m_lastUpdateTime = m_currentTime;

    if (m_currentTime - m_launchTime > UILoading::MINIMUM_LOADING_TIME_IN_MILLISECONDS)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool UILoading::select()
{
    //TODO
    return true;
}

bool UILoading::cancel()
{
    //TODO
    return true;
}

void UILoading::updateLoadingMessage()
{
    switch (m_pointCount)
    {
    case 0:
        m_loadingMessage->Ecrire(L"Loading");
        break;

    case 1:
        m_loadingMessage->Ecrire(L"Loading.");
        break;

    case 2:
        m_loadingMessage->Ecrire(L"Loading..");
        break;

    case 3:
    default:
        m_loadingMessage->Ecrire(L"Loading...");
        m_pointCount = 0;
        return;
    }

    ++m_pointCount;
}