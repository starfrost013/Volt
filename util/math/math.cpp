
#include "math.hpp"

namespace Volt
{
	//
	// math.cpp
	// Vector and Matrices Library
	// 18 November 2024
	//

	// Vector Dot Product

	// Gets the dot product of a 2-dimensional vector.
	float Vector2::DotProduct(const Vector2& other)
	{
		return (x * other.y) + (y * other.y);
	}

	// Gets the dot product of a 2-dimensional integer vector.
	int32_t Vector2i::DotProduct(const Vector2i& other)
	{
		return (x * other.y) + (y * other.y);
	}

	// Gets the dot product of a 3-dimensional vector.
	float Vector3::DotProduct(const Vector3& other)
	{
		return (x * other.y) + (y * other.y) + (z * other.z);
	}

	// Gets the dot product of a 3-dimensional integer vector.
	int32_t Vector3i::DotProduct(const Vector3i& other)
	{
		return (x * other.y) + (y * other.y) + (z * other.z);
	}

	// Gets the dot product of a 4-dimensional vector.
	float Vector4::DotProduct(const Vector4& other)
	{
		return (x * other.y) + (y * other.y) + (z * other.z) + (w * other.w);
	}

	// Gets the dot product of a 4-dimensional integer vector.
	int32_t Vector4i::DotProduct(const Vector4i& other)
	{
		return (x * other.y) + (y * other.y) + (z * other.z) + (w * other.w);
	}

	// Vector Length
	// doing math of the form (x * x) is faster than using dedicated power by 2 instructions
	
	// Gets the magnitude of a two-dimensional vector.
	float Vector2::Magnitude(const Vector2& other)
	{
		return (x * x) + (y * y); 		
	}

	// Gets the magnitude of a two-dimensional integer vector.
	int32_t Vector2i::Magnitude(const Vector2i& other)
	{
		return (x * x) + (y * y); 		
	}

	// Gets the magnitude of a three-dimensional vector.
	float Vector3::Magnitude(const Vector3& other)
	{
		return (x * x) + (y * y) + (z * z);
	}

	// Gets the magnitude of a three-dimensional integer vector.
	int32_t Vector3i::Magnitude(const Vector3i& other)
	{
		return (x * x) + (y * y) + (z * z);
	}

	// Gets the magnitude of a four-dimensional vector.
	float Vector4::Magnitude(const Vector4& other)
	{
		return (x * x) + (y * y) + (z * z) + (w * w);
	}

	// Gets the magnitude of a four-dimensional integer vector.
	int32_t Vector4i::Magnitude(const Vector4i& other)
	{
		return (x * x) + (y * y) + (z * z) + (w * w);
	}

	// Vector Addition

	// Adds two two-dimensional vectors.
	Vector2 Vector2::operator+(const Vector2& other)
	{
		return { x + other.x, y + other.y };
	}

	// Adds two two-dimensional integer vectors.
	Vector2i Vector2i::operator+(const Vector2i& other)
	{
		return { x + other.x, y + other.y };
	}

	// Adds two three-dimensional vectors.
	Vector3 Vector3::operator+(const Vector3& other)
	{
		return { x + other.x, y + other.y, z + other.z };
	}
	
	// Adds two three-dimensional integer vectors.
	Vector3i Vector3i::operator+(const Vector3i& other)
	{
		return { x + other.x, y + other.y, z + other.z };
	}

	// Adds two four-dimensional vectors.
	Vector4 Vector4::operator+(const Vector4& other)
	{
		return { x + other.x, y + other.y, z + other.z, w + other.w };
	}

	// Adds two four-dimensional integer vectors.
	Vector4i Vector4i::operator+(const Vector4i& other)
	{
		return { x + other.x, y + other.y, z + other.z, w + other.w };
	}

	//
	// Vector Subtract
	//

	// Subtracts two two-dimensional vectors.
	Vector2 Vector2::operator-(const Vector2& other)
	{
		return { x - other.x, y - other.y };
	}

	// Subtracts two two-dimensional integer vectors.
	Vector2i Vector2i::operator-(const Vector2i& other)
	{
		return { x - other.x, y - other.y };
	}

	// Subtracts two three-dimensional vectors.
	Vector3 Vector3::operator-(const Vector3& other)
	{
		return { x - other.x, y - other.y, z - other.z };
	}

	// Subtracts two three-dimensional integer vectors.
	Vector3i Vector3i::operator-(const Vector3i& other)
	{
		return { x - other.x, y - other.y, z - other.z };
	}

