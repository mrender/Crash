#include "Matrix.h"
#include "Math.h"
#include <math.h>


namespace GameDev2D
{
    Matrix Matrix::Identity()
    {
        Matrix matrix;
        matrix.SetIdentity();
        return matrix;
    }

    Matrix Matrix::Make(Vector2 aTranslation, float aRadians, Vector2 aScale)
    {
        Matrix translation = Matrix::MakeTranslation(aTranslation);
        Matrix rotation = Matrix::MakeRotation(aRadians);
        Matrix scale = Matrix::MakeScale(aScale);
        Matrix transformation = translation * rotation * scale;
        return transformation;
    }

    Matrix Matrix::MakeTranslation(Vector2 aTranslation)
    {
        Matrix matrix = Identity();
        matrix.SetTranslation(aTranslation);
        return matrix;
    }

    Matrix Matrix::MakeTranslation(float aX, float aY)
    {
        return MakeTranslation(Vector2(aX, aY));
    }

    Matrix Matrix::MakeRotation(Rotation aRotation)
    {
        Matrix matrix = Identity();
        matrix.SetRotation(aRotation);
        return matrix;
    }

    Matrix Matrix::MakeRotation(float aRadians)
    {
        return MakeRotation(Rotation::Radians(aRadians));
    }

    Matrix Matrix::MakeScale(Vector2 aScale)
    {
        Matrix matrix = Identity();
        matrix.SetScale(aScale);
        return matrix;
    }

    Matrix Matrix::MakeScale(float aSx, float aSy)
    {
        return MakeScale(Vector2(aSx, aSy));
    }

    Matrix Matrix::Orthographic(float aLeft, float aRight, float aBottom, float aTop, float aZNear, float aZFar)
    {
        Matrix matrix = Identity();
        matrix.m[0][0] = 2.0f / (aRight - aLeft);
        matrix.m[1][1] = 2.0f / (aTop - aBottom);
        matrix.m[2][2] = -2.0f / (aZFar - aZNear);
        matrix.m[3][0] = -(aRight + aLeft) / (aRight - aLeft);
        matrix.m[3][1] = -(aTop + aBottom) / (aTop - aBottom);
        matrix.m[3][2] = -(aZFar + aZNear) / (aZFar - aZNear);
        return matrix;
    }

    Matrix::Matrix()
    {
        m[0][0] = 0;  m[1][0] = 0;  m[2][0] = 0;  m[3][0] = 0;
        m[0][1] = 0;  m[1][1] = 0;  m[2][1] = 0;  m[3][1] = 0;
        m[0][2] = 0;  m[1][2] = 0;  m[2][2] = 0;  m[3][2] = 0;
        m[0][3] = 0;  m[1][3] = 0;  m[2][3] = 0;  m[3][3] = 0;
    }

    Matrix::Matrix(const Matrix& aMatrix)
    {
        for (unsigned int i = 0; i < MATRIX_NUM_ROWS; i++)
        {
            for (unsigned int j = 0; j < MATRIX_NUM_COLUMNS; j++)
            {
                m[i][j] = aMatrix.m[i][j];
            }
        }
    }

    void Matrix::SetIdentity()
    {
        m[0][0] = 1;  m[1][0] = 0;  m[2][0] = 0;  m[3][0] = 0;
        m[0][1] = 0;  m[1][1] = 1;  m[2][1] = 0;  m[3][1] = 0;
        m[0][2] = 0;  m[1][2] = 0;  m[2][2] = 1;  m[3][2] = 0;
        m[0][3] = 0;  m[1][3] = 0;  m[2][3] = 0;  m[3][3] = 1;
    }

    void Matrix::SetTranslation(Vector2 aTranslation)
    {
        m[3][0] = aTranslation.x;
        m[3][1] = aTranslation.y;
    }

    void Matrix::SetTranslation(float aX, float aY)
    {
        SetTranslation(Vector2(aX, aY));
    }

    void Matrix::SetRotation(Rotation aRotation)
    {
        Matrix rotation = Matrix::Identity();
        rotation.m[0][0] = aRotation.GetDirection().x;  rotation.m[1][0] = -aRotation.GetDirection().y;
        rotation.m[0][1] = aRotation.GetDirection().y;  rotation.m[1][1] = aRotation.GetDirection().x;
        *this = rotation * *this;
    }

    void Matrix::SetRotation(float aRadians)
    {
        SetRotation(Rotation::Radians(aRadians));
    }

