#ifndef FILES_MANAGER_H
#define FILES_MANAGER_H
#include <vector>

namespace PirateSimulator
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