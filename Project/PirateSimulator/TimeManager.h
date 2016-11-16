#ifndef TIME_MANAGER_HEADER
#define TIME_MANAGER_HEADER
#include "../PetitMoteur3D/PetitMoteur3D/Singleton.h"
#include <chrono>
#include <map>

using namespace std::chrono;

class TimeManager : public PM3D::CSingleton<TimeManager>
{
    time_point<system_clock> m_fastSave = system_clock::now();
    time_point<system_clock> m_startExecutionTime = system_clock::now();
    time_point<system_clock> m_startGameTime = system_clock::now();
    milliseconds m_turnTimeLimit = 0ms;
    std::map<std::string, time_point<system_clock>> m_saveTimePoints;
public:
    static time_point<system_clock> now()
    {
        return system_clock::now();
    }

    void startGameTime()
    {
        m_startGameTime = system_clock::now();
    }
    milliseconds getTimeFromStart() const
    {
        return duration_cast<milliseconds>(system_clock::now() - m_startGameTime);
    }
    // Return the time point of the saved name. If no name, return now()
    time_point<system_clock> getPoint(std::string name);
    time_point<system_clock> savePoint(std::string name);

    // Time between two saved time point (first - second)
    milliseconds getTimeBetweenTwoPoints(std::string name1, std::string name2)
    {
        return duration_cast<milliseconds>(m_saveTimePoints[name1] - m_saveTimePoints[name2]);
    }
    // Time between two time point (first - second)
    milliseconds getTimeBetweenTwoPoints(time_point<system_clock> tp1, time_point<system_clock> tp2) const
    {
        return duration_cast<milliseconds>(tp1 - tp2);
    }
    void fastSave()
    {
        m_fastSave = system_clock::now();
    }
    milliseconds getFastDifference() const
    {
        return duration_cast<milliseconds>(m_fastSave - system_clock::now());
    }

    void setTurnLimitTime(milliseconds ms);
    void setTurnLimitTime(seconds s);

    // Get the remaining time between the fast save and now
    milliseconds getRemainingFastTime() const;
};

#endif //TIME_MANAGER_HEADER