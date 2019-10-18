#include <src/pch.h>
#include "Vector4.h"
#include "Vector3.h"
#include "Math.h"
#include <math.h>
#include <sstream>

using namespace Utility;

const Vector4 Vector4::zero = { 0,0,0,0 };
const Vector4 Vector4::one = { 1,1,1,1 };
const Vector4 Vector4::red = { 1,0,0,1 };
const Vector4 Vector4::green = { 0,1,0,1 };
const Vector4 Vector4::blue = { 0,0,1,1 };
const Vector4 Vector4::white = { 1,1,1,1 };
const Vector4 Vector4::black = { 0,0,0,0 };

Vector4::Vector4() :
	x(0), y(0), z(0), w(0)
{
}

Vector4::Vector4(float  x, float  y, float  z, float  w) :
	x(x), y(y), z(z), w(w)
{
}

Vector4::Vector4(Vector3 vec3, float w) :
	x(vec3.x), y(vec3.y), z(vec3.z), w(w)
{
}

Vector4::~Vector4()
{
}

Vector4::Vector4(const std::string& str)
{
	//Removing spaces vector size if neccesary and splits into all the elements
	std::vector<std::string> parts = strSplit(strSplit(str, ";")[0], " ");
	for (unsigned int i = 0; i < min(4, parts.size()); i++)
		(*this)[i] = numf(parts[i]);
}

Vector4 Vector4::Parse(const std::string& str)
{
	//Removing spaces vector size if neccesary and splits into all the elements
	std::vector<std::string> parts = strSplit(strSplit(str, ";")[0], " ");
	Vector4 result;
	for (unsigned int i = 0; i < min(4, parts.size()); i++)
		result[i] = numf(parts[i]);
	return result;
}

void Vector4::operator=(const Vector4& vec4)
{
	x = vec4.x;
	y = vec4.y;
	z = vec4.z;
	w = vec4.w;
}

Vector4 Vector4::operator+(const Vector4& vec4) const
{
	return Vector4(x + vec4.x, y + vec4.y, z + vec4.z, w + vec4.w);
}

Vector4 Vector4::operator-(const Vector4& vec4) const
{
	return Vector4(x - vec4.x, y - vec4.y, z - vec4.z, w - vec4.w);
}

Vector4 Vector4::operator*(const Vector4& vec4) const
{
	return Vector4(x * vec4.x, y * vec4.y, z * vec4.z, w * vec4.w);
}

Vector4 Vector4::operator/(const Vector4& vec4) const
{
	return Vector4(x / vec4.x, y / vec4.y, z / vec4.z, w / vec4.w);
}

Vector4 Vector4::operator+(float scalar) const
{
	return Vector4(x + scalar, y + scalar, z + scalar, w + scalar);
}

Vector4 Vector4::operator-(float scalar) const
{
	return Vector4(x - scalar, y - scalar, z - scalar, w - scalar);
}

Vector4 Vector4::operator*(float scalar) const
{
	return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
}

Vector4 Vector4::operator/(float scalar) const
{
	return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
}

void Vector4::operator+=(const Vector4& vec4)
{
	x += vec4.x;
	y += vec4.y;
	z += vec4.z;
	w += vec4.w;
}

void Vector4::operator-=(const Vector4& vec4)
{
	x -= vec4.x;
	y -= vec4.y;
	z -= vec4.z;
	w -= vec4.w;
}

void Vector4::operator*=(const Vector4& vec4)
{
	x *= vec4.x;
	y *= vec4.y;
	z *= vec4.z;
	w *= vec4.w;
}

void Vector4::operator/=(const Vector4& vec4)
{
	x /= vec4.x;
	y /= vec4.y;
	z /= vec4.z;
	w /= vec4.w;
}

void Vector4::operator+=(float scalar)
{
	x += scalar;
	y += scalar;
	z += scalar;
	w += scalar;
}

void Vector4::operator-=(float scalar)
{
	x -= scalar;
	y -= scalar;
	z -= scalar;
	w -= scalar;
}

void Vector4::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
}

void Vector4::operator/=(float scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;
}

float Vector4::SqrMagnitude() const
{
	return x * x + y * y + z * z + w * w;
}

float Vector4::Magnitude() const
{
	return sqrt(x * x + y * y + z * z + w * w);
}

Vector4 Vector4::Normalize() const
{
	if (SqrMagnitude() < 0.000001f)
		return Vector4(0, 0, 0, 0);
	float mag = Magnitude();

	return Vector4(x / mag, y / mag, z / mag, w / mag);
}

void Vector4::Normalize(Vector4* out)
{
	if (SqrMagnitude() < 0.000001f)
		return;
	float mag = Magnitude();

	Vector4 outVec(x / mag, y / mag, z / mag, w / mag);
	if (out)
		* out = outVec;

	return;

}

Vector4 Vector4::ClampMag(float min, float max) const
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

Vector4 Vector4::ClampMinMag(float min) const
{
	if (SqrMagnitude() < min * min)
	{
		return Normalize() * min;
	}
	return *this;
}

Vector4 Vector4::ClampMaxMag(float max) const
{
	if (SqrMagnitude() > max * max)
	{
		return Normalize() * max;
	}
	return *this;
}

Vector4 Vector4::strClamp(float min, float max) const
{
	Vector4 result;
	result.x = x > max ? max : x < min ? min : x;
	result.y = y > max ? max : y < min ? min : y;
	result.z = z > max ? max : z < min ? min : z;
	result.w = w > max ? max : w < min ? min : w;
	return result;
}

Vector4 Vector4::ClampMin(float min) const
{
	Vector4 result;
	result.x = x < min ? min : x;
	result.y = y < min ? min : y;
	result.z = z < min ? min : z;
	result.w = w < min ? min : w;
	return result;
}

Vector4 Vector4::ClampMax(float max) const
{
	Vector4 result;
	result.x = x > max ? max : x;
	result.y = y > max ? max : y;
	result.z = z > max ? max : z;
	result.w = w > max ? max : w;
	return result;
}


float Vector4::Dot(const Vector4& a, const Vector4& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Vector4 Vector4::Project(const Vector4& vector, const Vector4& axis)
{
	if (!vector.Valid())
		return Vector4();
	return Vector4(axis * (Dot(vector, axis)) / axis.SqrMagnitude());
}

float Vector4::ProjectFlat(const Vector4& vector, const Vector4& axis)
{
	return (Dot(vector.Normalize(), axis.Normalize()) * vector.Magnitude());
}

Vector4 Vector4::Lerp(const Vector4& a, const Vector4& b, float t)
{
	t = Math::Clamp01(t);
	return Vector4((a * (1 - t) + b * t));
}



