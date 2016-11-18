#ifndef BINARYFAST_H
#define BINARYFAST_H

#include <type_traits>

#include <stdint.h>

namespace PirateSimulator
{
    class BinaryFast
    {
    public:
        struct Load{};
        struct Save{};


    public:
        struct Binary32
        {
            uint8_t m_firstByte;
            uint8_t m_secondByte;
            uint8_t m_thirdByte;
            uint8_t m_fourthByte;

            Binary32(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4) :
                m_firstByte { byte1 },
                m_secondByte{ byte2 },
                m_thirdByte { byte3 },
                m_fourthByte{ byte4 }
            {}

            Binary32(uint32_t value) :
                m_firstByte { value >> 24 },
                m_secondByte{ (value >> 16) & 0xFF },
                m_thirdByte { (value >> 8) & 0xFF } ,
                m_fourthByte{ value & 0xFF }
            {}
        };


    private:
        union
        {
            uint32_t m_intValue;
            float m_floatValue;

            Binary32 m_byte32;
        };


    public:
        BinaryFast() :
            m_intValue{}
        {}

        BinaryFast(uint32_t value) :
            m_intValue{value}
        {}

        BinaryFast(float value) :
            m_floatValue{ value }
        {}

        BinaryFast(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4) :
            m_byte32{ byte1, byte2, byte3, byte4 }
        {}

        BinaryFast(const Binary32& binaryStruct) :
            m_byte32{ binaryStruct }
        {}


    public:
        uint32_t getUint32() const noexcept
        {
            return m_intValue;
        }

        float getFloat() const noexcept
        {
            return m_floatValue;
        }

        Binary32 getBinary() const noexcept
        {
            return m_byte32;
        }

        void setUint32(uint32_t newValue) noexcept
        {
            m_intValue = newValue;
        }

        void setFloat(float newValue) noexcept
        {
            m_floatValue = newValue;
        }

        void setBinary(const Binary32& newValue) noexcept
        {
            m_byte32 = newValue;
        }

        void setBinary(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4) noexcept;

        /*
        Template method that load or set a value from or into a buffer at the specified offset
        use as : 
          put<BinaryFast::Save>(bufferToSave, positionInBufferToSave);
          put<BinaryFast::Load>(bufferToLoad, positionInBufferToLoadFrom);
        */
        template<class Mode>
        void put(char* output, size_t offset) noexcept
        {
            this->put(output, offset, std::conditional_t<std::is_same<Mode, Save>::value,
                      Save,
                      std::conditional_t<std::is_same<Mode, Load>::value,
                          Load,
                          Mode>>
            {}
            );
        }


    public:
        bool operator==(const Binary32& other) const noexcept;

        bool operator==(uint32_t other) const noexcept
        {
            return m_intValue == other;
        }

        bool operator==(float other) const noexcept
        {
            return m_floatValue == other;
        }

        bool operator!=(const Binary32& other) const noexcept
        {
            return !(*this == other);
        }

        bool operator!=(uint32_t other) const noexcept
        {
            return !(*this == other);
        }

        bool operator!=(float other) const noexcept
        {
            return !(*this == other);
        }

        void operator()(uint32_t valueToSave, char* buffer, size_t& offset) noexcept;
        void operator()(float valueToSave, char* buffer, size_t& offset) noexcept;


    private:
        void put(char* output, size_t offset, Save) const noexcept;

        void put(char* output, size_t offset, Load) noexcept
        {
            this->setBinary(*(output + offset), *(output + offset + 1), *(output + offset + 2), *(output + offset + 3));
        }
    };
}

#endif //!BINARYFAST_H
