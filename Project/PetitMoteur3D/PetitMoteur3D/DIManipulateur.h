#pragma once
#include <dinput.h>

struct MouseState
{
    MouseState();

    float       m_offsetMouseX;
    float       m_offsetMouseY;
    float       m_sensibility;
};

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

	const MouseState& EtatSouris() { return m_mouseState;}

protected:
	IDirectInput8* pDirectInput;
	IDirectInputDevice8* pClavier;
	IDirectInputDevice8* pSouris;
	IDirectInputDevice8* pJoystick;

	static bool bDejaInit;

	//char tamponClavier[256];
	DIMOUSESTATE mouseInfo;
    MouseState  m_mouseState;
    char newKeyboardState[256];
    char oldKeyboardState[256];
    unsigned int keyStatus[256];
    //DIMOUSESTATE mouseState;
};

