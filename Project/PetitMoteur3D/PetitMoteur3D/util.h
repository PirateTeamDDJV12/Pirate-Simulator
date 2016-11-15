namespace UtilitairesDX
{
    // Essayer en envoyant le code d'erreur comme résultat
    // Il ne faut pas oublier de "rattraper" le code...
    template <class Type>
    inline void DXEssayer(const Type& Resultat)
    {
        if(Resultat != S_OK)
            throw Resultat;
    }

    // Plus pratique, essayer en envoyant un code spécifique 
    // comme résultat
    template <class Type1, class Type2>
    inline void DXEssayer(const Type1& Resultat, const Type2& unCode)
    {
        if(Resultat != S_OK)
            throw unCode;
    }

    // Valider un pointeur
    template <class Type>
    inline void DXValider(const void* UnPointeur, const Type& unCode)
    {
        if(UnPointeur == NULL)
            throw unCode;
    }

    // Relâcher un objet COM (un objet DirectX dans notre cas)
    template <class Type>
    inline void DXRelacher(Type& UnPointeur)
    {
        if(UnPointeur != NULL)
        {
            UnPointeur->Release();
            UnPointeur = NULL;
        }
    }


    inline int roundNum(int numToRound, int multiple, bool up = true)
    {
        if(multiple == 0)
            return numToRound;

        int remainder = abs(numToRound) % multiple;
        if(remainder == 0)
            return numToRound;


        // Ceil
        if(up)
        {
            if(numToRound < 0)
            {
                return -(abs(numToRound) - remainder);
            }

            return numToRound + multiple - remainder;
        }

        // Floor
        if(numToRound < 0)
        {
            return -(abs(numToRound) + multiple - remainder);
        }
        return numToRound - remainder;
    }
}
