#include "stdafx.h"

#include "FilesManager.h"
#include <fstream>

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
