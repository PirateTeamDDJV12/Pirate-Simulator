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
        };


    public:
        //create the flow of the game
        static void createAllTask();
    };
}

#endif // GAMELOGIC_H_INCLUDED