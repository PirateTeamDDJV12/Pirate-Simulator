#ifndef GAMELOGIC_H_INCLUDED
#define GAMELOGIC_H_INCLUDED

namespace PirateSimulator
{
    class Transform;

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


    public:
        //create the flow of the game
        void createAllTask();

        void loadMusics();
    };
}

#endif // GAMELOGIC_H_INCLUDED