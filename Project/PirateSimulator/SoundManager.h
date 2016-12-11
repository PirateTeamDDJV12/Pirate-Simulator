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

        static constexpr const float MAX_VOLUME     = 1.0f;
        static constexpr const float MUTE_VOLUME    = 0.f;


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
        void loadMusicFromFile(const char* fileName, int mode);

        void setVolume(float volume);

        void playMusic(size_t id);
        void stopMusic(size_t id);
        void pauseMusic(size_t id);
        void restartMusic(size_t id);
        float getVolumeMusic(size_t id);
        void setVolumeMusic(size_t id, float volume);
        bool isPlayingMusic(size_t id);

        void playMusic(const char* fileName);
        void stopMusic(const char* fileName);
        void pauseMusic(const char* fileName);
        void restartMusic(const char* fileName);
        float getVolumeMusic(const char* fileName);
        void setVolumeMusic(const char* fileName, float volume);
        bool isPlayingMusic(const char* fileName);


    private:
        std::vector<FMODBank>::iterator findMusic(const char* fileName);
    };
}

#endif // !SOUNDMANAGER_H_INCLUDED