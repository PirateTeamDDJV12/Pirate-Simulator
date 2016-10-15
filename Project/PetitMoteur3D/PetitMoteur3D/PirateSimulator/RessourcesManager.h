#ifndef RESSOURCES_MANAGER_H
#define RESSOURCES_MANAGER_H
#include "../Singleton.h"
#include "FilesManager.h"

class RessourcesManager : public PM3D::CSingleton<RessourcesManager>
{
    FilesManager m_filesManager;
public:
    std::vector<float> ReadHeightMapFile(std::string filePath) const
    {
        return m_filesManager.readFileHeightMap(filePath);
    }
};

#endif // RESSOURCES_MANAGER_H