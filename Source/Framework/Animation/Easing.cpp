#include "Easing.h"
#include <math.h>


namespace GameDev2D
{
    float Linear::Interpolation(float aValue)
    {
        return aValue;
    }

    float Quadratic::In(float aValue)
    {
        return aValue * aValue;
    }

    float Quadratic::Out(float aValue)
    {
        return aValue * (2.0f - aValue);
    }

    float Quadratic::InOut(float aValue)
    {
        if (aValue < 0.5)
        {
            return 2 * aValue * aValue;
        }
        else
        {
            return (-2 * aValue * aValue) + (4 * aValue) - 1;
        }
    }

    float Cubic::In(float aValue)
    {
        return aValue * aValue * aValue;
    }

    float Cubic::Out(float aValue)
    {
        float f = (aValue - 1.0f);
        return f * f * f + 1.0f;
    }

    float Cubic::InOut(float aValue)
    {
        if (aValue < 0.5f)
        {
            return 4.0f * aValue * aValue * aValue;
        }
        else
        {
            float f = ((2.0f * aValue) - 2.0f);
            return 0.5f * f * f * f + 1.0f;
        }
    }

    float Quartic::In(float aValue)
    {
        return aValue * aValue * aValue * aValue;
    }

    float Quartic::Out(float aValue)
    {
        float f = (aValue - 1.0f);
        return f * f * f * (1.0f - aValue) + 1.0f;
    }

    float Quartic::InOut(float aValue)
    {
        if (aValue < 0.5f)
        {
            return 8.0f * aValue * aValue * aValue * aValue;
        }
        else
        {
            float f = (aValue - 1.0f);
            return -8.0f * f * f * f * f + 1.0f;
        }
    }


    float Quintic::In(float aValue)
    {
        return aValue * aValue * aValue * aValue * aValue;
    }

    float Quintic::Out(float aValue)
    {
        float f = (aValue - 1.0f);
        return f * f * f * f * f + 1.0f;
    }

    float Quintic::InOut(float aValue)
    {
        if (aValue < 0.5f)
        {
            return 16.0f * aValue * aValue * aValue * aValue * aValue;
        }
        else
        {
            float f = ((2.0f * aValue) - 2.0f);
            return  0.5f * f * f * f * f * f + 1.0f;
        }
    }

    float Sinusoidal::In(float aValue)
    {
        return sinf((aValue - 1) * (float)M_PI_2) + 1;
    }

    float Sinusoidal::Out(float aValue)
    {
        return sinf(aValue * (float)M_PI_2);
    }

    float Sinusoidal::InOut(float aValue)
    {
        return 0.5f * (1.0f - cosf(aValue *(float)M_PI));
    }

    float Exponential::In(float aValue)
    {
        return (aValue == 0.0f) ? aValue : powf(2, 10 * (aValue - 1));
    }

    float Exponential::Out(float aValue)
    {
        return (aValue == 1.0f) ? aValue : 1.0f - powf(2, -10 * aValue);
    }

    float Exponential::InOut(float aValue)
    {
        if (aValue == 0.0f || aValue == 1.0f) return aValue;

        if (aValue < 0.5f)
        {
            return 0.5f * powf(2, (20 * aValue) - 10);
        }
        else
        {
            return -0.5f * powf(2, (-20 * aValue) + 10) + 1;
        }
    }

    float Circular::In(float aValue)
    {
        return 1.0f - sqrtf(1 - (aValue * aValue));
    }

    float Circular::Out(float aValue)
    {
        return sqrtf((2.0f - aValue) * aValue);
    }

    float Circular::InOut(float aValue)
    {
        if (aValue < 0.5f)
        {
            return 0.5f * (1.0f - sqrtf(1.0f - 4.0f * (aValue * aValue)));
        }
        else
        {
            return 0.5f * (sqrtf(-((2.0f * aValue) - 3.0f) * ((2.0f * aValue) - 1.0f)) + 1.0f);
        }
    }

    float Elastic::In(float aValue)
    {
        return sinf(13.0f * (float)M_PI_2 * aValue) * powf(2, 10 * (aValue - 1.0f));
    }

    float Elastic::Out(float aValue)
    {
        return sinf(-13.0f * (float)M_PI_2 * (aValue + 1.0f)) * powf(2, -10 * aValue) + 1.0f;
    }

    float Elastic::InOut(float aValue)
    {
        if (aValue < 0.5f)
        {
            return 0.5f * sinf(13.0f * (float)M_PI_2 * (2.0f * aValue)) * powf(2, 10 * ((2.0f * aValue) - 1.0f));
        }
        else
        {
            return 0.5f * (sinf(-13.0f * (float)M_PI_2 * ((2.0f * aValue - 1.0f) + 1.0f)) * powf(2, -10 * (2.0f * aValue - 1.0f)) + 2.0f);
        }
    }

    float Back::In(float aValue)
    {
        return aValue * aValue * aValue - aValue * sinf(aValue * (float)M_PI);
    }

    float Back::Out(float aValue)
    {
        float f = (1.0f - aValue);
        return 1.0f - (f * f * f - f * sinf(f * (float)M_PI));
    }

    float Back::InOut(float aValue)
    {
        if (aValue < 0.5f)
        {
            float f = 2.0f * aValue;
            return 0.5f * (f * f * f - f * sinf(f * (float)M_PI));
        }
        else
        {
            float f = (1.0f - (2.0f * aValue - 1.0f));
            return 0.5f * (1.0f - (f * f * f - f * sinf(f * (float)M_PI))) + 0.5f;
        }
    }

    float Bounce::In(float aValue)
    {
        return 1.0f - Bounce::Out(1.0f - aValue);
    }

    float Bounce::Out(float aValue)
    {
        if (aValue < 4.0f / 11.0f)
        {
            return (121.0f * aValue * aValue) / 16.0f;
        }
        else if (aValue < 8.0f / 11.0f)
        {
            return (363.0f / 40.0f * aValue * aValue) - (99.0f / 10.0f * aValue) + 17.0f / 5.0f;
        }
        else if (aValue < 9.0f / 10.0f)
        {
            return (4356.0f / 361.0f * aValue * aValue) - (35442.0f / 1805.0f * aValue) + 16061.0f / 1805.0f;
        }
        else
        {
            return (54.0f / 5.0f * aValue * aValue) - (513.0f / 25.0f * aValue) + 268.0f / 25.0f;
        }
    }

    float Bounce::InOut(float aValue)
    {
        if (aValue < 0.5f)
        {
            return 0.5f * Bounce::In(aValue * 2.0f);
        }
        else
        {
            return 0.5f * Bounce::Out(aValue * 2.0f - 1.0f) + 0.5f;
        }
    }
}