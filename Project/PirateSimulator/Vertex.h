/*
Created by Sun-lay gagneux
*/
#ifndef VERTEX_H
#define VERTEX_H


#include "Vect3f.h"
#include "Vect2f.h"

#include <string>
#include <sstream>


namespace PirateSimulator
{
    class Vertex
    {
	public:
		enum
		{
			INFO_COUNT = 2 * Vect3f::INFO_COUNT + Vect2f::INFO_COUNT
		};


    private:
        Vect3f m_position;
        Vect3f m_normal;
        Vect2f m_textureCoordinate;


    public:
        Vertex() noexcept : 
            m_position{},
            m_normal{},
            m_textureCoordinate{}
        {}

        Vertex(float xPos, float yPos, float zPos, float xNormal, float yNormal, float zNormal, float uText, float vText) noexcept :
            m_position{ xPos, yPos, zPos },
            m_normal{ xNormal, yNormal, zNormal },
            m_textureCoordinate{ uText, vText }
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

        void setTextureCoordinate(const Vect2f& textCoord) noexcept
        {
            m_textureCoordinate = textCoord;
        }

        void setTextureCoordinate(float u, float v) noexcept
        {
            m_textureCoordinate.m_U = u;
            m_textureCoordinate.m_V = v;
        }

        Vect3f position() const noexcept
        {
            return m_position;
        }

        Vect3f normalVector() const noexcept
        {
            return m_normal;
        }

        const Vect2f& getTextureCoordinate() const noexcept
        {
            return m_textureCoordinate;
        }

        /*
        Method that returns a std::string that looks like : posX posY posZ normalX normalY normalZ\n
        */
        std::string toString() const noexcept;


    private:
        void writeVectIntoStdString(std::stringstream& container, const Vect3f& valueToWrite) const noexcept;

        void writeVectIntoStdString(std::stringstream& container, const Vect2f& valueToWrite) const noexcept;


    public:
        bool operator==(const Vertex& other) const noexcept
        {
            return (position() == other.position() && normalVector() == other.normalVector() && getTextureCoordinate() == other.getTextureCoordinate());
        }

        bool operator!=(const Vertex& other) const noexcept
        {
            return !(*this == other);
        }
    };
}


#endif //!VERTEX_H