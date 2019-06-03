#include "Matrix.h"
#include <cmath>

Matrix4::~Matrix4()
{
}

Matrix4 Matrix4::Identity()
{
	Matrix4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
				result.m_data[i][j] = 1;
			else
				result.m_data[i][j] = 0;
		}
	}
	return result;
}

Matrix4 Matrix4::Zero()
{
	Matrix4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m_data[i][j] = 0;
		}
	}
	return result;
}

Matrix4 Matrix4::Translate(Vector3 translation)
{
	Matrix4 result;
	Vector4 trans(translation, 0);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (j == 3)
				result.m_data[i][j] = *trans[i];
			else if (i == j)
				result.m_data[i][j] = 1;
			else
				result.m_data[i][j] = 0;
		}
	}
	return result;
}

Matrix4 Matrix4::RotateYawPitchRoll(Vector3 rotation)
{
Matrix4 result;

	//Pitch
	float sinX = sin(rotation.x);
	float cosX = cos(rotation.x);

	//Yaw
	float sinY = sin(rotation.y);
	float cosY = cos(rotation.y);

	//Roll
	float sinZ = sin(rotation.z);
	float cosZ = cos(rotation.z);

	//row 1
	result.m_data[0][0] = cosY * cosZ + sinY * sinX * sinZ;
	result.m_data[1][0] = cosY * -sinZ + sinY * sinX * cosZ;
	result.m_data[2][0] = sinY * cosX;

	//row 2
	result.m_data[0][1] = cosX * sinZ;
	result.m_data[1][1] = cosX * cosZ;
	result.m_data[2][1] = -sinX;
	//row 3
	result.m_data[0][2] = -sinY * cosZ + sinX * cosY * sinZ;
	result.m_data[1][2] = -sinY * -sinZ + sinX * cosY * cosZ;
	result.m_data[2][2] = cosX * cosY;

	//filling the border
	result.m_data[3][0] = 0;
	result.m_data[3][1] = 0;
	result.m_data[3][2] = 0;
	result.m_data[3][3] = 1;
	result.m_data[0][3] = 0;
	result.m_data[1][3] = 0;
	result.m_data[2][3] = 0;

	return result;
}

Matrix4 Matrix4::RotateX(float amount)
{
	Matrix4 result;

	result.m_data[0][0] = 1;
	result.m_data[1][1] = cos(amount);
	result.m_data[2][1] = -sin(amount);
	result.m_data[1][2] = sin(amount);
	result.m_data[2][2] = cos(amount);
	result.m_data[3][3] = 1;
	return result;
}

Matrix4 Matrix4::RotateY(float amount)
{
	Matrix4 result;

	result.m_data[0][0] = cos(amount);
	result.m_data[2][0] = sin(amount);
	result.m_data[1][1] = 1;
	result.m_data[0][2] = -sin(amount);
	result.m_data[2][2] = cos(amount);
	result.m_data[3][3] = 1;
	return result;
}

Matrix4 Matrix4::RotateZ(float amount)
{
	Matrix4 result;

	result.m_data[0][0] = cos(amount);
	result.m_data[1][0] = -sin(amount);
	result.m_data[0][1] = sin(amount);
	result.m_data[1][1] = cos(amount);
	result.m_data[2][2] = 1;
	result.m_data[3][3] = 1;

	return result;
}

Matrix4 Matrix4::Scale(const Vector3& scale)
{
	Matrix4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
				result.m_data[i][j] = scale.Get(j);
			else
				result.m_data[i][j] = 0;
		}
	}
	return result;
}

Matrix4 Matrix4::Translate(const Vector4& translation)
{
	Matrix4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == 3)
				result.m_data[i][j] = translation.Get(j);
			else if (i = j)
				result.m_data[i][j] = 1;
			else
				result.m_data[i][j] = 0;
		}
	}
	return result;
}

Matrix4 Matrix4::Scale(const Vector4& scale)
{
	Matrix4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i = j)
				result.m_data[i][j] = scale.Get(j);
			else
				result.m_data[i][j] = 0;
		}
	}
	return result;
}


Vector3 Matrix4::operator*(const Vector3& colVec)
{
	if (colVec.SqrMagnitude() < 0.1f)
		return Vector3(0, 0, 0);
	return Vector3(
		m_data[0][0] * colVec.x + m_data[0][1] * colVec.y + m_data[0][2] * colVec.z + m_data[0][3],
		m_data[1][0] * colVec.x + m_data[1][1] * colVec.y + m_data[1][2] * colVec.z + m_data[1][3],
		m_data[2][0] * colVec.x + m_data[2][1] * colVec.y + m_data[2][2] * colVec.z + m_data[2][3]);
}


Vector4 Matrix4::operator*(const Vector4& colVec)
{
	return Vector4(
		m_data[0][0] * colVec.x + m_data[0][1] * colVec.y + m_data[0][2] * colVec.z + m_data[0][3] * colVec.w,
		m_data[1][0] * colVec.x + m_data[1][1] * colVec.y + m_data[1][2] * colVec.z + m_data[1][3] * colVec.w,
		m_data[2][0] * colVec.x + m_data[2][1] * colVec.y + m_data[2][2] * colVec.z + m_data[2][3] * colVec.w,
		m_data[3][0] * colVec.x + m_data[3][1] * colVec.y + m_data[3][2] * colVec.z + m_data[3][3] * colVec.w);
}


void Matrix4::operator=(const Matrix4& matrix)
{
	memcpy(this, &matrix, sizeof(Matrix4));
}

float& Matrix4::operator()(int row, int col)
{
	return m_data[row][col];
}

Matrix4 Matrix4::operator*(const Matrix4 matrix)
{
	Matrix4 result;
	for (unsigned int i = 0; i < 4; i++) //row
	{
		for (unsigned int j = 0; j < 4; j++)//col
		{
			//Loops through second matrix column and puts in result col
			for (unsigned int d = 0; d < 4; d++)
				result(i, d) += (*this)(i, j) * matrix.Get(j, d);
		}
	}
	return result;
}