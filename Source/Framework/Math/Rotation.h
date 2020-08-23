#ifndef __GameDev2D_Rotation_h__
#define __GameDev2D_Rotation_h__

#include "Vector2.h"


namespace GameDev2D
{
    class Rotation
    {
    public:
        Rotation();

		static Rotation Degrees(float degrees);
		static Rotation Radians(float radians);
		static Rotation Direction(const Vector2& direction);

		void SetDegrees(float degrees);
		void SetRadians(float radians);

		float GetDegrees();
		float GetRadians();

		void RotateDegrees(float incrementInDegrees);
		void RotateRadians(float incrementInRadians);

        void SetDirection(const Vector2& direction);
        Vector2 GetDirection();

    private:
        float m_Radians;
    };
}

#endif
