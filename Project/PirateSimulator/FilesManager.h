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
        
        //read the piece position from a binary file named filename and return a vector of float representing one part of the coordinate 
        //(the value at odd position is the z coordinate and the even position value is x (LH coordinate))
        std::vector<float> readPiecePositionFile(const std::string& fileName) const;

        static std::vector<float> readFileHeightMap(std::string filePath);
    };
}

#endif // FILES_MANAGER_H