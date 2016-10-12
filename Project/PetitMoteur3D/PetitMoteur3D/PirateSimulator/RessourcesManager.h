#ifndef RESSOURCES_MANAGER_H
#define RESSOURCES_MANAGER_H
#include "../Singleton.h"
#include "FilesManager.h"

class RessourcesManager : public PM3D::CSingleton<RessourcesManager>
{
    FilesManager m_filesManager;
public:
    std::string ReadFile(std::string filePath)
    {
        return m_filesManager.readFile(filePath);
    }
    void ReadFile(std::string filePath, std::string &returnString) const
    {
        returnString = m_filesManager.readFile(filePath);
    }
};

#endif // RESSOURCES_MANAGER_H