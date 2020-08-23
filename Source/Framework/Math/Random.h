#pragma once

#include <random>

namespace GameDev2D
{
    class Random
    {
    public:
        Random();
        Random(int seed);

        int RandomInt(int min, int max);             //Returns an int value between a min value and a max value

        float RandomFloat();                         //Returns a float value between 0.0 and 1.0
        float RandomFloat(float min, float max);     //Returns a float value between a min value and a max value

        double RandomDouble();                        //Returns a double value between 0.0 and 1.0
        double RandomDouble(double min, double max);  //Returns a double value between a min value and a max value

    private:
        std::mt19937 m_MersenneTwister;
    };
}