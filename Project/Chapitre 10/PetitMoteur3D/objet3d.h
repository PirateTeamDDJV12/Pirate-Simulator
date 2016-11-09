#pragma once
#include "dispositif.h"

#include "../../PirateSimulator/Component.h"



namespace PM3D
{
    //  Classe : CObjet3D
    //
    //  BUT : 	Classe de base de tous nos objets 3D
    //
    class CObjet3D : public PirateSimulator::Component
    {
    public:
        // Destructeur
        virtual ~CObjet3D(void) {}


        virtual void Anime(float elapsedTime) {}
        virtual void Draw() = 0;

    protected:
        // Constructeur par défaut
        CObjet3D(void) {}
    };

}
