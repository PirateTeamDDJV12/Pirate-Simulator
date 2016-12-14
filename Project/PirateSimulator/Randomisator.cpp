#include "Randomisator.h"

#include <chrono>


using namespace PirateSimulator;


std::random_device Randomisator::m_randomDeviceSeed;


Randomisator::Randomisator() :
    m_random{ m_randomDeviceSeed() }
{}
    