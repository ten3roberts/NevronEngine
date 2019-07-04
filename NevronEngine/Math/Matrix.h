#pragma once
#include "Vector.h"
#include <iostream>

struct Matrix4
{
	Matrix4() : m_data{ 0 } {}
	Matrix4(std::initializer_list<float> list) : m_data{ 0 }
	{
		memcpy(m_data, list.begin(), (16 < list.size() ? 16 : list.size()) * sizeof(float));
	}
	Matrix4(std::vector<float> list) : m_data{ 0 }
	{
		memcpy(m_data, &list[0], (4 < list.size() ? 4 : list.size()) * sizeof(float));
	}
	Matrix4(float min, float max) { memcpy(m_data, Random(min, max)[0], 16 * sizeof(float)); }
	~Matrix4();

	//Parsing
	static Matrix4 Parse(const std::string& str);
	static Matrix4 ParseInline(const std::string& str);



	static Matrix4 Identity();
	static Matrix4 Zero();

	static Matrix4 Random()
	{
		Matrix4 result;
		for (unsigned int i = 0; i < 16; i++)
			* result[i] = (float)std::rand() / RAND_MAX;
		return result;
	}

	static Matrix4 Random(float min, float max)
	{
		Matrix4 result;
		for (unsigned int i = 0; i < 16; i++)
			* result[i] = ((float)std::rand() / RAND_MAX) * (max - min) + min;
		return result;
	}

	

	//Rotates in the order Yaw, Pitch, Roll. x : pitch, y : yaw, z : roll. Yaw clockwise
	static Matrix4 Euler(Vector3 euler);
	static Matrix4 RotateX(float amount);
	static Matrix4 RotateY(float amount);
	static Matrix4 RotateZ(float amount);

	static Matrix4 Scale(const Vector3& scale);

	static Matrix4 Translate(Vector3 translation);
	static Matrix4 Translate(const Vector4& translation);
	static Matrix4 Scale(const Vector4& scale);

	//Will create an orthographic porjection matrix with the edges specified
	static Matrix4 Ortho(float left, float right, float bottom, float top, float nearZ, float farZ);

	//Will create an orthographic porjection matrix with the size specified centered around 0,0
	static Matrix4 Ortho(float width, float height, float nearZ, float farZ);

	//Will create an orthographic porjection matrix with the hieght and aspect ratio specified centered around 0,0
	inline static Matrix4 OrthoAspect(float height, float aspect, int nearZ, int farZ) { return Matrix4::Ortho(height * aspect, height, nearZ, farZ); }


	static Matrix4 Perspective(float fov, float aspect, float nearZ, float farZ);
	Vector3 operator*(const Vector3& colVec);


	Vector4 operator*(const Vector4& colVec);

	Matrix4 operator*(Matrix4 matrix);

	void operator=(const Matrix4& matrix);

	float& operator()(int row, int col);

	//Returns the element at $index; treating the matrix as a 1D array
	float* operator[](unsigned int index) { return &m_data[index / 4][index % 4]; }
	const float& get(unsigned int row, unsigned int col) const { return m_data[row][col]; }
	const float& get(unsigned index) const { return m_data[index / 4][index % 4]; }


	operator std::string() const
	{
		return str();
	}

	std::string str() const
	{
		std::string result;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result += std::to_string(m_data[i][j]);
				if (j < 3)
					result += ", ";
			}
			result += "\n";
		}
		return result;
	}

	std::string strInline()
	{
		std::string result;
		for (int i = 0; i < 16; i++)
		{
			result += std::to_string(get(i));
			if (i < 15)
				result += ", ";
		}
		return result;
	}

	//Return a copy of the matrix with the columns and rows swapped
	Matrix4 Transpose() const
	{
		Matrix4 result;
		for (unsigned int i = 0; i < 4; i++)
			for (unsigned int j = 0; j < 4; j++)
				result(j, i) = m_data[i][j];
		return result;
	}


private:
	//row, col
	float m_data[4][4];

};

struct Matrix
{
	//Doesn't allocate any memory
	Matrix() : m_height(0), m_width(0), m_size(0), m_data(0) {}

	//Sets all to zero
	Matrix(unsigned int height, unsigned int width) : m_height(height), m_width(width), m_size(height* width)
	{
		m_data = new float[m_size];
		for (unsigned int i = 0; i < m_size; i++)
			m_data[i] = 0;
	}
	Matrix(unsigned int height, unsigned int width, std::initializer_list<float> list) : m_height(height), m_width(width), m_size(height* width)
	{
		ASSERT(m_size == list.size());

		m_data = new float[m_height * m_width];

		memcpy(m_data, list.begin(), m_size * sizeof(float));
	}
	Matrix(unsigned int height, unsigned int width, std::vector<float> list) : m_height(height), m_width(width), m_size(height* width)
	{
		ASSERT(m_size == list.size());

		m_data = new float[m_height * m_width];

		memcpy(m_data, &list[0], m_size * sizeof(float));
	}

	Matrix(unsigned int height, unsigned int width, float min, float max) { memcpy(m_data, Random(height, width, min, max)[0], height * width * sizeof(float)); }


