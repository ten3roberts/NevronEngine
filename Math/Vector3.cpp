#include <src/pch.h>
#include "Vector3.h"
#include <src/Time.h>
#include <Math/Math.h>
#include "Quaternion.h"
#include <src/Utility.h>

#include <cmath>
#include <iostream>
#include <sstream>
#include <random>

using namespace Utility;

const Vector3 Vector3::red = { 1,0,0 };
const Vector3 Vector3::green = { 0,1,0 };
const Vector3 Vector3::blue = { 0,0,1 };
const Vector3 Vector3::white = { 1,1,1 };
const Vector3 Vector3::black = { 0,0,0 };


const Vector3 Vector3::forward = { 0,0,1 };
const Vector3 Vector3::back = { 0,0,-1 };
const Vector3 Vector3::up = { 0,1,0 };
const Vector3 Vector3::down = { 0,-1,0 };
const Vector3 Vector3::right = { 1,0,0 };
const Vector3 Vector3::left = { -1,0,0 };
const Vector3 Vector3::zero = { 0,0,0 };
const Vector3 Vector3::one = { 1, 1, 1 };



Vector3::Vector3() :
	x(0), y(0), z(0)
{
}

Vector3::Vector3(float x, float y, float z) :
	x(x), y(y), z(z)
{
}

Vector3::Vector3(float setTo) : x(setTo), y(setTo), z(setTo)
{
}

Vector3::Vector3(const std::string& str) : x(0), y(0), z(0)
{
	//Removing spaces vector size if neccesary and splits into all the elements
	std::vector<std::string> parts = strSplit(strSplit(str, ";")[0], " ");
	for (unsigned int i = 0; i < min(3, parts.size()); i++)
		(*this)[i] = numf(parts[i]);
}

Vector3 Vector3::Parse(const std::string& str)
{
	//Removing spaces vector size if neccesary and splits into all the elements
	std::vector<std::string> parts = strSplit(strSplit(str, ";")[0], " ");
	Vector3 result;
	for (unsigned int i = 0; i < min(3, parts.size()); i++)
		result[i] = numf(parts[i]);
	return result;
}

Vector3 Vector3::HSV(float h, float s, float v)
{
	Vector3 out;

	h = fmod(h, 1.0f);
	float hh, p, q, t, ff;
	long i;

	if (s <= 0.0f) {       // < is bogus, just shuts up warnings
		out.r = v;
		out.g = v;
		out.b = v;
		return out;
	}
	hh = h;
	if (hh >= 1) hh = 0.0f;
	hh /= 1 / 6.0f;
	i = (long)hh;
	ff = hh - i;
	p = v * (1.0f - s);
	q = v * (1.0f - (s * ff));
	t = v * (1.0f - (s * (1.0f - ff)));

	switch (i) {
	case 0:
		out.r = v;
		out.g = t;
		out.b = p;
		break;
	case 1:
		out.r = q;
		out.g = v;
		out.b = p;
		break;
	case 2:
		out.r = p;
		out.g = v;
		out.b = t;
		break;

	case 3:
		out.r = p;
		out.g = q;
		out.b = v;
		break;
	case 4:
		out.r = t;
		out.g = p;
		out.b = v;
		break;
	case 5:
	default:
		out.r = v;
		out.g = p;
		out.b = q;
		break;
	}
	return out;
}

Vector3 Vector3::OnSphere(float radius)
{
	return Random::OnSphere(radius);
}

Vector3 Vector3::InSphere(float outerRadius, float innerRadius)
{
	return Random::InSphere(outerRadius, innerRadius);
}

Vector3 Vector3::InSphereEven(float outerRadius, float innerRadius)
{
	return Random::InSphereEven(outerRadius, innerRadius);
}

Vector3 Vector3::InCube(float width)
{
	return Random::InCube(width);
}

Vector3 Vector3::HSV(Vector3 hsv)
{
	Vector3 out;

	hsv.h = fmod(hsv.h, 1.0f);
	float hh, p, q, t, ff;
	long i;

	if (hsv.s <= 0.0f) {       // < is bogus, just shuts up warnings
		out.r = hsv.v;
		out.g = hsv.v;
		out.b = hsv.v;
		return out;
	}
	hh = hsv.h;
	if (hh >= 1) hh = 0.0f;
	hh /= 1 / 6.0f;
	i = (long)hh;
	ff = hh - i;
	p = hsv.v * (1.0f - hsv.s);
	q = hsv.v * (1.0f - (hsv.s * ff));
	t = hsv.v * (1.0f - (hsv.s * (1.0f - ff)));

	switch (i) {
	case 0:
		out.r = hsv.v;
		out.g = t;
		out.b = p;
		break;
	case 1:
		out.r = q;
		out.g = hsv.v;
		out.b = p;
		break;
	case 2:
		out.r = p;
		out.g = hsv.v;
		out.b = t;
		break;

	case 3:
		out.r = p;
		out.g = q;
		out.b = hsv.v;
		break;
	case 4:
		out.r = t;
		out.g = p;
		out.b = hsv.v;
		break;
	case 5:
	default:
		out.r = hsv.v;
		out.g = p;
		out.b = q;
		break;
	}
	return out;
}

Vector3::~Vector3()
{
}

/*Vector3 Vector3::ToRotation()
{
	return Vector3(cos(y)*cos(x), sin(x), sin(y)*cos(x));
}*/

