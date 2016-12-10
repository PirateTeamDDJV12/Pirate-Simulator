#include "SoundManager.h"

#include "FMOD/fmod_studio.hpp"
#include "FMOD/fmod.hpp"
#include "FMOD/common.h"

using namespace PirateSimulator;


SoundManager SoundManager::singleton;


struct FMODSystemBloc
{
public:
    FMOD::Studio::System* m_system;
    FMOD::System* m_lowLevelSystem;

    void* m_extraDriverData;


public:
    FMODSystemBloc() :
        m_system{ nullptr },
        m_lowLevelSystem{ nullptr },
        m_extraDriverData{ nullptr }
    {}
};

class FMODBank
{
private:
    FMOD::Studio::Bank* m_musicBank;

    std::string m_name;


public:
    FMODBank() :
        m_musicBank{ nullptr },
        m_name{ "" }
    {}

    FMODBank(FMOD::Studio::System* system, std::string& fileName) :
        m_musicBank{ nullptr },
        m_name{ fileName }
    {
        ERRCHECK(system->loadBankFile(Common_MediaPath(fileName.c_str()), FMOD_STUDIO_LOAD_BANK_NORMAL, &m_musicBank));
    }


public:
    const std::string& getName() const noexcept
    {
        return m_name;
    }

    void load(FMOD::Studio::System* system, std::string& fileName)
    {
        ERRCHECK(system->loadBankFile(Common_MediaPath(fileName.c_str()), FMOD_STUDIO_LOAD_BANK_NORMAL, &m_musicBank));
        m_name = fileName;
    }
};


SoundManager::SoundManager() :
    m_systemBloc{ new FMODSystemBloc }
{
    ERRCHECK(FMOD::Studio::System::create(&m_systemBloc->m_system));

    ERRCHECK(m_systemBloc->m_system->getLowLevelSystem(&m_systemBloc->m_lowLevelSystem));
    ERRCHECK(m_systemBloc->m_lowLevelSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0));

    ERRCHECK(m_systemBloc->m_system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, m_systemBloc->m_extraDriverData));
}

SoundManager::~SoundManager()
{
    this->cleanup();
    delete m_systemBloc;
}

void SoundManager::init()
{
    
}

void SoundManager::update()
{

}

void SoundManager::cleanup()
{
    if (m_systemBloc->m_system)
    {
        m_systemBloc->m_system->release();
        m_systemBloc->m_system = nullptr;
    }
}

void SoundManager::loadMusicFromFile(const char* fileName)
{
    std::string name{ fileName };
    for (auto iter = m_musicBank.begin(); iter != m_musicBank.end(); ++iter)
    {
        if (iter->getName().size() == name.size() && iter->getName() == name)
        {
            return;
        }
    }

    m_musicBank.emplace_back(m_systemBloc->m_system, fileName);
}