	Matrix(const Matrix& matrix)
	{
		m_height = matrix.height();
		m_width = matrix.width();
		m_size = matrix.size();

		m_data = new float[m_size];
		memcpy(m_data, matrix.m_data, m_size * sizeof(float));
	}
	~Matrix()
	{
		delete[] m_data;
	}

	//Parsing
	static Matrix Parse(const std::string& str);
	static Matrix ParseInline(unsigned int width, unsigned int height, const std::string& str);

	static Matrix Identity(unsigned int height, unsigned int width)
	{
		Matrix result(height, width);
		for (unsigned int i = 0; i < height; i++) //row
		{
			for (unsigned int j = 0; j < width; j++) //col
				if (i == j)
					result(i, j) = 1;
				else
					result(i, j) = 0;
		}
		return result;
	}
	static Matrix Zero(unsigned int height, unsigned int width)
	{
		Matrix result(height, width);
		for (unsigned int i = 0; i < height; i++)
			for (unsigned int j = 0; j < width; j++)
				result(i, j) = 0;
		return result;
	}
	static Matrix Random(unsigned int height, unsigned int width)
	{
		Matrix result(height, width);
		for (unsigned int i = 0; i < result.size(); i++)
			* result[i] = (float)std::rand() / RAND_MAX;
		return result;
	}
	static Matrix Random(unsigned int height, unsigned int width, float min, float max)
	{
		Matrix result(height, width);
		for (unsigned int i = 0; i < result.size(); i++)
			* result[i] = ((float)std::rand() / RAND_MAX) * (max - min) + min;
		return result;
	}

	Vector operator*(const Vector& colVec)
	{
		Vector result(m_height);
		for (unsigned int row = 0; row < m_height; row++)
			for (unsigned int col = 0; col < m_width; col++)
			{
				result[row] += get(row, col) * colVec.get(col);
			}
		return result;
	}

	Matrix operator*(const Matrix& matrix) const
	{
		//Matrix multiplication is not compatible
		ASSERT(matrix.height() == m_width);

		Matrix result = Matrix::Zero(m_height, matrix.width());

		for (unsigned int i = 0; i < m_height; i++) //row
		{
			for (unsigned int j = 0; j < m_width; j++)//col
			{
				//Loops through second matrix column and puts in result col
				for (unsigned int d = 0; d < matrix.height(); d++)
					result(i, d) += get(i, j) * matrix.get(j, d);
			}
		}
		return result;
	}

	//Will multiply all elements by $scalar
	Matrix operator*(const float& scalar)
	{
		Matrix result(m_width, m_height);
		for (unsigned int i = 0; i < m_size; i++)
			* result[i] = get(i) * scalar;
		return result;
	}

	Matrix operator+(const Matrix& matrix) const
	{
		ASSERT((*this) == matrix);

		Matrix result(m_height, m_width);
		for (unsigned int i = 0; i < m_size; i++)
			* result[i] = get(i) + matrix.get(i);
		return result;
	}

	Matrix operator-(const Matrix& matrix) const
	{
		ASSERT((*this) == matrix);

		Matrix result(m_height, m_width);
		for (unsigned int i = 0; i < m_size; i++)
			* result[i] = get(i) - matrix.get(i);
		return result;
	}

	//Returns true if the matrices are the same dimensions
	bool operator==(const Matrix& matrix) const
	{
		return (m_height == matrix.height() && m_width == matrix.width());
	}

	inline float& operator()(unsigned int row, unsigned int col) { return m_data[row * m_width + col]; }

	//Returns the element at $index; treating the matrix as a 1D array
	inline float* operator[](unsigned int index) { return m_data + index; }
	const float& get(unsigned int row, unsigned int col) const { return m_data[row * m_width + col]; }
	const float& get(unsigned int index) const { return m_data[index]; }

	unsigned int size() const { return m_size; }
	unsigned int height() const { return m_height; }
	unsigned int width() const { return m_width; }



	operator std::string()
	{
		return str();
	}

	std::string str() const
	{
		std::string result;
		for (unsigned int i = 0; i < m_height; i++)
		{
			for (unsigned int j = 0; j < m_width; j++)
			{
				result += std::to_string(get(i, j));
				if (j < m_height - 1)
					result += ", ";
			}
			result += "\n";
		}
		return result;
	}

	std::string strInline()
	{
		std::string result;
		for (unsigned int i = 0; i < m_size; i++)
		{
			result += std::to_string(get(i));
			if (i < m_size - 1)
				result += ", ";
		}
		return result;
	}

	//Return a copy of the matrix with the columns and rows swapped
	Matrix Transpose() const
	{
		Matrix result(m_width, m_height);
		for (unsigned int i = 0; i < m_height; i++)
			for (unsigned int j = 0; j < m_width; j++)
				result(j, i) = get(i, j);
		return result;
	}



private:
	//row, col //Goes down then right
	float* m_data;
	unsigned int m_width, m_height, m_size;

};

typedef Matrix4 mat4;
using mat = Matrix;