#pragma once
#include "../PetitMoteur3D/PetitMoteur3D/dispositif.h"

#include "Component.h"



namespace PirateSimulator
{
    //  Classe : Mesh
    //
    //  BUT : 	Classe de base de tous nos objets 3D
    //
    class Mesh : public Component
    {
    public:
        // Destructeur
        virtual ~Mesh(void) {}

        virtual void Draw() = 0;

    protected:
        // Constructeur par défaut
        Mesh(void) {}
    };

}
