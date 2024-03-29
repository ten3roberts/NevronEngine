#include <src/pch.h>
#include "Matrix.h"
#include "Quaternion.h"
#include "Math.h"
#include <cmath>

using namespace Utility;

Matrix4 Matrix4::identity = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
Matrix4 Matrix4::zero = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

Matrix4 Matrix4::Parse(const std::string& str)
{
	Matrix4 result;
	std::vector<std::string> completeRows = strSplit(strPurge(str, " "), "\n");
	if (completeRows.size() == 1) //Inline format
		return ParseInline(str);

	for (unsigned int i = 0; i < 4; i++)
	{
		std::vector<std::string> cols = strSplit(completeRows[i], ",");
		for (unsigned int j = 0; j < 4; j++)
			result(i, j) = numf(cols[j]);
	}
	return result;
}

Matrix4 Matrix4::ParseInline(const std::string& str)
{
	Matrix4 result;
	std::vector<std::string> elems = strSplit(strPurge(str, " "), ",");
	for (unsigned int i = 0; i < 16; i++)
		* result[i] = numf(elems[i]);
	return result;
}

Matrix4::~Matrix4()
{
}

Matrix4 Matrix4::Identity()
{
	return identity;
}

Matrix4 Matrix4::Zero()
{
	return zero;
}

Matrix4 Matrix4::Euler(Vector3 euler)
{
	// Calculate rotation about x axis
	Matrix4 pitchM =
	{
		1, 0, 0, 0,
		0, cosf(euler.pitch), -sinf(euler.pitch), 0,
		0, sinf(euler.pitch), cosf(euler.pitch), 0,
		0, 0, 0, 0 };
	// Calculate rotation about y axis
	Matrix4 yawM =
	{
		cosf(euler.yaw), 0, sinf(euler.yaw), 0,
		0, 1, 0, 0,
		-sinf(euler.yaw), 0, cosf(euler.yaw), 0,
		0, 0, 0, 0
	};

	// Calculate rotation about z axis
	Matrix4 rollM =
	{
		cosf(euler.roll), -sinf(euler.roll), 0,
		sinf(euler.roll), cosf(euler.roll), 0,
		0, 0, 1, 0,
		0, 0, 0, 0
	};


	// Combined rotation matrix
	//Matrix4 R = R_z * R_y * R_x;

	return (yawM * pitchM * rollM);
}

Matrix4 Matrix4::RotateX(float amount)
{
	Matrix4 result;

	result.m_data[0][0] = 1;
	result.m_data[1][1] = cosf(amount);
	result.m_data[2][1] = -sinf(amount);
	result.m_data[1][2] = sinf(amount);
	result.m_data[2][2] = cosf(amount);
	result.m_data[3][3] = 1;
	return result;
}

Matrix4 Matrix4::RotateY(float amount)
{
	Matrix4 result;

	result.m_data[0][0] = cosf(amount);
	result.m_data[2][0] = sinf(amount);
	result.m_data[1][1] = 1;
	result.m_data[0][2] = -sinf(amount);
	result.m_data[2][2] = cosf(amount);
	result.m_data[3][3] = 1;
	return result;
}

Matrix4 Matrix4::RotateZ(float amount)
{
	Matrix4 result;

	result.m_data[0][0] = cosf(amount);
	result.m_data[1][0] = -sinf(amount);
	result.m_data[0][1] = sinf(amount);
	result.m_data[1][1] = cosf(amount);
	result.m_data[2][2] = 1;
	result.m_data[3][3] = 1;

	return result;
}

Matrix4 Matrix4::Rotate(Quaternion rotation)
{
	return
	{
	1 - 2 * rotation.y * rotation.y - 2 * rotation.z * rotation.z,		2 * rotation.x * rotation.y - 2 * rotation.z * rotation.w,		2 * rotation.x * rotation.z + 2 * rotation.y * rotation.w, 0,
		2 * rotation.x * rotation.y + 2 * rotation.z * rotation.w,	1 - 2 * rotation.x * rotation.x - 2 * rotation.z * rotation.z,		2 * rotation.y * rotation.z - 2 * rotation.x * rotation.w, 0,
		2 * rotation.x * rotation.z - 2 * rotation.y * rotation.w,		2 * rotation.y * rotation.z + 2 * rotation.x * rotation.w,	1 - 2 * rotation.x * rotation.x - 2 * rotation.y * rotation.y, 0,
		0,							0,							0,					   1
	};
}

