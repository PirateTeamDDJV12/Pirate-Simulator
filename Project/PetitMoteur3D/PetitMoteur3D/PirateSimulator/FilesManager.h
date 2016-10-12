#ifndef FILES_MANAGER_H
#define FILES_MANAGER_H
#include <string>

class FilesManager
{
public :
    FilesManager() {}
    std::string readFile(std::string filePath) const;
};

#endif // FILES_MANAGER_H