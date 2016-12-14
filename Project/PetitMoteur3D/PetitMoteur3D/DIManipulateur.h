#pragma once
#include <dinput.h>

enum KeyStates
{
    KeyHold,
    KeyDown,
    KeyUp,
    KeyRealesed
};


class CDIManipulateur
{
public:
	CDIManipulateur(void);
	~CDIManipulateur(void);

	bool CDIManipulateur::Init(HINSTANCE hInstance, HWND hWnd);
	void StatutClavier();
	bool getKey(UINT touche) const;
    bool getButtonDown(UINT touche);
    bool getButtonUp(UINT touche);
	void SaisirEtatSouris();

	const DIMOUSESTATE& EtatSouris() { return mouseState;}

protected:
	IDirectInput8* pDirectInput;
	IDirectInputDevice8* pClavier;
	IDirectInputDevice8* pSouris;
	IDirectInputDevice8* pJoystick;

	static bool bDejaInit;

    char newKeyboardState[256];
    char oldKeyboardState[256];
    unsigned int keyStatus[256];
    DIMOUSESTATE mouseState;
};

