#include "Vertex.h"


using namespace PirateSimulator;


void Vertex::writeVectIntoStdString(std::stringstream& container, const Vect3f& valueToWrite) const noexcept
{
    container << valueToWrite.x() << ' ';
    container << valueToWrite.y() << ' ';
    container << valueToWrite.z();
}

void Vertex::writeVectIntoStdString(std::stringstream& container, const Vect2f& valueToWrite) const noexcept
{
    container << valueToWrite.m_U << ' ';
    container << valueToWrite.m_V << ' ';
}

std::string Vertex::toString() const noexcept
{
    std::stringstream intermediaryStr;

    writeVectIntoStdString(intermediaryStr, position());
    intermediaryStr << ' ';

    writeVectIntoStdString(intermediaryStr, normalVector());
    intermediaryStr << ' ';

    writeVectIntoStdString(intermediaryStr, getTextureCoordinate());
    intermediaryStr << '\n';

    return intermediaryStr.str();
}