    void Matrix::SetScale(Vector2 aScale)
    {
        m[0][0] *= aScale.x;  m[1][0] *= aScale.x;  m[3][0] *= aScale.x;
        m[0][1] *= aScale.y;  m[1][1] *= aScale.y;  m[3][1] *= aScale.y;
    }

    void Matrix::SetScale(float aSx, float aSy)
    {
        SetScale(Vector2(aSx, aSy));
    }

    Vector2 Matrix::GetTranslation()
    {
        return Vector2(m[3][0], m[3][1]);
    }

    Rotation Matrix::GetRotation()
    {
        return Rotation::Radians(GetRadians());
    }

	float Matrix::GetRadians()
	{
		float radians = atan2f(m[0][1], m[1][1]);
		return radians;
	}

	float Matrix::GetDegrees()
	{
		return Math::RadiansToDegrees(GetRadians());
	}

    Vector2 Matrix::GetScale()
    {
        Vector2 scale;
        scale.x = Vector2(m[0][0], m[0][1]).Length();
        scale.y = Vector2(m[1][0], m[1][1]).Length();
        return scale;
    }

    Matrix Matrix::GetTransposed()
    {
        Matrix matrix(*this);

        matrix.m[0][1] = m[1][0];
        matrix.m[1][0] = m[0][1];

        matrix.m[0][2] = m[2][0];
        matrix.m[2][0] = m[0][2];

        matrix.m[0][3] = m[3][0];
        matrix.m[3][0] = m[0][3];

        matrix.m[1][2] = m[2][1];
        matrix.m[2][1] = m[1][2];

        matrix.m[1][3] = m[3][1];
        matrix.m[3][1] = m[1][3];

        matrix.m[2][3] = m[3][2];
        matrix.m[3][2] = m[2][3];

        return matrix;
    }

