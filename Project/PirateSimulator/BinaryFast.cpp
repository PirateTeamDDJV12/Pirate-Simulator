#include "BinaryFast.h"


using namespace PirateSimulator;


void BinaryFast::setBinary(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4) noexcept
{
    m_byte32.m_firstByte = byte1;
    m_byte32.m_secondByte = byte2;
    m_byte32.m_thirdByte = byte3;
    m_byte32.m_fourthByte = byte4;
}

bool BinaryFast::operator==(const Binary32& other) const noexcept
{
    return m_byte32.m_firstByte == other.m_firstByte &&
        m_byte32.m_secondByte == other.m_secondByte &&
        m_byte32.m_thirdByte == other.m_thirdByte &&
        m_byte32.m_fourthByte == other.m_fourthByte;
}

void BinaryFast::operator()(uint32_t valueToSave, char* buffer, size_t& offset) noexcept
{
    *this = valueToSave;

    this->put<Save>(buffer, offset);
    offset += 4;
}

void BinaryFast::operator()(float valueToSave, char* buffer, size_t& offset) noexcept
{
    *this = valueToSave;

    this->put<Save>(buffer, offset);
    offset += 4;
}

void BinaryFast::put(char* output, size_t offset, Save) const noexcept
{
    *(output + offset) = m_byte32.m_firstByte;
    *(output + offset + 1) = m_byte32.m_secondByte;
    *(output + offset + 2) = m_byte32.m_thirdByte;
    *(output + offset + 3) = m_byte32.m_fourthByte;
}
