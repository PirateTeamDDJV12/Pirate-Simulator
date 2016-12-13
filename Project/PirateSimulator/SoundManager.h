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

        /*Set the volume of all musics. Value must be between 0.f and 1.f*/
        void setVolume(float volume);


        /*Play the music at specified Id*/
        void playMusic(size_t id);

        /*Stop the music at specified Id*/
        void stopMusic(size_t id);
        
        /*Pause the music at specified Id*/
        void pauseMusic(size_t id);

        /*Restart a paused music at specified Id*/
        void restartMusic(size_t id);

        /*Get the volume of music at specified Id*/
        float getVolumeMusic(size_t id);

        /*Set the volume of the music at specified Id. Value must be between 0.f and 1.f*/
        void setVolumeMusic(size_t id, float volume);

        /*return if the music at specified Id is currently playing or not*/
        bool isPlayingMusic(size_t id);


        /*Play the music specified by its fileName. Create one if they are no music named fileName*/
        void playMusic(const char* fileName);

        /*Stop the music named filename*/
        void stopMusic(const char* fileName);

        /*Pause the music named filename*/
        void pauseMusic(const char* fileName);

        /*restart a paused music named filename*/
        void restartMusic(const char* fileName);

        /*get the music volume named filename*/
        float getVolumeMusic(const char* fileName);

        /*set the music volume named filename. Value must be between 0.f and 1.f*/
        void setVolumeMusic(const char* fileName, float volume);

        /*return if the music named fileName is currently playing or not*/
        bool isPlayingMusic(const char* fileName);


        /*return the music load count*/
        size_t musicCount() const noexcept;


    private:
        std::vector<FMODBank>::iterator findMusic(const char* fileName);
    };
}

#endif // !SOUNDMANAGER_H_INCLUDED