    Matrix Matrix::GetInverse()
    {
        Matrix inverse;

        inverse.m[0][0] = m[1][1] * m[2][2] * m[3][3] -
                          m[1][1] * m[2][3] * m[3][2] -
                          m[2][1] * m[1][2] * m[3][3] +
                          m[2][1] * m[1][3] * m[3][2] +
                          m[3][1] * m[1][2] * m[2][3] -
                          m[3][1] * m[1][3] * m[2][2];

        inverse.m[1][0] = -m[1][0] * m[2][2] * m[3][3] +
                           m[1][0] * m[2][3] * m[3][2] +
                           m[2][0] * m[1][2] * m[3][3] -
                           m[2][0] * m[1][3] * m[3][2] -
                           m[3][0] * m[1][2] * m[2][3] +
                           m[3][0] * m[1][3] * m[2][2];

        inverse.m[2][0] = m[1][0] * m[2][1] * m[3][3] -
                          m[1][0] * m[2][3] * m[3][1] -
                          m[2][0] * m[1][1] * m[3][3] +
                          m[2][0] * m[1][3] * m[3][1] +
                          m[3][0] * m[1][1] * m[2][3] -
                          m[3][0] * m[1][3] * m[2][1];

        inverse.m[3][0] = -m[1][0] * m[2][1] * m[3][2] +
                           m[1][0] * m[2][2] * m[3][1] +
                           m[2][0] * m[1][1] * m[3][2] -
                           m[2][0] * m[1][2] * m[3][1] -
                           m[3][0] * m[1][1] * m[2][2] +
                           m[3][0] * m[1][2] * m[2][1];

        inverse.m[0][1] = -m[0][1] * m[2][2] * m[3][3] +
                           m[0][1] * m[2][3] * m[3][2] +
                           m[2][1] * m[0][2] * m[3][3] -
                           m[2][1] * m[0][3] * m[3][2] -
                           m[3][1] * m[0][2] * m[2][3] +
                           m[3][1] * m[0][3] * m[2][2];

        inverse.m[1][1] = m[0][0] * m[2][2] * m[3][3] -
                          m[0][0] * m[2][3] * m[3][2] -
                          m[2][0] * m[0][2] * m[3][3] +
                          m[2][0] * m[0][3] * m[3][2] +
                          m[3][0] * m[0][2] * m[2][3] -
                          m[3][0] * m[0][3] * m[2][2];

        inverse.m[2][1] = -m[0][0] * m[2][1] * m[3][3] +
                           m[0][0] * m[2][3] * m[3][1] +
                           m[2][0] * m[0][1] * m[3][3] -
                           m[2][0] * m[0][3] * m[3][1] -
                           m[3][0] * m[0][1] * m[2][3] +
                           m[3][0] * m[0][3] * m[2][1];

        inverse.m[3][1] = m[0][0] * m[2][1] * m[3][2] -
                          m[0][0] * m[2][2] * m[3][1] -
                          m[2][0] * m[0][1] * m[3][2] +
                          m[2][0] * m[0][2] * m[3][1] +
                          m[3][0] * m[0][1] * m[2][2] -
                          m[3][0] * m[0][2] * m[2][1];

        inverse.m[0][2] = m[0][1] * m[1][2] * m[3][3] -
                          m[0][1] * m[1][3] * m[3][2] -
                          m[1][1] * m[0][2] * m[3][3] +
                          m[1][1] * m[0][3] * m[3][2] +
                          m[3][1] * m[0][2] * m[1][3] -
                          m[3][1] * m[0][3] * m[1][2];

        inverse.m[1][2] = -m[0][0] * m[1][2] * m[3][3] +
                           m[0][0] * m[1][3] * m[3][2] +
                           m[1][0] * m[0][2] * m[3][3] -
                           m[1][0] * m[0][3] * m[3][2] -
                           m[3][0] * m[0][2] * m[1][3] +
                           m[3][0] * m[0][3] * m[1][2];

        inverse.m[2][2] = m[0][0] * m[1][1] * m[3][3] -
                          m[0][0] * m[1][3] * m[3][1] -
                          m[1][0] * m[0][1] * m[3][3] +
                          m[1][0] * m[0][3] * m[3][1] +
                          m[3][0] * m[0][1] * m[1][3] -
                          m[3][0] * m[0][3] * m[1][1];

        inverse.m[3][2] = -m[0][0] * m[1][1] * m[3][2] +
                           m[0][0] * m[1][2] * m[3][1] +
                           m[1][0] * m[0][1] * m[3][2] -
                           m[1][0] * m[0][2] * m[3][1] -
                           m[3][0] * m[0][1] * m[1][2] +
                           m[3][0] * m[0][2] * m[1][1];

        inverse.m[0][3] = -m[0][1] * m[1][2] * m[2][3] +
                           m[0][1] * m[1][3] * m[2][2] +
                           m[1][1] * m[0][2] * m[2][3] -
                           m[1][1] * m[0][3] * m[2][2] -
                           m[2][1] * m[0][2] * m[1][3] +
                           m[2][1] * m[0][3] * m[1][2];

        inverse.m[1][3] = m[0][0] * m[1][2] * m[2][3] -
                          m[0][0] * m[1][3] * m[2][2] -
                          m[1][0] * m[0][2] * m[2][3] +
                          m[1][0] * m[0][3] * m[2][2] +
                          m[2][0] * m[0][2] * m[1][3] -
                          m[2][0] * m[0][3] * m[1][2];

        inverse.m[2][3] = -m[0][0] * m[1][1] * m[2][3] +
                           m[0][0] * m[1][3] * m[2][1] +
                           m[1][0] * m[0][1] * m[2][3] -
                           m[1][0] * m[0][3] * m[2][1] -
                           m[2][0] * m[0][1] * m[1][3] +
                           m[2][0] * m[0][3] * m[1][1];

        inverse.m[3][3] = m[0][0] * m[1][1] * m[2][2] -
                          m[0][0] * m[1][2] * m[2][1] -
                          m[1][0] * m[0][1] * m[2][2] +
                          m[1][0] * m[0][2] * m[2][1] +
                          m[2][0] * m[0][1] * m[1][2] -
                          m[2][0] * m[0][2] * m[1][1];

        float determinant = m[0][0] * inverse.m[0][0] + m[0][1] * inverse.m[1][0] + m[0][2] * inverse.m[2][0] + m[0][3] * inverse.m[3][0];

        if (determinant == 0)
        {
            return Matrix();
        }

        determinant = 1.0f / determinant;

        for (int i = 0; i < MATRIX_NUM_ROWS; i++)
            for (int j = 0; j < MATRIX_NUM_COLUMNS; j++)
                inverse.m[i][j] *= determinant;
        
        return inverse;
    }