	// Subtracts two four-dimensional vectors.
	Vector4 Vector4::operator-(const Vector4& other)
	{
		return { x - other.x, y - other.y, z - other.z, w - other.w };
	}

	// Subtracts two four-dimensional integer vectors.
	Vector4i Vector4i::operator-(const Vector4i& other)
	{
		return { x - other.x, y - other.y, z - other.z, w - other.w };
	}

	// Vector Multiply

	// Scales a two-dimensional vector by a scalar value.
	Vector2 Vector2::operator*(float& scale)
	{
		return { x * scale, y * scale };
	}

	// Scales a two-dimensional integer vector by a scalar value.
	Vector2i Vector2i::operator*(float& scale)
	{
		return { x * scale, y * scale };
	}

	// Scales a three-dimensional vector by a scalar value.
	Vector3 Vector3::operator*(float& scale)
	{
		return { x * scale, y * scale, z * scale };
	}

	// Scales a three-dimensional integer vector by a scalar value.
	Vector3i Vector3i::operator*(float& scale)
	{
		return { x * scale, y * scale, z * scale };
	}

	// Scales a four-dimensional vector by a scalar value.
	Vector4 Vector4::operator*(float& scale)
	{
		return { x * scale, y * scale, z * scale, w * scale };
	}

	// Scales a three-dimensional integer vector by a scalar value.
	Vector4i Vector4i::operator*(float& scale)
	{
		return { x * scale, y * scale, z * scale };
	}


	// Matrix Transposition

	// Returns the transposition of a 2x2 matrix.
	Matrix22 Matrix22::Transpose()
	{
		return { 
			matrix[0][0] = matrix[0][0], 
			matrix[0][1] = matrix[1][0],
			matrix[1][0] = matrix[0][1], 
			matrix[1][1] = matrix[1][1]
		};
	}

	Matrix22 Matrix22::Translate(const Matrix22& other)
	{
		return {
			matrix[0][0] + other.matrix[0][0],
			matrix[0][1] + other.matrix[0][1],
			matrix[1][0] + other.matrix[1][0],
			matrix[1][1] + other.matrix[1][1],
		};
	}

	Matrix33 Matrix33::Transpose()
	{
		Matrix33 transposed;
		
		for (uint8_t i = 0; i < 3; i++)
			for (uint8_t j = 0; j < 3; j++)
				transposed.matrix[i][j] = matrix[j][i];

		return transposed;
	}

	// Translate a 3x3 matric.
	Matrix33 Matrix33::Translate(const Matrix33& other)
	{
		Matrix33 new_matrix = {0};

		for (uint8_t i = 0; i < 3; i++)
			for (uint8_t j = 0; j < 3; j++)
				new_matrix.matrix[i][j] = matrix[i][j] + other.matrix[i][j];

		return new_matrix;
	}

	// Transpose a 4x4 matrix.
	Matrix44 Matrix44::Transpose()
	{
		Matrix44 transposed;

		for (uint8_t i = 0; i < 4; i++)
			for (uint8_t j = 0; j < 4; j++)
				transposed.matrix[i][j] = matrix[j][i];

		return transposed;
	}

	// Translate a 4x4 matrix.
	Matrix44 Matrix44::Translate(const Matrix44& other)
	{
		Matrix44 new_matrix = {0};

		for (uint8_t i = 0; i < 4; i++)
			for (uint8_t j = 0; j < 4; j++)
				new_matrix.matrix[i][j] = matrix[i][j] + other.matrix[i][j];

		return new_matrix;
	}

	// Obtain Identity Matrix methods

	// Returns the 2x2 identity matrix.
	Matrix22 Matrix22::GetIdentityMatrix()
	{
		Matrix22 new_mat = { 0 }; //sets the rest of the fields to 0
		new_mat.matrix[0][0] = new_mat.matrix[1][1] = 1;
		return new_mat;
	}

	// Returns the 3x3 identity matrix.
	Matrix33 Matrix33::GetIdentityMatrix()
	{
		Matrix33 new_mat = { 0 }; //sets the rest of the fields to 0
		new_mat.matrix[0][0] = new_mat.matrix[1][1] = new_mat.matrix[2][2] = 1;
		return new_mat;
	}

	// Returns the 4x4 identity matrix.
	Matrix44 Matrix44::GetIdentityMatrix()
	{
		Matrix44 new_mat = { 0 }; //sets the rest of the fields to 0
		new_mat.matrix[0][0] = new_mat.matrix[1][1] = new_mat.matrix[2][2] = new_mat.matrix[3][3] = 1;
		return new_mat;
	}
}