#include "stdafx.h"

#include "Vertex.h"


using namespace PirateSimulator;


void Vertex::writeVectIntoStdString(std::stringstream& container, const Vect3f& valueToWrite) const noexcept
{
    container << valueToWrite.x() << SPACE_SEPARATOR;
    container << valueToWrite.y() << SPACE_SEPARATOR;
    container << valueToWrite.z();
}

std::string Vertex::toString() const noexcept
{
    std::stringstream intermediaryStr;

    writeVectIntoStdString(intermediaryStr, position());
    intermediaryStr << SPACE_SEPARATOR;

    writeVectIntoStdString(intermediaryStr, normalVector());
    intermediaryStr << LINE_JUMP_SEPARATOR;

    return intermediaryStr.str();
}
