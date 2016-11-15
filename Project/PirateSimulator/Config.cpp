#include <stdafx.h>

#include <fstream>
#include <sstream>

#include "Config.h"

using PirateSimulator::Config;

PirateSimulator::Config* Config::m_instance;

Config::Config() : m_width{0}, m_height{0}, m_scale{0.0f}, m_path{}, m_offset{0.0f}
{
    initialize();
}

void Config::initialize()
{
    std::ifstream configFile("PirateSimulator/heightmap.conf");
    std::string type;
    std::string value;
    std::stringstream ss;


    if(configFile.bad() || configFile.fail())
    {
        throw BadConfigFile("ERROR : Wrong config file path");
    }
    while(std::getline(configFile, type, '='))
    {
        std::getline(configFile, value);

        ss << value;

        if(type == "path")
        {
            ss >> m_path;
        }
        else if(type == "width")
        {
            ss >> m_width;
        }
        else if(type == "height")
        {
            ss >> m_height;
        }
        else if(type == "scale")
        {
            ss >> m_scale;
        }
        else if(type == "repeat")
        {
            ss >> m_textureMappingRepetition;
        }
        else if(type == "offset")
        {
            ss >> m_offset;
        }
        else if(type == "mapscale")
        {
            ss >> m_mapScale;
        }
        else if(type == "exportName")
        {
            ss >> m_exportName;
        }
        else if(type == "texturePath")
        {
            ss >> m_texturePath;
        }
        ss.str(std::string());
        ss.clear();
    }
    if(m_width != m_height || m_width == 0 || m_path.empty())
    {
        throw BadConfigFile("ERROR : Wrong config file content");
    }
}

Config* Config::getInstance()
{
    if(m_instance == nullptr)
    {
        m_instance = new Config;
    }

    return m_instance;
}

int Config::getWidth() const
{
    return m_width;
}

int Config::getHeight() const
{
    return m_height;
}

float Config::getScale() const
{
    return m_scale;
}

float Config::getOffset() const
{
    return m_offset;
}

float Config::getMapScale() const
{
    return m_mapScale;
}

std::string Config::getExportName() const
{
    return m_exportName;
}

std::string Config::getTexturePath() const
{
    return m_texturePath;
}

std::string Config::getPath() const
{
    return m_path;
}

unsigned int Config::getTextureMappingRepetition() const
{
    return m_textureMappingRepetition;
}

Config::~Config()
{}
