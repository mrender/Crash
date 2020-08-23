#include "Math.h"
#include <math.h>
#include <stdlib.h>


namespace GameDev2D
{
#ifdef RANDOM_SEED
    Random Math::s_Random(RANDOM_SEED);
#else
    Random Math::s_Random = Random();
#endif

    float Math::DegreesToRadians(float aDegrees)
    {
        return aDegrees * 0.01745329251f;
    }

    float Math::RadiansToDegrees(float aRadians)
    {
        return aRadians * 57.2957795131f;
    }

    float Math::Clamp(float aValue, float aMin, float aMax)
    {
        if (aValue < aMin)
        {
            return aMin;
        }

        if (aValue > aMax)
        {
            return aMax;
        }

        return aValue;
    }

    Vector2 Math::Clamp(const Vector2& aValue, const Vector2& aMin, const Vector2& aMax)
    {
        if (aValue < aMin)
        {
            return aMin;
        }

        if (aValue > aMax)
        {
            return aMax;
        }

        return aValue;
    }

    int Math::RandomInt(int aMin, int aMax)
    {
        return s_Random.RandomInt(aMin, aMax);
    }

    float Math::RandomFloat()
    {
        return RandomFloat(0.0f, 1.0f);
    }

    float Math::RandomFloat(float aMin, float aMax)
    {
        return s_Random.RandomFloat(aMin, aMax);
    }

    double Math::RandomDouble()
    {
        return RandomDouble(0.0, 1.0);
    }

    double Math::RandomDouble(double aMin, double aMax)
    {
        return s_Random.RandomDouble(aMin, aMax);
    }

    bool Math::IsClose(float aValue, float aTarget, float aRange)
    {
        if (aValue > aTarget - aRange && aValue < aTarget + aRange)
        {
            return true;
        }

        return false;
    }
}