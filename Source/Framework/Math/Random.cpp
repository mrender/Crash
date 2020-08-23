#include "Random.h"


namespace GameDev2D
{
    Random::Random() :
        m_MersenneTwister(std::random_device{}())
    {

    }

    Random::Random(int aSeed) :
        m_MersenneTwister(aSeed)
    {

    }

    int Random::RandomInt(int aMin, int aMax)
    {
        std::uniform_int_distribution<int> distribution(aMin, aMax);
        return distribution(m_MersenneTwister);
    }

    float Random::RandomFloat()
    {
        return RandomFloat(0.0f, 1.0f);
    }

    float Random::RandomFloat(float aMin, float aMax)
    {
        std::uniform_real_distribution<float> distribution(aMin, aMax);
        return distribution(m_MersenneTwister);
    }

    double Random::RandomDouble()
    {
        return RandomDouble(0.0, 1.0);
    }

    double Random::RandomDouble(double aMin, double aMax)
    {
        std::uniform_real_distribution<double> distribution(aMin, aMax);
        return distribution(m_MersenneTwister);
    }
}