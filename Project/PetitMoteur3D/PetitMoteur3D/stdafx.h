// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once

// Modifiez les définitions suivantes si vous devez cibler une plate-forme avant celles spécifiées ci-dessous.
// Reportez-vous à MSDN pour obtenir les dernières informations sur les valeurs correspondantes pour les différentes plates-formes.
#ifndef WINVER				// Autorise l'utilisation des fonctionnalités spécifiques à Windows XP ou version ultérieure.
#define WINVER 0x0501		// Attribuez la valeur appropriée à cet élément pour cibler d'autres versions de Windows.
#endif

#ifndef _WIN32_WINNT		// Autorise l'utilisation des fonctionnalités spécifiques à Windows XP ou version ultérieure.                   
#define _WIN32_WINNT 0x0501	// Attribuez la valeur appropriée à cet élément pour cibler d'autres versions de Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Autorise l'utilisation des fonctionnalités spécifiques à Windows 98 ou version ultérieure.
#define _WIN32_WINDOWS 0x0410 // Attribuez la valeur appropriée à cet élément pour cibler Windows Me ou version ultérieure.
#endif

#ifndef _WIN32_IE			// Autorise l'utilisation des fonctionnalités spécifiques à Internet Explorer 6.0 ou version ultérieure.
#define _WIN32_IE 0x0600	// Attribuez la valeur appropriée à cet élément pour cibler d'autres versions d'Internet Explorer.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclure les en-têtes Windows rarement utilisés
// Fichiers d'en-tête Windows :
#include <windows.h>

// Fichiers d'en-tête C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// certains constructeurs CString seront explicites

#include <atlbase.h>
#include <atlstr.h>

// TODO : faites référence ici aux en-têtes supplémentaires nécessaires au programme
#include <mmsystem.h>
#define _XM_NO_INTRINSICS_
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include <dinput.h>

//Fichier PhysX
#include "targetver.h"
#include <cassert>
#include "PhysX/Include/PxPhysicsAPI.h"
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#define SAFE_RELEASE(_x) { (_x)->Release(); (_x) = nullptr; }

#ifdef _DEBUG
#define ASSERT(x) assert(x)
#else
#define ASSERT(x) do { } while (false)
#endif

#ifndef _NOPCH

// TODO: reference additional headers your program requires here

#endif // _NOPCH

