#ifndef SOUNDMANAGER_H_INCLUDED
#define SOUNDMANAGER_H_INCLUDED

#include <vector>

struct FMODSystemBloc;

class FMODBank;


namespace PirateSimulator
{
    class SoundManager
    {
    public:
        enum
        {
            CHANNEL_MAX_COUNT = 1024
        };


    public:
        static SoundManager singleton;
        

    private:
        FMODSystemBloc* m_systemBloc;

        std::vector<FMODBank> m_musicBank;


    private:
        SoundManager();
        SoundManager(SoundManager&) = delete;
        SoundManager& operator=(SoundManager&) = delete;


    public:
        ~SoundManager();


    public:
        void init();
        void update();
        void cleanup();


    public:
        void loadMusicFromFile(const char* fileName);
        void setVolume(float volume);

        void playMusic(size_t id);
        void stopMusic(size_t id);

        void playMusic(const char* fileName);
        void stopMusic(const char* fileName);
    };
}

#endif // !SOUNDMANAGER_H_INCLUDED