    Vector2 Matrix::operator *(const Vector2 aVector2) const
    {
        float x = m[0][0] * aVector2.x + m[1][0] * aVector2.y + 0.0f + m[3][0] * 1.0f;
        float y = m[0][1] * aVector2.x + m[1][1] * aVector2.y + 0.0f + m[3][1] * 1.0f;
        float z = m[0][2] * aVector2.x + m[1][2] * aVector2.y + 0.0f + m[3][2] * 1.0f;
        float w = m[0][3] * aVector2.x + m[1][3] * aVector2.y + 0.0f + m[3][3] * 1.0f;

        if (w == 0.0f)
        {
            return Vector2(x, y);
        }

        return Vector2(x / w, y / w);
    }

    Matrix Matrix::operator *(const Matrix aMatrix) const
    {
        Matrix matrix;

        matrix.m[0][0] = m[0][0] * aMatrix.m[0][0] + m[1][0] * aMatrix.m[0][1] + m[2][0] * aMatrix.m[0][2] + m[3][0] * aMatrix.m[0][3];
        matrix.m[0][1] = m[0][1] * aMatrix.m[0][0] + m[1][1] * aMatrix.m[0][1] + m[2][1] * aMatrix.m[0][2] + m[3][1] * aMatrix.m[0][3];
        matrix.m[0][2] = m[0][2] * aMatrix.m[0][0] + m[1][2] * aMatrix.m[0][1] + m[2][2] * aMatrix.m[0][2] + m[3][2] * aMatrix.m[0][3];
        matrix.m[0][3] = m[0][3] * aMatrix.m[0][0] + m[1][3] * aMatrix.m[0][1] + m[2][3] * aMatrix.m[0][2] + m[3][3] * aMatrix.m[0][3];
        matrix.m[1][0] = m[0][0] * aMatrix.m[1][0] + m[1][0] * aMatrix.m[1][1] + m[2][0] * aMatrix.m[1][2] + m[3][0] * aMatrix.m[1][3];
        matrix.m[1][1] = m[0][1] * aMatrix.m[1][0] + m[1][1] * aMatrix.m[1][1] + m[2][1] * aMatrix.m[1][2] + m[3][1] * aMatrix.m[1][3];
        matrix.m[1][2] = m[0][2] * aMatrix.m[1][0] + m[1][2] * aMatrix.m[1][1] + m[2][2] * aMatrix.m[1][2] + m[3][2] * aMatrix.m[1][3];
        matrix.m[1][3] = m[0][3] * aMatrix.m[1][0] + m[1][3] * aMatrix.m[1][1] + m[2][3] * aMatrix.m[1][2] + m[3][3] * aMatrix.m[1][3];
        matrix.m[2][0] = m[0][0] * aMatrix.m[2][0] + m[1][0] * aMatrix.m[2][1] + m[2][0] * aMatrix.m[2][2] + m[3][0] * aMatrix.m[2][3];
        matrix.m[2][1] = m[0][1] * aMatrix.m[2][0] + m[1][1] * aMatrix.m[2][1] + m[2][1] * aMatrix.m[2][2] + m[3][1] * aMatrix.m[2][3];
        matrix.m[2][2] = m[0][2] * aMatrix.m[2][0] + m[1][2] * aMatrix.m[2][1] + m[2][2] * aMatrix.m[2][2] + m[3][2] * aMatrix.m[2][3];
        matrix.m[2][3] = m[0][3] * aMatrix.m[2][0] + m[1][3] * aMatrix.m[2][1] + m[2][3] * aMatrix.m[2][2] + m[3][3] * aMatrix.m[2][3];
        matrix.m[3][0] = m[0][0] * aMatrix.m[3][0] + m[1][0] * aMatrix.m[3][1] + m[2][0] * aMatrix.m[3][2] + m[3][0] * aMatrix.m[3][3];
        matrix.m[3][1] = m[0][1] * aMatrix.m[3][0] + m[1][1] * aMatrix.m[3][1] + m[2][1] * aMatrix.m[3][2] + m[3][1] * aMatrix.m[3][3];
        matrix.m[3][2] = m[0][2] * aMatrix.m[3][0] + m[1][2] * aMatrix.m[3][1] + m[2][2] * aMatrix.m[3][2] + m[3][2] * aMatrix.m[3][3];
        matrix.m[3][3] = m[0][3] * aMatrix.m[3][0] + m[1][3] * aMatrix.m[3][1] + m[2][3] * aMatrix.m[3][2] + m[3][3] * aMatrix.m[3][3];

        return matrix;
    }
}

