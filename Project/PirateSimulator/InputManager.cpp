#include "InputManager.h"

#include "..\PetitMoteur3D\PetitMoteur3D\MoteurWindows.h"

using PirateSimulator::InputManager;
using namespace PM3D;


InputManager InputManager::singleton;

CDIManipulateur& PirateSimulator::InputManager::Key::m_gestionnaireDeSaisie = 
                                  CMoteurWindows::GetInstance().GetGestionnaireDeSaisie();


void PirateSimulator::InputManager::Key::update()
{
    lastState = currentState;
    currentState = static_cast<State>(m_gestionnaireDeSaisie.ToucheAppuyee(keyValue));
}
