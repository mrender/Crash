#pragma once

#include "Vector2.h"
#include "Rotation.h"
#include "Random.h"


namespace GameDev2D
{
    struct Math
    {
        static float DegreesToRadians(float degrees);
        static float RadiansToDegrees(float radians);

        static float Clamp(float value, float min, float max);
        static Vector2 Clamp(const Vector2& value, const Vector2& min, const Vector2& max);
        
        static int RandomInt(int min, int max);   

        static float RandomFloat();                         //Returns a float value between 0.0 and 1.0
        static float RandomFloat(float min, float max);     //Returns a float value between a min value and a max value
    
        static double RandomDouble();                         //Returns a double value between 0.0 and 1.0
        static double RandomDouble(double min, double max);   //Returns a double value between a min value and a max value

        static bool IsClose(float value, float target, float range);

    private:
        static Random s_Random;
    };
}