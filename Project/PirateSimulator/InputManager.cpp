#include "InputManager.h"

#include "..\PetitMoteur3D\PetitMoteur3D\MoteurWindows.h"

using PirateSimulator::InputManager;
using namespace PM3D;


InputManager InputManager::singleton;

CDIManipulateur& PirateSimulator::InputManager::Key::m_gestionnaireDeSaisie = InputManager::singleton.getManipulator();


void PirateSimulator::InputManager::Key::update()
{
    lastState = currentState;
    currentState = static_cast<State>(m_gestionnaireDeSaisie.ToucheAppuyee(keyValue));
}

void InputManager::update()
{
    // Prendre en note le statut du clavier
    m_manipulator.StatutClavier();

    // Prendre en note l'état de la souris
    m_manipulator.SaisirEtatSouris();

    for(size_t iter = KEY1; iter < NBKEYS; ++iter)
    {
        keys[iter].update();
    }
}