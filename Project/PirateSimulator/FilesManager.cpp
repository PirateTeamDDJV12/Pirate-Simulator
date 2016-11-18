#include "FilesManager.h"
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include "BinaryFast.h"
#include "Vertex.h"
#include "../PetitMoteur3D/PetitMoteur3D/Config/Config.hpp"

using namespace PirateSimulator;

std::string FilesManager::readFile(std::string filePath) const
{
    std::string allFile;
    std::ifstream infile;
    infile.open(filePath);
    while(!infile.eof()) // To get you all the lines.
    {
        std::string temp;
        std::getline(infile, temp); // Saves the line in STRING.
        allFile += temp + "_";
    }
    infile.close();

    return allFile;
}

std::vector<float> FilesManager::readFileHeightMap(std::string filePath)
{
    std::vector<float> v;
    std::ifstream is{filePath,std::ios::binary};

    char *test = new char[4];
    is.read(test, sizeof(unsigned));

    BinaryFast bfTest;
    bfTest.put<BinaryFast::Load>(test, 0);
    int nbIndices = (Config::getInstance()->getWidth() - 1) * (Config::getInstance()->getHeight() - 1) * 2 * 3 * sizeof(unsigned);
    int sizeChar = bfTest.getUint32() * sizeof(Vertex);
    v.reserve(sizeChar / sizeof(float) + nbIndices / sizeof(unsigned));

    char* allVertex = new char[sizeChar];
    is.read(allVertex, sizeChar);
    int i = 0;
    for(; i < sizeChar; i += 4)
    {
        bfTest.put<BinaryFast::Load>(allVertex, i);
        //bfTest.put<BinaryFast::Save>(reinterpret_cast<char*>(v.data()), i);
        v.push_back(bfTest.getFloat());
    }
    char* allTriangle = new char[nbIndices];
    is.read(allTriangle, nbIndices);

    for(i = 0; i < nbIndices; i += 4)
    {
        bfTest.put<BinaryFast::Load>(allTriangle, i);
        v.push_back(bfTest.getUint32());
    }

    // Close the file
    is.close();
    auto ipoawdjaiow = v.back();
    return v;
}

//std::vector<float> FilesManager::readFileHeightMap(std::string filePath, std::vector<float> &v)
//{
//    std::ifstream ifs(filePath);
//    return v(std::istream_iterator<float>(ifs), {});
//}
