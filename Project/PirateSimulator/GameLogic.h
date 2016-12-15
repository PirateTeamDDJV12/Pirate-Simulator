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
        //create the flow of the game
        static void createAllTask();

        static void cleanAllTasks();
    };
}

#endif // GAMELOGIC_H_INCLUDED