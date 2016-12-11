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

    FMODBank(FMOD::System* system, const std::string& fileName, int mode) :
        m_musicSound{ nullptr },
        m_musicChannel{ nullptr }
    {
        this->load(system, fileName, mode);
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

    void load(FMOD::System* system, const std::string& fileName, int mode)
    {
        FMODTry(system->createSound(fileName.c_str(), mode, 0, &m_musicSound));

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

    void play(FMOD::System* system)
    {
        FMODTry(system->playSound(m_musicSound, nullptr, false, &m_musicChannel));
    }

    void stop()
    {
        if (m_musicChannel)
        {
            FMODTry(m_musicChannel->stop());
        }
    }

    void pause()
    {
        if (m_musicChannel)
        {
            FMODTry(m_musicChannel->setPaused(true));
        }
    }

    void restart()
    {
        if (m_musicChannel)
        {
            FMODTry(m_musicChannel->setPaused(false));
        }
    }

    bool isPlaying()
    {
        if (m_musicChannel)
        {
            bool playing;
            
            FMODTry(m_musicChannel->isPlaying(&playing));

            return playing;
        }
    }
    

public:
    void setVolume(float volume)
    {
        if (m_musicChannel)
        {
            FMODTry(m_musicChannel->setVolume(volume));
        }
    }

    float getVolume() const
    {
        float volume;
        
        FMODTry(m_musicChannel->getVolume(&volume));

        return volume;
    }
};


SoundManager::SoundManager() :
    m_systemBloc{ new FMODSystemBloc }
{
    m_musicBank.reserve(SoundManager::CHANNEL_MAX_COUNT);

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
    SoundManager::singleton.loadMusicFromFile("PirateSimulator/UISoundtrack.mp3", FMOD_LOOP_NORMAL);
    SoundManager::singleton.playMusic(static_cast<size_t>(0));
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

void SoundManager::loadMusicFromFile(const char* fileName, int mode)
{
    std::string name{ fileName };
    for (auto iter = m_musicBank.begin(); iter != m_musicBank.end(); ++iter)
    {
        if (iter->getName().size() == name.size() && iter->getName() == name)
        {
            return;
        }
    }

    m_musicBank.emplace_back(m_systemBloc->m_lowLevelSystem, name, mode);
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

void SoundManager::pauseMusic(size_t id)
{
    m_musicBank[id].pause();
}

void SoundManager::restartMusic(size_t id)
{
    m_musicBank[id].restart();
}

float SoundManager::getVolumeMusic(size_t id)
{
    return m_musicBank[id].getVolume();
}

void SoundManager::setVolumeMusic(size_t id, float volume)
{
    if (volume > MAX_VOLUME)
    {
        m_musicBank[id].setVolume(MAX_VOLUME);
    }
    else if (volume < MUTE_VOLUME)
    {
        m_musicBank[id].setVolume(MUTE_VOLUME);
    }
    else
    {
        m_musicBank[id].setVolume(volume);
    }
}

bool SoundManager::isPlayingMusic(size_t id)
{
    return m_musicBank[id].isPlaying();
}

void SoundManager::playMusic(const char* fileName)
{
    std::string name{ fileName };
    auto iter = m_musicBank.begin();

    for (; iter != m_musicBank.end(); ++iter)
    {
        if (iter->getName().size() == name.size() && iter->getName() == name)
        {
            iter->play(m_systemBloc->m_lowLevelSystem);
            return;
        }
    }

    this->loadMusicFromFile(fileName);
    m_musicBank.back().play(m_systemBloc->m_lowLevelSystem);
}

void SoundManager::stopMusic(const char* fileName)
{
    auto music = this->findMusic(fileName);
    if (music != m_musicBank.end())
    {
        music->stop();
    }
}

void SoundManager::pauseMusic(const char* fileName)
{
    auto music = this->findMusic(fileName);
    if (music != m_musicBank.end())
    {
        music->pause();
    }
}

void SoundManager::restartMusic(const char* fileName)
{
    auto music = this->findMusic(fileName);
    if (music != m_musicBank.end())
    {
        music->restart();
    }
}

float SoundManager::getVolumeMusic(const char* fileName)
{
    auto music = this->findMusic(fileName);
    if (music != m_musicBank.end())
    {
        return music->getVolume();
    }

    return MUTE_VOLUME;
}

void SoundManager::setVolumeMusic(const char* fileName, float volume)
{
    auto music = this->findMusic(fileName);
    if (music != m_musicBank.end())
    {
        if (volume > MAX_VOLUME)
        {
            music->setVolume(MAX_VOLUME);
        }
        else if (volume < MUTE_VOLUME)
        {
            music->setVolume(MUTE_VOLUME);
        }
        else
        {
            music->setVolume(volume);
        }
    }
}

bool SoundManager::isPlayingMusic(const char* fileName)
{
    auto music = this->findMusic(fileName);
    if (music != m_musicBank.end())
    {
        return music->isPlaying();
    }

    return false;
}

std::vector<FMODBank>::iterator SoundManager::findMusic(const char* fileName)
{
    std::string name{ fileName };

    auto iter = m_musicBank.begin();

    for (; iter != m_musicBank.end(); ++iter)
    {
        if (iter->getName().size() == name.size() && iter->getName() == name)
        {
            break;
        }
    }

    return iter;
}