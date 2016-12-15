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

    char *binaryNbVertex = new char[4];
    is.read(binaryNbVertex, sizeof(unsigned));

    BinaryFast bfTest;
    bfTest.put<BinaryFast::Load>(binaryNbVertex, 0);
    int nbIndices = (Config::getInstance()->getWidth() - 1) * (Config::getInstance()->getHeight() - 1) * 2 * 3 * sizeof(unsigned);
    int sizeChar = bfTest.getUint32() * sizeof(Vertex);
    v.reserve(sizeChar / sizeof(float) + nbIndices / sizeof(unsigned));

    char* allVertex = new char[sizeChar];
    is.read(allVertex, sizeChar);
    int i = 0;
    for(; i < sizeChar; i += 4)
    {
        bfTest.put<BinaryFast::Load>(allVertex, i);
        v.push_back(bfTest.getFloat());
    }
    char* allTriangle = new char[nbIndices];
    is.read(allTriangle, nbIndices);

    for(i = 0; i < nbIndices; i += 4)
    {
        bfTest.put<BinaryFast::Load>(allTriangle, i);
        v.push_back(static_cast<float>(bfTest.getUint32()));
    }

    delete []binaryNbVertex;
    delete []allVertex;
    delete []allTriangle;
    // Close the file
    is.close();
    return v;
}

//std::vector<float> FilesManager::readFileHeightMap(std::string filePath, std::vector<float> &v)
//{
//    std::ifstream ifs(filePath);
//    return v(std::istream_iterator<float>(ifs), {});
//}

std::vector<float> FilesManager::readPiecePositionFile(const std::string& fileName) const
{
    const size_t coordinateCount = 2;

    std::vector<float> pos;
    std::ifstream is{ fileName,std::ios::binary };

    char* posBuffer = new char[4];
    is.read(posBuffer, sizeof(size_t));

    BinaryFast binaryFast;

    size_t posCount;
    binaryFast.put<BinaryFast::Load>(posBuffer, 0);

    delete[] posBuffer;

    posCount = binaryFast.getUint32();
    pos.reserve(posCount);

    size_t totalCount = posCount * sizeof(float) * coordinateCount;

    posBuffer = new char[totalCount];
    
    is.read(posBuffer, totalCount);

    for (size_t offset = 0; offset < totalCount; offset += 4)
    {
        binaryFast.put<BinaryFast::Load>(posBuffer, offset);

        pos.emplace_back(binaryFast.getFloat());
    }

    delete[] posBuffer;

    // Close the file
    is.close();

    return std::move(pos);
}