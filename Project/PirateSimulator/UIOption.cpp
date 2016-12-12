#include "UIOption.h"

#include "..\PetitMoteur3D\PetitMoteur3D\AfficheurSprite.h"
#include "RendererManager.h"

#include <memory>


using namespace PirateSimulator;



namespace PirateSimulator
{
    class OptionGraphic
    {
    private:
        std::unique_ptr<PM3D::CAfficheurSprite> m_spriteDisplayer;


    public:
        OptionGraphic(PM3D::CDispositifD3D11* pDispositif)
        {
            m_spriteDisplayer = std::make_unique<PM3D::CAfficheurSprite>(pDispositif);
        }


    public:
        void draw()
        {
            m_spriteDisplayer->Draw();
        }



    public:
        void addPannel(const std::string& file, const DirectX::XMFLOAT3& posOnScreen, float width, float height)
        {
            m_spriteDisplayer->AjouterPanneau(file, posOnScreen, width, height);
        }

        void addSprite(const std::string& file, float xPos, float yPos, float width, float height)
        {
            m_spriteDisplayer->AjouterSprite(file, xPos, yPos, width, height);
        }

        void addText(ID3D11ShaderResourceView* ressouceView, float xPos, float yPos)
        {
            m_spriteDisplayer->AjouterSpriteTexte(ressouceView, xPos, yPos);
        }
    };
}






UIOption::UIOption()
{}

void UIOption::initialize()
{
    const float fullScreenSize = 2.f;

    this->attachGraphics(RendererManager::singleton.getDispositif());

    m_graphics->addSprite("PirateSimulator/OptionTexture.dds", 0.f, 0.f, fullScreenSize, fullScreenSize);
}

bool UIOption::update()
{
    this->draw();
    return false;
}

bool UIOption::select()
{
    //TODO
    return true;
}

bool UIOption::cancel()
{
    //TODO
    return true;
}

void UIOption::attachItem(OptionItem& optionItem, AlgorithmObject algoCallback)
{
    m_items.push_back(optionItem);
    m_selector->addACallback(algoCallback);
}

OptionItem& UIOption::getItem(size_t index)
{
    return m_items[index];
}

void UIOption::draw()
{
    m_graphics->draw();
}