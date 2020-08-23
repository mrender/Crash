#pragma once


namespace GameDev2D
{
	class Rotation;

    struct Vector2
    {
        Vector2(float x, float y);
        Vector2(const Vector2& vector2);
        Vector2();

        //2D zero vector constant (0,0)
        static const Vector2 Zero;

        //2D unit vector constant (1,1)
        static const Vector2 Unit;

		//2D unit vector constant (-1,0)
		static const Vector2 Left;

		//2D unit vector constant (1,0)
		static const Vector2 Right;

		//2D unit vector constant (0,1)
		static const Vector2 Up;

		//2D unit vector constant (0,-1)
		static const Vector2 Down;

        //Calculates the angle of the vector
        Rotation Angle();

        //Returns the Length (magnitude) of the vector
        float Length();

        //Returns the Length squared of the vector
        float LengthSquared();

        //Normalizes the current vector
        void Normalize();

        //Returns a normalized copy of the current vector
        Vector2 Normalized();

        //Calculates the distance between the current vector and the supplied vector
        float Distance(const Vector2& vector2);

        //Calculates the distance squared between the current vector and the supplied vector
        float DistanceSquared(const Vector2& vector2);

        //Calculates the dot product between the current vector and the supplied vector
        float DotProduct(const Vector2& vector2);

		//Returns a vector rotated 90 degrees clockwise
		Vector2 PerpendicularClockwise();

		//Returns a vector rotated 90 degrees counterclockwise
		Vector2 PerpendicularCounterClockwise();

        Vector2 operator+(const Vector2& vector2) const;
        void operator+=(const Vector2& vector2);

        Vector2 operator-(const Vector2& vector2) const;
        void operator-=(const Vector2& vector2);

        Vector2 operator*(const Vector2& vector2) const;
        void operator*=(const Vector2& vector2);

        Vector2 operator*(const float& scale) const;
        void operator*=(const float& scale);

        Vector2 operator/(const Vector2& vector2) const;
        void operator/=(const Vector2& vector2);

        Vector2 operator/(const float& scale) const;
        void operator/=(const float& scale);

        Vector2 operator-() const;

        bool operator==(const Vector2& vector2) const;
        bool operator!=(const Vector2& vector2) const;

        bool operator<(const Vector2& vector2) const;
        bool operator<=(const Vector2& vector2) const;

        bool operator>(const Vector2& vector2) const;
        bool operator>=(const Vector2& vector2) const;

        //Member variables
        float x;
        float y;
    };


	Vector2 operator* (float scale, const Vector2& vector2);
}
