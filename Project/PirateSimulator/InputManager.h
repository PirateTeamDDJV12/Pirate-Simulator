#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <wtypes.h>

#include "..\PetitMoteur3D\PetitMoteur3D\DIManipulateur.h"


namespace PirateSimulator
{
    class InputManager
    {
    public:
        enum KeyMapperEnum
        {
            KEY1,
            KEY2,
            KEY3,
            KEY4,

            NBKEYS
        };


    public:
        struct Key
        {
        public:
            enum State : bool
            {
                PRESSED = true,
                RELEASED = false
            };


        public:
            State lastState;
            State currentState;
            UINT keyValue;


        private:
            static CDIManipulateur& m_gestionnaireDeSaisie;


        public:
            void setKey(UINT key) noexcept { keyValue = key; };
            void update();

            bool onPressed() const noexcept { return lastState == RELEASED && currentState != lastState; }
            bool onReleased() const noexcept { return lastState == PRESSED && currentState != lastState; }
        };


    public:
        static InputManager singleton;


    private:
        Key keys[NBKEYS];


    private:
        InputManager() {}

        InputManager(const InputManager&) = delete;
        InputManager& operator=(const InputManager&) = delete;
        CDIManipulateur m_manipulator;

    public:
        Key::State getKey(KeyMapperEnum keyToGet) const noexcept { return keys[keyToGet].currentState; }
        void setKey(KeyMapperEnum keyToSet, UINT newKeyValue) noexcept { keys[keyToSet].setKey(newKeyValue); }

        void update();

        CDIManipulateur& getManipulator() noexcept
        {
            return m_manipulator;
        }

        void InitManipulator(HINSTANCE hAppInstance, HWND hMainWnd)
        {
            m_manipulator.Init(hAppInstance, hMainWnd);
        }
    };
}

#endif // !INPUTMANAGER_H
