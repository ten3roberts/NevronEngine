#include <src/pch.h>
#include "Vector.h"
#include "..\src\Systemdefs.h"
#include "Math.h"
#include "..\src\Utility.h"
#include <iostream>

using namespace Utility;


Vector::Vector(unsigned int size) : m_size(size)
{
	m_data = new float[m_size];
	for (unsigned int i = 0; i < m_size; i++)
		m_data[i] = 0;
}
Vector::Vector(std::initializer_list<float> list) : m_size(list.size())
{
	m_data = new float[m_size];

	memcpy(m_data, list.begin(), m_size * sizeof(float));
}

Vector::Vector(unsigned int size, float* list) : m_size(size)
{
	m_data = new float[m_size];

	memcpy(m_data, list, m_size * sizeof(float));
}

Vector::Vector(std::vector<float> list) : m_size(list.size())
{
	m_data = new float[m_size];
	for (unsigned int i = 0; i < m_size; i++)
		m_data[i] = 0;
	memcpy(m_data, &list[0], m_size * sizeof(float));
}

Vector::Vector(const Vector& vec) : m_size(vec.size())
{
	m_data = new float[m_size];
	memcpy(m_data, vec.m_data, m_size * sizeof(float));
}

//Append constructor
Vector::Vector(const Vector& a, const Vector& b)
{
	m_size = a.size() + b.size();
	m_data = new float[m_size];
	memcpy(m_data, a.m_data, a.size() * sizeof(float));
	memcpy(m_data + a.size(), b.m_data, a.size() * sizeof(float));
}

Vector::Vector(const std::string& str) : m_data(0), m_size(0)
{
	//Removing spaces vector size if neccesary and splits into all the elements
	std::vector<std::string> parts = strSplit(strSplit(strPurge(str, " "), ";")[0], ",");
	m_data = new float[parts.size()];
	for (unsigned int i = 0; i < parts.size(); i++)
		(*this)[i] = num(parts[i]);
}

Vector Vector::Parse(const std::string& str)
{
	//Removing spaces vector size if neccesary and splits into all the elements
	std::vector<std::string> parts = strSplit(strSplit(strPurge(str, " "), ";")[0], ",");
	Vector result(parts.size());
	for (unsigned int i = 0; i < parts.size(); i++)
		result[i] = num(parts[i]);
	return result;
}

Vector Vector::OnSphere(unsigned int size, float radius)
{
	Vector result(size);
	for(unsigned int i = 0; i < size; i++)
		result[i] = (2.0f * std::rand() / RAND_MAX) - 1.0f;

	return result.Normalize() * size;
}

Vector Vector::InSphere(unsigned int size, float outerRadius, float innerRadius)
{
	//Generates a random unit vector
	Vector result(size);
	for (unsigned int i = 0; i < size; i++)
		result[i] = (2.0f * std::rand() / RAND_MAX) - 1.0f;

	result.Normalize(&result);

	//Scales it with a random length
	float randomLength = (float)std::rand() / RAND_MAX;
	result *= randomLength * (outerRadius - innerRadius) + innerRadius;

	return result;
}

Vector Vector::InSphereEven(unsigned int size, float outerRadius, float innerRadius)
{
	Vector result(size);
	for (unsigned int i = 0; i < size; i++)
		result[i] = (2.0f * std::rand() / RAND_MAX) - 1.0f;

	result.Normalize(&result);

	float randomLength = sqrt(((float)std::rand() / RAND_MAX) / MATH_PI); //Accounting sparser distrobution
	result *= randomLength * (outerRadius - innerRadius) + innerRadius;

	return result;
}

Vector Vector::InCube(unsigned int size, float width)
{
	Vector result(size);
	for (unsigned int i = 0; i < size; i++)
		result[i] = ((float)std::rand() / RAND_MAX) * width;

	return result;
}

Vector Vector::operator+(const Vector& vec) const
{
	ASSERT(m_size == vec.size());

	Vector result(m_size);
	for (unsigned int i = 0; i < m_size; i++)
		result[i] = m_data[i] + vec.get(i);
	return result;
}

Vector Vector::operator-(const Vector& vec) const
{
	ASSERT(m_size == vec.size());

	Vector result(m_size);
	for (unsigned int i = 0; i < m_size; i++)
		result[i] = m_data[i] - vec.get(i);
	return result;
}

Vector Vector::operator*(const Vector& vec) const
{
	ASSERT(m_size == vec.size());

	Vector result(m_size);
	for (unsigned int i = 0; i < m_size; i++)
		result[i] = m_data[i] * vec.get(i);
	return result;
}
Vector Vector::operator/(const Vector& vec) const
{
	ASSERT(m_size == vec.size());

	Vector result(m_size);
	for (unsigned int i = 0; i < m_size; i++)
		result[i] = m_data[i] + vec.get(i);
	return result;
}


