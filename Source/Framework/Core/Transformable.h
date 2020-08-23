#pragma once

#include "../Math/Rotation.h"
#include "../Math/Vector2.h"
#include "../Math/Matrix.h"


namespace GameDev2D
{
    class Transformable
    {
    public:
        Transformable();
		Transformable(const Transformable& transformable);
		virtual ~Transformable();

        //Returns the Transformation matrix
        Matrix GetTransformMatrix();

        //Returns the Position of the Transformable object
        Vector2 GetPosition();

        //Sets the Position of the Transformable object
        void SetPosition(Vector2 position);

		//Sets the Position of the Transformable object
		void SetPosition(float x, float y);

		//Sets the Position along the x-axis of the Transformable object
		void SetPositionX(float x);

		//Sets the Position along the y-axis of the Transformable object
		void SetPositionY(float y);

        //Translates the Transformable object from its current position
        void Translate(Vector2 offset);

        //Returns the Angle of the Transformable object in Degrees
        float GetDegrees();

		//Returns the Angle of the Transformable object in Radians
		float GetRadians();

        //Sets the Angle of the Transformable object in Degrees
        void SetDegrees(float degrees);

		//Sets the Angle of the Transformable object in Degrees
		void SetRadians(float radians);

        //Rotates the Transformable object from its current angle
        void Rotate(Rotation& rotation);

        //Returns the direction vector of the Transformable object
        Vector2 GetDirection();

        //Sets the direction vector of the Transformable object
        void SetDirection(Vector2 direction);

        //Returns the scale of the Transformable object
        Vector2 GetScale();

        //Sets the scale of the Transformable object
        void SetScale(Vector2 scale);

		//Sets the scale of the Transformable object
		void SetScale(float scaleX, float scaleY);

		//Sets the scale along the x-axis of the Transformable object
		void SetScaleX(float scaleX);

		//Sets the scale along the y-axis of the Transformable object
		void SetScaleY(float scaleY);

        //Logs the position, rotation and scale data to the output window
        void Log();

    protected:
		virtual void TransformMatrixIsDirty();

        //Member variables
        Matrix m_Transform;
        Rotation m_Rotation;
        Vector2 m_Position;
        Vector2 m_Scale;
        bool m_IsTransformMatrixDirty;
    };
}
