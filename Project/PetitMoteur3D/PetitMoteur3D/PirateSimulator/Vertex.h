#ifndef VERTEX_H
#define VERTEX_H


#include "Vect3f.h"

#include <string>
#include <sstream>


namespace PirateSimulator
{
    class Vertex
    {
    private:
        const char SPACE_SEPARATOR = ' ';
        const char LINE_JUMP_SEPARATOR = '\n';


    private:
        Vect3f m_position;
        Vect3f m_normal;


    public:
        Vertex() noexcept : 
            m_position{},
            m_normal{}
        {}

        Vertex(float xPos, float yPos, float zPos, float xNormal, float yNormal, float zNormal) noexcept :
            m_position{xPos, yPos, zPos},
            m_normal{xNormal, yNormal, zNormal}
        {}


    public:
        Vect3f& position() noexcept
        {
            return m_position;
        }

        Vect3f& normalVector() noexcept
        {
            return m_normal;
        }

        Vect3f position() const noexcept
        {
            return m_position;
        }

        Vect3f normalVector() const noexcept
        {
            return m_normal;
        }

        /*
        Method that returns a std::string that looks like : posX posY posZ normalX normalY normalZ\n
        */
        std::string toString() const noexcept;


    private:
        void writeVectIntoStdString(std::stringstream& container, const Vect3f& valueToWrite) const noexcept;


    public:
        bool operator==(const Vertex& other) const noexcept
        {
            return (position() == other.position() && normalVector() == other.normalVector());
        }

        bool operator!=(const Vertex& other) const noexcept
        {
            return !(*this == other);
        }
    };
}


#endif //!VERTEX_H