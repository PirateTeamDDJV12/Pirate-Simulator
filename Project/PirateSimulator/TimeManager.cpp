#include "TimeManager.h"

time_point<system_clock> TimeManager::savePoint(std::string name)
{
    m_saveTimePoints[name] = system_clock::now();
    return m_saveTimePoints[name];
}

void TimeManager::setTurnLimitTime(milliseconds ms)
{
    if(ms <= 0ms)
    {
        return;
    }
    m_turnTimeLimit = ms;
}

void TimeManager::setTurnLimitTime(seconds s)
{
    if(s <= 0ms)
    {
        return;
    }
    m_turnTimeLimit = s;
}

milliseconds TimeManager::getRemainingFastTime() const
{
    milliseconds currentDuration = duration_cast<milliseconds>(system_clock::now() - m_fastSave);
    milliseconds remaining = m_turnTimeLimit - currentDuration;
    return remaining <= 0ms ? 0ms : remaining;
}

void TimeManager::update()
{
    m_timeCurrent = msNow();

    if(m_timeCurrent > m_timeNextFrame)
    {
        m_timePreviousFrame = m_timeCurrent;
        m_timeNextFrame = m_timeCurrent + m_ecartTemps;
    }
}

time_point<system_clock> TimeManager::getPoint(std::string name)
{
    if(m_saveTimePoints.find(name) == end(m_saveTimePoints))
    {
        return system_clock::now();
    }
    return m_saveTimePoints[name];
}