Vector Vector::operator+(float scalar) const
{
	Vector result(m_size);
	for (unsigned int i = 0; i < m_size; i++)
		result[i] = m_data[i] + scalar;
	return result;
}

Vector Vector::operator-(float scalar) const
{
	Vector result(m_size);
	for (unsigned int i = 0; i < m_size; i++)
		result[i] = m_data[i] - scalar;
	return result;
}

Vector Vector::operator*(float scalar) const
{
	Vector result(m_size);
	for (unsigned int i = 0; i < m_size; i++)
		result[i] = m_data[i] * scalar;
	return result;
}

Vector Vector::operator/(float scalar) const
{
	Vector result(m_size);
	for (unsigned int i = 0; i < m_size; i++)
		result[i] = m_data[i] / scalar;
	return result;
}



//operator "?=":
void Vector::operator+=(const Vector& vec)
{
	ASSERT(m_size == vec.size());

	for (unsigned int i = 0; i < m_size; i++)
		m_data[i] += vec.get(i);
}

void Vector::operator-=(const Vector& vec)
{
	ASSERT(m_size == vec.size());

	for (unsigned int i = 0; i < m_size; i++)
		m_data[i] -= vec.get(i);
}


void Vector::operator*=(const Vector& vec)
{
	ASSERT(m_size == vec.size());

	for (unsigned int i = 0; i < m_size; i++)
		m_data[i] *= vec.get(i);
}

void Vector::operator/=(const Vector& vec)
{
	ASSERT(m_size == vec.size());

	for (unsigned int i = 0; i < m_size; i++)
		m_data[i] /= vec.get(i);
}

void Vector::operator+=(float scalar)
{
	for (unsigned int i = 0; i < m_size; i++)
		m_data[i] += scalar;
}

void Vector::operator-=(float scalar)
{
	for (unsigned int i = 0; i < m_size; i++)
		m_data[i] -= scalar;
}


void Vector::operator*=(float scalar)
{
	for (unsigned int i = 0; i < m_size; i++)
		m_data[i] *= scalar;
}

void Vector::operator/=(float scalar)
{
	for (unsigned int i = 0; i < m_size; i++)
		m_data[i] /= scalar;
}


float Vector::SqrMagnitude() const
{
	float result = 0;
	for (unsigned int i = 0; i < m_size; i++)
		result += m_data[i] * m_data[i];
	return result;
}


float Vector::Magnitude() const
{
	float result = 0;
	for (unsigned int i = 0; i < m_size; i++)
		result += m_data[i] * m_data[i];
	return sqrtf(result);
}


//Multiplies the components together
float Vector::Volume() const
{
	float result = 1;
	for (unsigned int i = 0; i < m_size; i++)
		result *= m_data[i];
	return result;
}

Vector Vector::ClampMag(float min, float max) const
{
	if (SqrMagnitude() > max * max)
	{
		return Normalize() * max;
	}

	else if (SqrMagnitude() < min * min)
	{
		return Normalize() * min;
	}
	return *this;
}

Vector Vector::ClampMinMag(float min) const
{
	if (SqrMagnitude() < min * min)
	{
		return Normalize() * min;
	}
	return *this;
}

Vector Vector::ClampMaxMag(float max) const
{
	if (SqrMagnitude() > max * max)
	{
		return Normalize() * max;
	}
	return *this;
}

Vector Vector::strClamp(float min, float max) const
{
	Vector result(m_size);
	for (unsigned int i = 0; i < m_size; i++)
		result[i] = m_data[i] > max ? max : m_data[i] < min ? min : m_data[i];
	return result;
}

Vector Vector::ClampMin(float min) const
{
	Vector result(m_size);
	for (unsigned int i = 0; i < m_size; i++)
		result[i] = m_data[i] < min ? min : m_data[i];
	return result;
}

Vector Vector::ClampMax(float max) const
{
	Vector result(m_size);

	for (unsigned int i = 0; i < m_size; i++)
		result[i] = m_data[i] > max ? max : m_data[i];
	return result;
}

float Vector::Dot(Vector a, Vector b)
{
	ASSERT(a.size() == b.size());

	float result = 0;
	for (unsigned int i = 0; i < a.size(); i++)
		result += a[i] * b[i];
	return result;
}

Vector Vector::Lerp(Vector a, Vector b, float t)
{
	ASSERT(a.size() == b.size());
	t = Math::Clamp01(t);
	Vector result(a.size());
	for (unsigned int i = 0; i < a.size(); i++)
		result[i] = a[i] * (1 - t) + b[i] * t;
	return result;
}

