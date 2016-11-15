#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdexcept>

namespace PirateSimulator
{


    class Config
    {
    public:
        class BadConfigFile : public std::runtime_error
        {
        public:
            BadConfigFile() = delete;
            BadConfigFile(const std::string &error) : std::runtime_error(error)
            {}
        };

    private:

        unsigned int                    m_width;
        unsigned int                    m_height;
        float                           m_scale;
        float                           m_offset;
        float                           m_mapScale;
        std::string                     m_path;
        std::string                     m_exportName;
        std::string                     m_texturePath;
        static Config*                  m_instance;
        unsigned int                    m_textureMappingRepetition;

        Config();

    public:
        Config(const Config &other) = delete;
        Config &operator=(const Config& other) = delete;


        static Config*  getInstance();
        void            initialize();

        int             getWidth() const;
        int             getHeight() const;
        float           getScale() const;
        float           getOffset() const;
        float           getMapScale() const;
        std::string     getExportName() const;
        std::string     getTexturePath() const;
        std::string     getPath() const;
        unsigned int    getTextureMappingRepetition() const;

        ~Config();

    };
}

#endif