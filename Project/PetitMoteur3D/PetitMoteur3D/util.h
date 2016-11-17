#ifndef _UTIL_H_
#define _UTIL_H_

#include <string>

namespace UtilitairesDX
{
	// Essayer en envoyant le code d'erreur comme résultat
	// Il ne faut pas oublier de "rattraper" le code...
	template <class Type>
	inline void DXEssayer(const Type& Resultat)
	{
		if (Resultat != S_OK)
			throw Resultat;
	}

	// Plus pratique, essayer en envoyant un code spécifique 
  	// comme résultat
	template <class Type1, class Type2>
	inline void DXEssayer(const Type1& Resultat, const Type2& unCode)
	{
		if (Resultat != S_OK)
			throw unCode;
	}

	// Valider un pointeur
	template <class Type>
	inline void DXValider(const void* UnPointeur, const Type& unCode)
	{
		if (UnPointeur == NULL)
			throw unCode;
	}

	// Relâcher un objet COM (un objet DirectX dans notre cas)
    template <class Type>
    inline void DXRelacher(Type& UnPointeur)
	{
		if (UnPointeur != NULL)
		{
			UnPointeur->Release(); 
			UnPointeur=NULL;
		}
	}

    inline std::wstring s2ws(const std::string& s)
    {
        int len;
        int slength = (int)s.length() + 1;
        len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
        wchar_t* buf = new wchar_t[len];
        MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
        std::wstring r(buf);
        delete[] buf;
        return r;
    }
}

#endif // !_UTIL_H_

