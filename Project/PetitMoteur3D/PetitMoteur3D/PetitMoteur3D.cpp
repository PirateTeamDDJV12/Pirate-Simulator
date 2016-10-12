// PetitMoteur3D.cpp : définit le point d'entrée pour l'application.
//

#include "stdafx.h"
#include "PetitMoteur3D.h"
#include "MoteurWindows.h"
#include <exception>


using namespace PM3D;


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	// Pour ne pas avoir d'avertissement
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

    try
    {
        // Création de l'objet Moteur
		CMoteurWindows& rMoteur = CMoteurWindows::GetInstance();

		// Spécifiques à une application Windows
		rMoteur.SetWindowsAppInstance(hInstance);

		// Initialisation du moteur
		rMoteur.Initialisations();

		// Boucle d'application
		rMoteur.Run();

		return (int)1;
    }

	catch (const std::exception& E)
    {
		wchar_t message[35];     
		mbtowc( message, E.what(), 34 );

        MessageBox(NULL, message, L"Erreur", MB_ICONWARNING);
	    
		return (int)99;
    }

	catch (int codeErreur)
  	{
	wchar_t szErrMsg[MAX_LOADSTRING];	// Un message d'erreur selon le code

		::LoadString(hInstance, codeErreur, szErrMsg, MAX_LOADSTRING);
		::MessageBox(NULL,szErrMsg,L"Erreur",MB_ICONWARNING);
	  
		return (int)99; // POURQUOI 99???
	}

}




