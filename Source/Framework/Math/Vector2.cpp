#include "Vector2.h"
#include <math.h>


namespace GameDev2D
{
    //Initialize the static ZeroVector and UnitVector variables
    const Vector2 Vector2::Zero = Vector2(0.0f, 0.0f);
    const Vector2 Vector2::Unit = Vector2(1.0f, 1.0f);

	const Vector2 Vector2::Left = Vector2(-1.0f, 0.0f);
	const Vector2 Vector2::Right = Vector2(1.0f, 0.0f);
	const Vector2 Vector2::Up = Vector2(0.0f, 1.0f);
	const Vector2 Vector2::Down = Vector2(0.0f, -1.0f);

    Vector2::Vector2(float aX, float aY) :
        x(aX),
        y(aY)
    {

    }

    Vector2::Vector2(const Vector2& aVector2) :
        x(aVector2.x),
        y(aVector2.y)
    {

    }

    Vector2::Vector2() :
        x(0.0f),
        y(0.0f)
    {

    }

	Rotation Vector2::Angle()
    {
		float radians = fmodf((atan2f(y, x) + static_cast<float>(M_PI) * 4.0f), static_cast<float>(M_PI) * 2.0f);
		return Rotation::Radians(radians);
    }

    float Vector2::Length()
    {
        return sqrtf(LengthSquared());
    }

    float Vector2::LengthSquared()
    {
        return x * x + y * y;
    }

    void Vector2::Normalize()
    {
        float length = LengthSquared();
        if (length != 0)
        {
            length = sqrtf(length);
            x /= length;
            y /= length;
        }
    }

    Vector2 Vector2::Normalized()
    {
        Vector2 vector2 = Vector2(x, y);
        vector2.Normalize();
        return vector2;
    }

    float Vector2::Distance(const Vector2& aVector)
    {
        return sqrtf(DistanceSquared(aVector));
    }

    float Vector2::DistanceSquared(const Vector2& aVector)
    {
        return (x - aVector.x) * (x - aVector.x) + (y - aVector.y) * (y - aVector.y);
    }

    float Vector2::DotProduct(const Vector2& aVector2)
    {
        return x * aVector2.x + y * aVector2.y;
    }

	Vector2 Vector2::PerpendicularClockwise()
	{
		return Vector2(y, -x);
	}

	Vector2 Vector2::PerpendicularCounterClockwise()
	{
		return Vector2(-y, x);
	}

    Vector2 Vector2::operator+(const Vector2& aVector2) const
    {
        return Vector2(x + aVector2.x, y + aVector2.y);
    }

    void Vector2::operator+=(const Vector2& aVector2)
    {
        x += aVector2.x;
        y += aVector2.y;
    }

    Vector2 Vector2::operator-(const Vector2& aVector2) const
    {
        return Vector2(x - aVector2.x, y - aVector2.y);
    }

    void Vector2::operator-=(const Vector2& aVector2)
    {
        x -= aVector2.x;
        y -= aVector2.y;
    }

    Vector2 Vector2::operator*(const Vector2& aVector2) const
    {
        return Vector2(x * aVector2.x, y * aVector2.y);
    }

    Vector2 Vector2::operator*(const float& aScale) const
    {
        return Vector2(x * aScale, y * aScale);
    }

    void Vector2::operator*=(const Vector2& aVector2)
    {
        x *= aVector2.x;
        y *= aVector2.y;
    }

    void Vector2::operator*=(const float& aScale)
    {
        x *= aScale;
        y *= aScale;
    }

    Vector2 Vector2::operator/(const Vector2& aVector2) const
    {
        return Vector2(x / aVector2.x, y / aVector2.y);
    }

    Vector2 Vector2::operator/(const float& aScale) const
    {
        return Vector2(x / aScale, y / aScale);
    }

    void Vector2::operator/=(const Vector2& aVector2)
    {
        x /= aVector2.x;
        y /= aVector2.y;
    }

    void Vector2::operator/=(const float& aScale)
    {
        x /= aScale;
        y /= aScale;
    }

    Vector2 Vector2::operator-() const
    {
        return Vector2(-x, -y);
    }

    bool Vector2::operator==(const Vector2& aVector2) const
    {
        return x == aVector2.x && y == aVector2.y;
    }

    bool Vector2::operator != (const Vector2& aVector2) const
    {
        return x != aVector2.x || y != aVector2.y;
    }

    bool Vector2::operator<(const Vector2& aVector2) const
    {
        return (x == aVector2.x) ? (y < aVector2.y) : (x < aVector2.x);
    }

    bool Vector2::operator<=(const Vector2& aVector2) const
    {
        return (x == aVector2.x) ? (y <= aVector2.y) : (x <= aVector2.x);
    }

    bool Vector2::operator>(const Vector2& aVector2) const
    {
        return (x == aVector2.x) ? (y > aVector2.y) : (x > aVector2.x);
    }

    bool Vector2::operator>=(const Vector2& aVector2) const
    {
        return (x == aVector2.x) ? (y >= aVector2.y) : (x >= aVector2.x);
    }

	Vector2 operator* (float scale, const Vector2& vector2)
	{
		return Vector2(vector2.x * scale, vector2.y * scale);
	}
}