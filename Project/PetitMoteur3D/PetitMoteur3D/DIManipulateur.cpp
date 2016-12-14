#include <string>

#include "DIManipulateur.h"
#include "util.h"
#include "resource.h"

using namespace UtilitairesDX;

bool CDIManipulateur::bDejaInit = false;

MouseState::MouseState() : m_offsetMouseX{0.0f}, m_offsetMouseY{0.0f}, m_sensibility{0.08f}
{
}

CDIManipulateur::CDIManipulateur(void) :
    pDirectInput{nullptr}, pClavier{nullptr}, pSouris{nullptr}, pJoystick{nullptr}
{
}


CDIManipulateur::~CDIManipulateur(void)
{
    if(pClavier)
    {
        pClavier->Unacquire();
        pClavier->Release();
        pClavier = NULL;
    }

    if(pSouris)
    {
        pSouris->Unacquire();
        pSouris->Release();
        pSouris = NULL;
    }

    if(pJoystick)
    {
        pJoystick->Unacquire();
        pJoystick->Release();
        pJoystick = NULL;
    }

    if(pDirectInput)
    {
        pDirectInput->Release();
        pClavier = NULL;
    }

}

bool CDIManipulateur::Init(HINSTANCE hInstance, HWND hWnd)
{
    // Un seul objet DirectInput est permis
    if(!bDejaInit)
    {
        // Objet DirectInput
        DXEssayer(DirectInput8Create(hInstance,
                                     DIRECTINPUT_VERSION,
                                     IID_IDirectInput8,
                                     (void**)&pDirectInput,
                                     NULL), ERREUR_CREATION_DIRECTINPUT);

        // Objet Clavier
        DXEssayer(pDirectInput->CreateDevice(GUID_SysKeyboard,
                                             &pClavier,
                                             NULL),
                  ERREUR_CREATION_CLAVIER);

        DXEssayer(pClavier->SetDataFormat(&c_dfDIKeyboard),
                  ERREUR_CREATION_FORMATCLAVIER);

        pClavier->SetCooperativeLevel(hWnd,
                                      DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_BACKGROUND | DISCL_NOWINKEY);

        pClavier->Acquire();



        // Objet Souris
        DXEssayer(pDirectInput->CreateDevice(GUID_SysMouse, &pSouris, NULL),
                  ERREUR_CREATION_SOURIS);

        DXEssayer(pSouris->SetDataFormat(&c_dfDIMouse), ERREUR_CREATION_FORMATSOURIS);

        pSouris->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_BACKGROUND | DISCL_NOWINKEY);

        pSouris->Acquire();

        // Objet Joystick

        bDejaInit = true;
    }

    pClavier->GetDeviceState(sizeof(newKeyboardState), (void*)& newKeyboardState);
    pClavier->GetDeviceState(sizeof(oldKeyboardState), (void*)& oldKeyboardState);

    return true;
}

void CDIManipulateur::StatutClavier()
{
    pClavier->GetDeviceState(sizeof(newKeyboardState), (void*)& newKeyboardState);
    for(unsigned int i = 0; i < 256; ++i)
    {
        if(newKeyboardState[i] != oldKeyboardState[i])
        {
            int newKeyState = newKeyboardState[i];
            int oldKeyState = oldKeyboardState[i];
            if(newKeyboardState[i] == 0 && oldKeyboardState != 0)
                keyStatus[i] = KeyStates::KeyUp;
            if(newKeyState != 0 && oldKeyState == 0)
                keyStatus[i] = KeyStates::KeyDown;
        }
        //else if (newKeyboardState[i] == 0)
        //    keyStatus[i] = KeyStates::KeyRealesed;
        //else
        //    keyStatus[i] = KeyStates::KeyHold;
    }
    pClavier->GetDeviceState(sizeof(oldKeyboardState), (void*)& oldKeyboardState);
}

bool CDIManipulateur::getKey(UINT touche) const
{
    return static_cast<bool>(newKeyboardState[touche] & 0x80);
}

bool CDIManipulateur::getButtonDown(UINT touche)
{
    if(keyStatus[touche] == KeyStates::KeyDown)
    {
        keyStatus[touche] = KeyStates::KeyHold;
        return true;
    }
    return false;
}

bool CDIManipulateur::getButtonUp(UINT touche)
{
    if(keyStatus[touche] == KeyStates::KeyUp)
    {
        keyStatus[touche] = KeyStates::KeyRealesed;
        return true;
    }
    return false;
}

void CDIManipulateur::SaisirEtatSouris()
{
	pSouris->GetDeviceState(sizeof(mouseInfo), (void*)&mouseInfo);
    m_mouseState.m_offsetMouseX = mouseInfo.lX;
    m_mouseState.m_offsetMouseY = mouseInfo.lY;
}

