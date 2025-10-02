#pragma once
#include <Volt.hpp>

namespace Volt
{
	//
	// Mathematical Utilities for 3D Rendering implementation
	//

	// Defines a two-dimensional vector where the dimensions are expressed as floating-point coordinates
	struct Vector2
	{
		float x, y;

		Vector2 operator +(const Vector2& other);	// Adds two two-dimensional vectors.
		Vector2 operator -(const Vector2& other);	// Subtracts two two-dimensional vectors
		Vector2 operator *(float& scale);			// Multiplies a two-dimensional vector by a scalar
	
		float Magnitude(const Vector2& other);		// Calculates the magnitude of a two-dimensional vector.
		float DotProduct(const Vector2& other);		// Calculates the dot product of a two-dimensional vector.
	};

	// Defines a two-dimensional vector where the dimensions are expressed as 32-bit signed integers.
	struct Vector2i
	{
		uint32_t x, y;

		Vector2i operator +(const Vector2i& other);	// Adds two two-dimensional integer vectors.
		Vector2i operator -(const Vector2i& other);	// Subtracts two two-dimensional integer vectors
		Vector2i operator *(float& scale);			// Multiplies a two-dimensional integer vector by a scalar

		int32_t Magnitude(const Vector2i& other);	// Calculates the magnitude of a two-dimensional vector.
		int32_t DotProduct(const Vector2i& other);	// Calculates the dot product of a two-dimensional vector.
	};

	// Defines a three-dimensional vector where the dimensions are expressed as floating-point coordinates.
	struct Vector3
	{
		float x, y, z;

		Vector3 operator +(const Vector3& other);	// Adds two three-dimensional vectors.
		Vector3 operator -(const Vector3& other);	// Subtracts two three-dimensional vectors
		Vector3 operator *(float& scale);			// Multiplies a three-dimensional vector by a scalar

		float Magnitude(const Vector3& other);		// Calculates the magnitude of a three-dimensional vector.
		float DotProduct(const Vector3& other);		// Calculates the dot product of a three-dimensional vector.
	};

	// Defines a three-dimensional vector where the dimensions are expressed as 32-bit signed integers.
	struct Vector3i
	{
		int32_t x, y, z;

		Vector3i operator +(const Vector3i& other);	// Adds two three-dimensional vectors.
		Vector3i operator -(const Vector3i& other);	// Subtracts two three-dimensional vectors
		Vector3i operator *(float& scale);			// Multiplies a three-dimensional vector by a scalar

		int32_t Magnitude(const Vector3i& other);	// Calculates the magnitude of a three-dimensional vector.
		int32_t DotProduct(const Vector3i& other);	// Calculates the dot product of a three-dimensional vector.
	};

	// Defines a four-dimensional vector where the dimensions are expressed as floating-point coordinates.
	struct Vector4
	{
		float x, y, z, w;

		Vector4 operator +(const Vector4& other);	// Adds two four-dimensional vectors.
		Vector4 operator -(const Vector4& other);	// Subtracts two four-dimensional vectors
		Vector4 operator *(float& scale);			// Multiplies a four-dimensional vector by a scalar

		float Magnitude(const Vector4& other);		// Calculates the magnitude of a four-dimensional vector.
		float DotProduct(const Vector4& other);		// Calculates the dot product of a four-dimensional vector.
	};

	// Defines a four-dimensional vector where the dimensions are expressed as 32-bit signed integers.
	struct Vector4i
	{
		int32_t x, y, z, w;

		Vector4i operator +(const Vector4i& other);	// Adds two four-dimensional vectors.
		Vector4i operator -(const Vector4i& other);	// Subtracts two four-dimensional vectors
		Vector4i operator *(float& scale);			// Multiplies a four-dimensional vector by a scalar

		int32_t Magnitude(const Vector4i& other);	// Calculates the magnitude of a four-dimensional vector.
		int32_t DotProduct(const Vector4i& other);	// Calculates the dot product of a four-dimensional vector.
	};

	struct Matrix22
	{
		float matrix[2][2];

		Matrix22 Transpose();						// Get the transposition of a 2x2 matrix.
		Matrix22 Translate(const Matrix22& other);	// Translate a 2x2 matrix by applying a transformation matrix to it
		Matrix22 Scale(const Matrix22& other);		// Scale a 2x2 matrix.
		Matrix22 Rotate(const Matrix22& other);		// Rotate a 2x2 matrix.

		static Matrix22 GetIdentityMatrix();		// Get the 2x2 identity matrix.
	};

	struct Matrix33
	{
		float matrix[3][3];

		Matrix33 Transpose();						// Get the transposition of a 3x3 matrix.
		Matrix33 Translate(const Matrix33& other);	// Translate a 3x3 matrix.
		Matrix33 Scale(const Matrix33& other);		// Scale a 3x3 matrix.
		Matrix33 Rotate(const Matrix33& other);		// Rotate a 3x3 matrix.

		static Matrix33 GetIdentityMatrix();		// Get the 3x3 identity matrix.
	};

	struct Matrix44
	{
		float matrix[4][4];

		Matrix44 Transpose();						// Get the transposition of a 4x4 matrix.
		Matrix44 Translate(const Matrix44& other);	// Translate a 3x3 matrix.
		Matrix44 Scale(const Matrix44& other);		// Scale a 3x3 matrix.
		Matrix44 Rotate(const Matrix44& other);		// Rotate a 3x3 matrix.

		static Matrix44 GetIdentityMatrix();		// Get the 4x4 identity matrix.
	};
}
