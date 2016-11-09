#ifndef RESSROUCE_MANAGER_H
#define RESSROUCE_MANAGER_H
#include "FilesManager.h"
#include "../PetitMoteur3D/PetitMoteur3D/Singleton.h"

namespace PirateSimulator
{
    class RessourcesManager : public PM3D::CSingleton<RessourcesManager>
    {
        PirateSimualtor::FilesManager m_filesManager;
    public:
        std::vector<float> ReadHeightMapFile(std::string filePath) const
        {
            return m_filesManager.readFileHeightMap(filePath);
        }
    };
}


#endif // RESSROUCE_MANAGER_H