void Vector3::operator=(const Vector3& vec3)
{
	x = vec3.x;
	y = vec3.y;
	z = vec3.z;
}

Vector3 Vector3::operator+(const Vector3& vec3) const
{
	return Vector3(x + vec3.x, y + vec3.y, z + vec3.z);
}

Vector3 Vector3::operator-(const Vector3& vec3) const
{
	return Vector3(x - vec3.x, y - vec3.y, z - vec3.z);
}

Vector3 Vector3::operator*(const Vector3& vec3) const
{
	return Vector3(x * vec3.x, y * vec3.y, z * vec3.z);
}

Vector3 Vector3::operator/(const Vector3& vec3) const
{
	return Vector3(x / vec3.x, y / vec3.y, z / vec3.z);
}

Vector3 Vector3::operator+(float scalar) const
{
	return Vector3(x + scalar, y + scalar, z + scalar);
}

Vector3 Vector3::operator-(float scalar) const
{
	return Vector3(x - scalar, y - scalar, z - scalar);
}

Vector3 Vector3::operator*(float scalar) const
{
	return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::operator/(float scalar) const
{
	return Vector3(x / scalar, y / scalar, z / scalar);
}

//operator "?=":

void Vector3::operator+=(const Vector3& vec3)
{
	x += vec3.x;
	y += vec3.y;
	z += vec3.z;
}

void Vector3::operator-=(const Vector3& vec3)
{
	x -= vec3.x;
	y -= vec3.y;
	z -= vec3.z;
}

void Vector3::operator*=(const Vector3& vec3)
{
	x *= vec3.x;
	y *= vec3.y;
	z *= vec3.z;

}

void Vector3::operator/=(const Vector3& vec3)
{
	x /= vec3.x;
	y /= vec3.y;
	z /= vec3.z;
}

void Vector3::operator+=(float scalar)
{
	x += scalar;
	y += scalar;
	z += scalar;
}

void Vector3::operator-=(float scalar)
{
	x -= scalar;
	y -= scalar;
	z -= scalar;
}

void Vector3::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
}

void Vector3::operator/=(float scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
}

float Vector3::SqrMagnitude() const
{
	return x * x + y * y + z * z;
}

float Vector3::Magnitude() const
{
	return sqrt(x * x + y * y + z * z);
}

void Vector3::Normalize(Vector3* out)
{
	if (SqrMagnitude() < 0.000001f)
		return;
	float mag = Magnitude();

	Vector3 outVec(x / mag, y / mag, z / mag);
	if (out)
		* out = outVec;
	return;

}

Vector3 Vector3::Normalize() const
{
	if (SqrMagnitude() < 0.000001f)
		return Vector3(0, 0, 0);
	float mag = Magnitude();

	return Vector3(x / mag, y / mag, z / mag);
}

Vector3 Vector3::ClampMag(float min, float max) const
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

Vector3 Vector3::ClampMinMag(float min) const
{
	if (SqrMagnitude() < min * min)
	{
		return Normalize() * min;
	}
	return *this;
}

Vector3 Vector3::ClampMaxMag(float max) const
{
	if (SqrMagnitude() > max * max)
	{
		return Normalize() * max;
	}
	return *this;
}

Vector3 Vector3::strClamp(float min, float max) const
{
	Vector3 result;
	result.x = x > max ? max : x < min ? min : x;
	result.y = y > max ? max : y < min ? min : y;
	result.z = z > max ? max : z < min ? min : z;
	return result;
}

Vector3 Vector3::ClampMin(float min) const
{
	Vector3 result;
	result.x = x < min ? min : x;
	result.y = y < min ? min : y;
	result.z = z < min ? min : z;
	return result;
}

Vector3 Vector3::ClampMax(float max) const
{
	Vector3 result;
	result.x = x > max ? max : x;
	result.y = y > max ? max : y;
	result.z = z > max ? max : z;
	return result;
}


float Vector3::Dot(Vector3 a, Vector3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Vector3::Project(const Vector3& vector, const Vector3& axis)
{
	if (!vector.Valid())
		return Vector3();
	return Vector3(axis * (Dot(vector, axis)) / axis.SqrMagnitude());
}

float Vector3::ProjectFlat(const Vector3& vector, const Vector3& axis)
{
	return (Dot(vector.Normalize(), axis.Normalize()) * vector.Magnitude());
}

Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)
{
	return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

Vector3 Vector3::Reflect(const Vector3& ray, const Vector3& normal)
{
	Vector3 n = normal.Normalize();
	return Vector3(ray - n * 2 * Dot(ray, n));
}

Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, float t)
{
	t = Math::Clamp01(t);
	return Vector3((a * (1 - t) + b * t));
}

Vector3 Vector3::Slerp(const Vector3& a, const Vector3& b, float t)
{
	t = Math::Clamp01(t);
	// get the axis of rotation between from and to
	Vector3 axis = Cross(b, a);
	axis.Normalize(&axis);

	// get the angle to rotate around the axis 
	// NOTE: from and to must be of unit length!!!
	float angleRads = acosf(Dot(b, a));

	// Build a quaternion to rotate between 'from' and 'to'
	// NOTE: interval must be between 0 and 1!!
	Quaternion rot(axis, angleRads * t);

	Vector3 result = rot * b;

	return result;
}


