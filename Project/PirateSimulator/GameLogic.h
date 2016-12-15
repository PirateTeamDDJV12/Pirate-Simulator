#ifndef GAMELOGIC_H_INCLUDED
#define GAMELOGIC_H_INCLUDED

namespace PirateSimulator
{
    class GameLogic
    {
        enum TasksOrder
        {
            TIMETASK,
            INPUTTASK,
            PHYSICSTASK,
            PLAYERTASK,
            RENDERTASK,
            SOUNDTASK,
            SPAWNTASK,
        };

    public:
        GameLogic() = default;


    private:
        GameLogic(GameLogic&) = delete;
        GameLogic& operator=(GameLogic&) = delete;


    public:
        //create the flow of the game
        void createAllTask() const;

        // load all musics in the game and start the title Screen Music music
        void loadMusics() const;

        // start the game music and stop the titleScreen music
        void startGameMusic() const;

        // start the titleScreen music
        void startTitleScreenMusic() const;
      
        //stop the music
        void killEveryMusicFlow() const;
      
        static void cleanAllTasks();
    };
}

#endif // GAMELOGIC_H_INCLUDED