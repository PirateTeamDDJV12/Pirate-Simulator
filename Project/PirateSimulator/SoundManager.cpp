#include "SoundManager.h"

#include "FMOD/fmod_studio.hpp"
#include "FMOD/fmod.hpp"
#include "FMOD/common.h"

using namespace PirateSimulator;


SoundManager SoundManager::singleton;


class ErrorFMOD {};

template<class Result>
void FMODTry(Result result)
{
#ifdef _DEBUG
    if (result != FMOD_OK)
    {
        throw ErrorFMOD{};
    }
#endif
}


struct FMODSystemBloc
{
public:
    FMOD::System* m_lowLevelSystem;

    void* m_extraDriverData;


public:
    FMODSystemBloc() :
        m_lowLevelSystem{ nullptr },
        m_extraDriverData{ nullptr }
    {}
};

class FMODBank
{
private:
    FMOD::Sound* m_musicSound;
    FMOD::Channel* m_musicChannel;

    std::string m_name;


public:
    FMODBank() :
        m_musicSound{ nullptr },
        m_name{ "" },
        m_musicChannel{ nullptr }
    {}

    FMODBank(FMOD::System* system, const std::string& fileName) :
        m_musicSound{ nullptr },
        m_musicChannel{ nullptr }
    {
        this->load(system, fileName);
    }

    ~FMODBank()
    {
        this->unload();
    }


public:
    const std::string& getName() const noexcept
    {
        return m_name;
    }

    void load(FMOD::System* system, const std::string& fileName)
    {
        FMODTry(system->createSound(fileName.c_str(), FMOD_DEFAULT, 0, &m_musicSound));

        m_name = fileName;
    }

    void unload()
    {
        if (m_musicSound)
        {
            m_musicSound->release();
            m_musicSound = nullptr;
            m_name = "";
        }
    }

    void setVolume(float volume)
    {
        m_musicChannel->setVolume(volume);
    }

    void play(FMOD::System* system)
    {
        system->playSound(m_musicSound, nullptr, false, &m_musicChannel);
    }

    void stop()
    {
        if (m_musicChannel)
        {
            m_musicChannel->stop();
        }
    }
};


SoundManager::SoundManager() :
    m_systemBloc{ new FMODSystemBloc }
{
    FMODTry(FMOD::System_Create(&m_systemBloc->m_lowLevelSystem));

    FMODTry(m_systemBloc->m_lowLevelSystem->init(SoundManager::CHANNEL_MAX_COUNT, FMOD_INIT_NORMAL, m_systemBloc->m_extraDriverData));
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
    m_systemBloc->m_lowLevelSystem->update();
}

void SoundManager::cleanup()
{
    for (auto iter = m_musicBank.begin(); iter != m_musicBank.end(); ++iter)
    {
        iter->unload();
    }

    if (m_systemBloc->m_lowLevelSystem)
    {
        m_systemBloc->m_lowLevelSystem->release();
        m_systemBloc->m_lowLevelSystem = nullptr;
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

    m_musicBank.emplace_back(m_systemBloc->m_lowLevelSystem, name);
}

void SoundManager::setVolume(float volume)
{
    for (auto iter = m_musicBank.begin(); iter != m_musicBank.end(); ++iter)
    {
        iter->setVolume(volume);
    }
}

void SoundManager::playMusic(size_t id)
{
    m_musicBank[id].play(m_systemBloc->m_lowLevelSystem);
}

void SoundManager::stopMusic(size_t id)
{
    m_musicBank[id].stop();
}

void SoundManager::playMusic(const char* fileName)
{
    std::string name{ fileName };
    for (auto iter = m_musicBank.begin(); iter != m_musicBank.end(); ++iter)
    {
        if (iter->getName().size() == name.size() && iter->getName() == name)
        {
            iter->play(m_systemBloc->m_lowLevelSystem);
            break;
        }
    }
}

void SoundManager::stopMusic(const char* fileName)
{
    std::string name{ fileName };
    for (auto iter = m_musicBank.begin(); iter != m_musicBank.end(); ++iter)
    {
        if (iter->getName().size() == name.size() && iter->getName() == name)
        {
            iter->stop();
            break;
        }
    }
}