#include <src/pch.h>
#include "Vector2.h"
#include "..\src\Systemdefs.h"
#include "Math.h"
#include "..\src\Utility.h"
#include <math.h>
#include <sstream>

using namespace Utility;

const Vector2 Vector2::zero = { 0, 0 };
const Vector2 Vector2::one = { 1, 1 };

Vector2::Vector2() :
	x(0), y(0)
{
}

Vector2::Vector2(float  x, float  y) :
	x(x), y(y)
{
}

Vector2::~Vector2()
{
}

Vector2::Vector2(const std::string& str) : x(0), y(0)
{
	//Removing spaces vector size if neccesary and splits into all the elements
	std::vector<std::string> parts = strSplit(strSplit(str, ";")[0], " ");
	for (unsigned int i = 0; i < min(2, parts.size()); i++)
		(*this)[i] = num(parts[i]);
}

Vector2 Vector2::Parse(const std::string& str)
{
	//Removing spaces vector size if neccesary and splits into all the elements
	std::vector<std::string> parts = strSplit(strSplit(str, ";")[0], " ");
	Vector2 result;
	for (unsigned int i = 0; i < min(2, parts.size()); i++)
		result[i] = num(parts[i]);
	return result;
}

void Vector2::operator=(const Vector2 & vec2)
{
	x = vec2.x;
	y = vec2.y;
}

Vector2 Vector2::operator+(const Vector2 & vec2) const
{
	return Vector2(x + vec2.x, y + vec2.y);
}

Vector2 Vector2::operator-(const Vector2 & vec2) const
{
	return Vector2(x - vec2.x, y - vec2.y);
}

Vector2 Vector2::operator*(const Vector2 & vec2) const
{
	return Vector2(x * vec2.x, y * vec2.y);
}

Vector2 Vector2::operator/(const Vector2 & vec2) const
{
	return Vector2(x / vec2.x, y / vec2.y);
}

Vector2 Vector2::operator+(float scalar) const
{
	return Vector2(x + scalar, y + scalar);
}

Vector2 Vector2::operator-(float scalar) const
{
	return Vector2(x - scalar, y - scalar);
}

Vector2 Vector2::operator*(float scalar) const
{
	return Vector2(x*scalar, y*scalar);
}

Vector2 Vector2::operator/(float scalar) const
{
	return Vector2(x / scalar, y / scalar);
}

//operator "?=":
void Vector2::operator+=(const Vector2 & vec2)
{
	x += vec2.x;
	y += vec2.y;
}

void Vector2::operator-=(const Vector2 & vec2)
{
	x -= vec2.x;
	y -= vec2.y;
}

void Vector2::operator*=(const Vector2 & vec2)
{
	x *= vec2.x;
	y *= vec2.y;
}

void Vector2::operator/=(const Vector2 & vec2)
{
	x /= vec2.x;
	y /= vec2.y;
}

void Vector2::operator+=(float scalar)
{
	x += scalar;
	y += scalar;
}

void Vector2::operator-=(float scalar)
{
	x -= scalar;
	y -= scalar;
}

void Vector2::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
}

void Vector2::operator/=(float scalar)
{
	x /= scalar;
	y /= scalar;
}

float Vector2::SqrMagnitude() const
{
	return x*x + y*y;
}

float Vector2::Magnitude() const
{
	return sqrt(x*x + y*y);
}

Vector2 Vector2::Normalize() const
{
	if (SqrMagnitude() < 0.000001f)
		return Vector2(0, 0);
	float mag = Magnitude();

	return Vector2(x / mag, y / mag);
}

void Vector2::Normalize(Vector2* out)
{
	if (SqrMagnitude() < 0.000001f)
		return;
	float mag = Magnitude();

	Vector2 outVec(x / mag, y / mag);
	if (out)
		* out = outVec;
	return;

}

Vector2 Vector2::ClampMag(float min, float max) const
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

Vector2 Vector2::ClampMinMag(float min) const
{
	if (SqrMagnitude() < min * min)
	{
		return Normalize() * min;
	}
	return *this;
}

Vector2 Vector2::ClampMaxMag(float max) const
{
	if (SqrMagnitude() > max * max)
	{
		return Normalize() * max;
	}
	return *this;
}

Vector2 Vector2::strClamp(float min, float max) const
{
	Vector2 result;
	result.x = x > max ? max : x < min ? min : x;
	result.y = y > max ? max : y < min ? min : y;
	return result;
}

Vector2 Vector2::ClampMin(float min) const
{
	Vector2 result;
	result.x = x < min ? min : x;
	result.y = y < min ? min : y;
	return result;
}

Vector2 Vector2::ClampMax(float max) const
{
	Vector2 result;
	result.x = x > max ? max : x;
	result.y = y > max ? max : y;
	return result;
}



float Vector2::Dot(const Vector2& a, const Vector2& b)
{
	return a.x* b.x + a.y * b.y;
}

Vector2 Vector2::Project(const Vector2& vector, const Vector2& axis)
{
	if (!vector.Valid())
		return Vector2();
	return Vector2(axis * (Dot(vector, axis)) / axis.SqrMagnitude());
}

float Vector2::ProjectFlat(const Vector2& vector, const Vector2& axis)
{
	return (Dot(vector.Normalize(), axis.Normalize()) * vector.Magnitude());
}

Vector2 Vector2::Lerp(const Vector2& a, const Vector2& b, float t)
{
	t = Math::Clamp01(t);
	return Vector2((a * (1 - t) + b * t));
}

