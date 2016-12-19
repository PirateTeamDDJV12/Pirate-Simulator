#ifndef RANDOMISATOR_H_INCLUDED
#define RANDOMISATOR_H_INCLUDED

#include <random>

namespace PirateSimulator
{
    class Randomisator
    {
    public:
        enum
        {
            MAX_VALUE = 1000000000,
            MIN_VALUE = 0
        };


    private:
        class FloatingVal{};
        class IntegerVal{};


    private:
        static std::random_device m_randomDeviceSeed;


    private:
        std::mt19937 m_random;


    public:
        Randomisator();


    public:
        //get a random value between min and max. The type of min and max determines the return type
        template<class Type>
        Type get(Type min, Type max)
        {
            static_assert(std::is_floating_point<Type>::value || std::is_integral<Type>::value, "Not a number");

            return get(min, max, std::conditional_t<std::is_floating_point<Type>::value, FloatingVal, IntegerVal>{});
        }


    private:
        template<class Type>
        Type get(Type min, Type max, FloatingVal)
        {
            return std::uniform_real_distribution<Type>(min, max)(m_random);
        }

        template<class Type>
        Type get(Type min, Type max, IntegerVal)
        {
            return std::uniform_int_distribution<Type>(min, max)(m_random);
        }
    };
}

#endif //!RANDOMISATOR_H_INCLUDED
