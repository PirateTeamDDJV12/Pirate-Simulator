#pragma once
class CDIManipulateur
{
public:
	CDIManipulateur(void);
	~CDIManipulateur(void);

	bool CDIManipulateur::Init(HINSTANCE hInstance, HWND hWnd);
	void StatutClavier();
	bool ToucheAppuyee(UINT touche) const;
	void SaisirEtatSouris();

	const DIMOUSESTATE& EtatSouris() { return mouseState;}

protected:
	IDirectInput8* pDirectInput;
	IDirectInputDevice8* pClavier;
	IDirectInputDevice8* pSouris;
	IDirectInputDevice8* pJoystick;

	static bool bDejaInit;

	char tamponClavier[256];
	DIMOUSESTATE mouseState;
};