Matrix4 Matrix4::Scale(const Vector3& scale)
{
	Matrix4 result;
	Vector4 scaling(scale, 1);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{

			if (i == j)
				result.m_data[i][j] = scaling.get(j);
			else
				result.m_data[i][j] = 0;
		}
	}
	return result;
}

Matrix4 Matrix4::Translate(Vector3 translation)
{
	Matrix4 result;
	Vector4 trans(translation, 1);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == 3)
				result.m_data[i][j] = trans.get(j);
			else if (i == j)
				result.m_data[i][j] = 1;
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
				result.m_data[i][j] = translation.get(j);
			else if (i == j)
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
				result.m_data[i][j] = scale.get(j);
			else
				result.m_data[i][j] = 0;
		}
	}
	return result;
}

Matrix4 Matrix4::Ortho(float left, float right, float bottom, float top, float nearZ, float farZ)
{
	Matrix4 result;
	result.m_data[0][0] = 2 / (right - left);
	result.m_data[3][0] = -(right + left) / (right - left);
	result.m_data[1][1] = 2 / (top - bottom);
	result.m_data[3][1] = -(top + bottom) / (top - bottom);
	result.m_data[2][2] = -2 / (farZ - nearZ);
	result.m_data[3][2] = -(farZ + nearZ) / (farZ - nearZ);
	result.m_data[3][3] = 1;

	return result;
}

Matrix4 Matrix4::Ortho(float width, float height, float nearZ, float farZ)
{
	Matrix4 result;
	result.m_data[0][0] = 2 / (width);
	result.m_data[1][1] = 2 / (height);
	result.m_data[2][2] = -2 / (farZ - nearZ);
	result.m_data[3][2] = -(farZ + nearZ) / (farZ - nearZ);
	result.m_data[3][3] = 1;
	return result;
}

Matrix4 Matrix4::Perspective(float fov, float aspect, float nearZ, float farZ)
{
	Matrix4 result;
	result.m_data[0][0] = 1 / (aspect * tan(fov / 2));
	result.m_data[1][1] = 1 / tan(fov / 2);
	result.m_data[2][2] = -(farZ + nearZ) / (farZ - nearZ);
	result.m_data[3][2] = -2 * (farZ * nearZ) / (farZ - nearZ);
	result.m_data[2][3] = -1;
	return result;
}

Vector3 Matrix4::operator*(const Vector3& colVec) const
{
	if (colVec.SqrMagnitude() < 0.1f)
		return Vector3(0, 0, 0);
	return Vector3(
		m_data[0][0] * colVec.x + m_data[0][1] * colVec.y + m_data[0][2] * colVec.z + m_data[0][3],
		m_data[1][0] * colVec.x + m_data[1][1] * colVec.y + m_data[1][2] * colVec.z + m_data[1][3],
		m_data[2][0] * colVec.x + m_data[2][1] * colVec.y + m_data[2][2] * colVec.z + m_data[2][3]);
}


Vector4 Matrix4::operator*(const Vector4& colVec) const
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

Matrix4 Matrix4::operator*(const Matrix4& matrix) const
{
	Matrix4 result;
	for (unsigned int i = 0; i < 4; i++) //row
	{
		for (unsigned int j = 0; j < 4; j++)//col
		{
			//Loops through second matrix column and puts in result col
			for (unsigned int d = 0; d < 4; d++)
				result(i, d) += get(i, j) * matrix.get(j, d);
		}
	}
	return result;
}

void Matrix4::operator*=(const Matrix4& matrix)
{
	Matrix4 result;
	for (unsigned int i = 0; i < 4; i++) //row
	{
		for (unsigned int j = 0; j < 4; j++)//col
		{
			//Loops through second matrix column and puts in result col
			for (unsigned int d = 0; d < 4; d++)
				result(i, d) += get(i, j) * matrix.get(j, d);
		}
	}
	*this = result;
}

Matrix Matrix::Parse(const std::string& str)
{
	std::vector<std::string> completeRows = strSplit(strPurge(str, " "), "\n");

	std::vector<std::vector<std::string>> elems;
	elems.push_back(strSplit(completeRows[0], ","));

	for (unsigned int i = 1; i < completeRows.size(); i++)
		elems.push_back(strSplit(completeRows[i], ","));

	Matrix result(elems[0].size(), elems.size());

	for (unsigned int i = 0; i < elems.size(); i++)
		for (unsigned int j = 0; j < elems[0].size(); j++)
		{
			result(i, j) = numf(elems[i][j]);
		}

	return result;
}

Matrix Matrix::ParseInline(unsigned int width, unsigned int height, const std::string& str)
{
	return Matrix();
}


