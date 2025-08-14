#pragma once
#include <Volt.hpp>

namespace Volt
{
	//
	// Mathematical Utilities for 3D Rendering
	//

	// Defines a two-dimensional vector
	struct Vector2
	{
		float x, y;

		Vector2 operator +(const Vector2& other);	// Adds two two-dimensional vectors.
		Vector2 operator -(const Vector2& other);	// Subtracts two two-dimensional vectors
		Vector2 operator *(float& scale);			// Multiplies a two-dimensional vector by a scalar
	
		float Magnitude(const Vector2& other);		// Calculates the magnitude of a two-dimensional vector.
		float DotProduct(const Vector2& other);		// Calculates the dot product of a two-dimensional vector.
	};

	struct Vector3
	{
		float x, y, z;

		Vector3 operator +(const Vector3& other);	// Adds two three-dimensional vectors.
		Vector3 operator -(const Vector3& other);	// Subtracts two three-dimensional vectors
		Vector3 operator *(float& scale);			// Multiplies a three-dimensional vector by a scalar

		float Magnitude(const Vector3& other);		// Calculates the magnitude of a three-dimensional vector.
		float DotProduct(const Vector3& other);		// Calculates the dot product of a three-dimensional vector.
	};

	struct Vector4
	{
		float x, y, z, w;

		Vector4 operator +(const Vector4& other);	// Adds two four-dimensional vectors.
		Vector4 operator -(const Vector4& other);	// Subtracts two four-dimensional vectors
		Vector4 operator *(float& scale);			// Multiplies a four-dimensional vector by a scalar

		float Magnitude(const Vector4& other);		// Calculates the magnitude of a four-dimensional vector.
		float DotProduct(const Vector4& other);		// Calculates the dot product of a four-dimensional vector.
	};

	struct Matrix22
	{
		float matrix[2][2];

		Matrix22 Transpose();						// Get the matrix of a 2x2 matrix.
		float Determinant(const Matrix22& other);	// Gets the determinant of a 2x2 matrix.
		Matrix22 GetInverse(const Matrix22& other);	// Gets theinverse of a 2x2 matrix.

		static Matrix22 GetIdentityMatrix();		// Get the 2x2 identity matrix.
	};

	struct Matrix33
	{
		float matrix[3][3];

		Matrix33 Transpose();						// Get the matrix of a 3x3 matrix.
		float Determinant();						// Gets the determinant of a 3x3 matrix.
		Matrix33 GetInverse(const Matrix33& other);	// Gets theinverse of a 3x3 matrix.

		static Matrix33 GetIdentityMatrix();		// Get the 3x3 identity matrix.
	};

	struct Matrix44
	{
		float matrix[4][4];

		Matrix44 Transpose();						// Get the matrix of a 4x4 matrix.
		float Determinant();						// Gets the determinant of a 4x4 matrix.
		Matrix44 GetInverse(const Matrix44& other);	// Gets theinverse of a 4x4 matrix.

		static Matrix44 GetIdentityMatrix();		// Get the 4x4 identity matrix.
	};
}
