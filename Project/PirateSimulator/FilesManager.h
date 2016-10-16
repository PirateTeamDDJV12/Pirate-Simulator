#ifndef FILES_MANAGER_H
#define FILES_MANAGER_H
#include <vector>

namespace PirateSimualtor
{
    class FilesManager
    {
    public:
        FilesManager()
        {}
        std::string readFile(std::string filePath) const;
        static std::vector<float> readFileHeightMap(std::string filePath);
    };
}

#endif // FILES_MANAGER_H