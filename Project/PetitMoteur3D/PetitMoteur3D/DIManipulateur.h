#pragma once

struct MouseState
{
    MouseState();

    float       m_offsetMouseX;
    float       m_offsetMouseY;
    float       m_sensibility;
};

class CDIManipulateur
{
public:
	CDIManipulateur(void);
	~CDIManipulateur(void);

	bool CDIManipulateur::Init(HINSTANCE hInstance, HWND hWnd);
	void StatutClavier();
	bool ToucheAppuyee(UINT touche) const;
	void SaisirEtatSouris();

	const MouseState& EtatSouris() { return m_mouseState;}

protected:
	IDirectInput8* pDirectInput;
	IDirectInputDevice8* pClavier;
	IDirectInputDevice8* pSouris;
	IDirectInputDevice8* pJoystick;

	static bool bDejaInit;

	char tamponClavier[256];
	DIMOUSESTATE mouseInfo;
    MouseState  m_mouseState;
};

