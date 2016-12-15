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

milliseconds TimeManager::getRemainingStartTime() const
{
    milliseconds remaining = m_turnTimeLimit - getTimeFromStart();
    return remaining <= 0ms ? 0ms : remaining;
}

void TimeManager::update()
{
    auto tempCurrent = msNow();

    if(tempCurrent > m_timeNextFrame)
    {
        m_timePreviousFrame = m_timeCurrent;
        m_timeCurrent = tempCurrent;
        m_timeNextFrame = tempCurrent + m_ecartTemps;
    }
}

void TimeManager::endPause()
{
    milliseconds elapsedTime = getTimeBetweenTwoPoints(system_clock::now(), getPoint("beginPause"));
    m_startGameTime += elapsedTime;
}

time_point<system_clock> TimeManager::getPoint(std::string name)
{
    if(m_saveTimePoints.find(name) == end(m_saveTimePoints))
    {
        return system_clock::now();
    }
    return m_saveTimePoints[name];
}
