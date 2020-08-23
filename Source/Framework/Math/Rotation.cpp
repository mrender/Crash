#include "Rotation.h"
#include "Math.h"
#include <math.h>


namespace GameDev2D
{
    Rotation::Rotation()
    {
        SetRadians(0.0f);
    }

	Rotation Rotation::Degrees(float aDegrees)
	{
		Rotation rotation;
		rotation.SetDegrees(aDegrees);
		return rotation;
	}

	Rotation Rotation::Radians(float aRadians)
	{
		Rotation rotation;
		rotation.SetRadians(aRadians);
		return rotation;
	}

	Rotation Rotation::Direction(const Vector2& aDirection)
	{
		Rotation rotation;
		rotation.SetDirection(aDirection);
		return rotation;
	}

	void Rotation::SetDegrees(float aDegrees)
	{
		SetRadians(Math::DegreesToRadians(aDegrees));
	}

	void Rotation::SetRadians(float aRadians)
	{
		//Set the angle in degrees
		m_Radians = aRadians;

		float twoPi = static_cast<float>(M_PI) * 2.0f;

		//Bounds check the angle
		if (m_Radians < 0.0f)
		{
			m_Radians += twoPi;
		}
		else if (m_Radians > twoPi)
		{
			m_Radians -= twoPi;
		}
	}

	float Rotation::GetDegrees()
	{
		return Math::RadiansToDegrees(m_Radians);
	}

	float Rotation::GetRadians()
	{
		return m_Radians;
	}

	void Rotation::RotateDegrees(float aIncrementInDegrees)
	{
		SetRadians(GetRadians() + Math::DegreesToRadians(aIncrementInDegrees));
	}

	void Rotation::RotateRadians(float aIncrementInRadians)
	{
		SetRadians(GetRadians() + aIncrementInRadians);
	}

    void Rotation::SetDirection(const Vector2& aDirection)
    {
        //Calculate the angle in radians
		m_Radians = atan2f(aDirection.y, aDirection.x);
    }

    Vector2 Rotation::GetDirection()
    {
		//Calculate the direction vector
		Vector2 direction = Vector2(cosf(m_Radians), sinf(m_Radians));
        return direction;
    }
}