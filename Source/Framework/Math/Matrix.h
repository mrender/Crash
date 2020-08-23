#ifndef __GameDev2D_Matrix_h__
#define __GameDev2D_Matrix_h__

#include "Rotation.h"
#include "Vector2.h"


namespace GameDev2D
{
    //Local constants
    const unsigned int MATRIX_NUM_ROWS = 4;
    const unsigned int MATRIX_NUM_COLUMNS = 4;

    struct Matrix
    {
        //Returns an Identity Matrix
        static Matrix Identity();

        //Returns a Matrix based on the translation
        static Matrix Make(Vector2 translation, float radians, Vector2 scale = Vector2(1.0f, 1.0f));

        //Returns a Matrix based on the translation
        static Matrix MakeTranslation(Vector2 translation);
        static Matrix MakeTranslation(float x, float y);

        //Returns a Matrix based on the rotation
        static Matrix MakeRotation(Rotation rotation);
        static Matrix MakeRotation(float radians);

        //Returns a Matrix based on the scale
        static Matrix MakeScale(Vector2 scale);
        static Matrix MakeScale(float sx, float sy);

        //Returns an orthographic projection Matrix
        static Matrix Orthographic(float left, float right, float bottom, float top, float zNear, float zFar);

        //Creates an empty Matrix, all elements initialized to zero
        Matrix();

        //Copy a Matrix
        Matrix(const Matrix& matrix);

        //Sets the Matrix to an identity Matrix
        void SetIdentity();

        //Translates the Matrix
        void SetTranslation(Vector2 translation);
        void SetTranslation(float x, float y);

        //Rotates the Matrix
        void SetRotation(Rotation rotation);
        void SetRotation(float radians);

        //Scales the Matrix
        void SetScale(Vector2 scale);
        void SetScale(float sx, float sy);

        //Returns the translation Vector2 value
        Vector2 GetTranslation();

        //Returns the angle (euler) value
        Rotation GetRotation();
        float GetRadians(); 
		float GetDegrees(); 

        //Returns the scale Vector2 value
        Vector2 GetScale();

        //Returns a transposed Matrix based on this Matrix
        Matrix GetTransposed();

        //Returns an inversed Matrix based on this Matrix
        Matrix GetInverse();

        //Multiplication operator override
        Vector2 operator *(const Vector2 vector2) const;

        //Multiplication operator override
        Matrix operator *(const Matrix matrix) const;

        //Member variable
        float m[MATRIX_NUM_COLUMNS][MATRIX_NUM_ROWS];
    };
}

#endif