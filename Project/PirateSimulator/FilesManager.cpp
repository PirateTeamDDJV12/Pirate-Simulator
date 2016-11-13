#include "stdafx.h"

#include "FilesManager.h"
#include <fstream>
#include <vector>
#include <iterator>
#include <string>

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
    std::ifstream ifs(filePath);
    return std::vector<float>(std::istream_iterator<float>(ifs), {});
}
