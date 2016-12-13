#ifndef GAMEUTILS_H_INCLUDED
#define GAMEUTILS_H_INCLUDED
#include <string>
#include <chrono>

namespace PirateSimulator
{
    struct MathUtils
    {
        //clamp value between min and max included
        template<class Type>
        static constexpr Type static_clamp(Type value, Type min, Type max)
        {
            return (value < min) ? min : (value > max) ? max : value;
        }
    };

    struct TimeUtils
    {
        static std::wstring timeToString(std::chrono::milliseconds time, bool displayMs = false)
        {
            unsigned int ms, s, m, h = 0;
            std::wstring msW, sW, mW, hW;
            s = static_cast<unsigned int>(time.count() / 1000);
            m = static_cast<unsigned int>(s / 60);
            if(m > 0)
            {
                s = s % (m * 60);
            }
            h = static_cast<unsigned int>(m / 60);
            if(h > 0)
            {
                m = m % (h * 60);
            }
            sW = std::to_wstring(s);
            padTo(sW, 2);
            mW = std::to_wstring(m);
            padTo(mW, 2);
            hW = std::to_wstring(h);
            padTo(hW, 2);

            if(displayMs)
            {
                ms = static_cast<unsigned int>(time.count() - (s * 1000));
                msW = std::to_wstring(ms);

                return hW + L":" + mW + L":" + sW + L"." + msW;
            }

            return hW + L":" + mW + L":" + sW;
        }

        static void padTo(std::wstring &str, const size_t num, const char paddingChar = '0')
        {
            if(num > str.size())
                str.insert(str.begin(), num - str.size(), paddingChar);
        }
    };
}

#endif //!GAMEUTILS_H_INCLUDED