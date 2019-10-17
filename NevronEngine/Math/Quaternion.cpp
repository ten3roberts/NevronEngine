#include <src/pch.h>
#include "Quaternion.h"
#include "Time.h"
#include "Math.h"
#include "Matrix.h"
const Quaternion Quaternion::identity = { 0,0,0,1 };

Quaternion::Quaternion() : x(0), y(0), z(0), w(1)
{
}

Quaternion::Quaternion(const Vector3& axis, float angle)
{
	float sinAngle = sinf(angle / 2);
	x = axis.x * sinAngle;
	y = axis.y * sinAngle;
	z = axis.z * sinAngle;
	w = cosf(angle / 2);// sqrt(1 - sinAngle * sinAngle); //Cosine of half angle
	*this = Normalize();
}

Quaternion::Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
{
}

Quaternion::Quaternion(const Vector3& euler)
{
	float Y = sinf(euler.y / 2);
	float X = sinf(euler.x / 2);
	float Z = sinf(euler.z / 2);

	float ey = cosf(euler.y / 2);
	float ex = cosf(euler.x / 2);
	float ez = cosf(euler.z / 2);

	x = +ey * X * ez + Y * ex * Z;
	y = -ey * X * Z + Y * ex * ez;
	z = -Y * X * ez + ey * ex * Z;
	w = +Y * X * Z + ey * ex * ez;

}

Quaternion::~Quaternion()
{
}

Quaternion Quaternion::Identity()
{
	return identity;
}

Quaternion Quaternion::Euler(const Vector3& euler)
{
	float Y = sinf(euler.y / 2);
	float X = sinf(euler.x / 2);
	float Z = sinf(euler.z / 2);

	float ey = cosf(euler.y / 2);
	float ex = cosf(euler.x / 2);
	float ez = cosf(euler.z / 2);
	return Quaternion(
		+ey * X * ez + Y * ex * Z,
		-ey * X * Z + Y * ex * ez,
		-Y * X * ez + ey * ex * Z,
		+Y * X * Z + ey * ex * ez);
}

Quaternion Quaternion::PointTo(const Vector3& direction)
{
	Vector3 forwardVector = direction.Normalize();

	float dot = Vector3::Dot(Vector3::forward, forwardVector);

	if (abs(dot - (-1.0f)) < 0.000001f)
	{
		return Quaternion(Vector3::up.x, Vector3::up.y, Vector3::up.z, 3.1415926535897932f);
	}
	if (abs(dot - (1.0f)) < 0.000001f)
	{
		return Quaternion::identity;
	}

	float rotAngle = (float)acos(dot);
	Vector3 rotAxis = Vector3::Cross(Vector3::forward, forwardVector);
	rotAxis = rotAxis.Normalize();
	return Quaternion(rotAxis, rotAngle);
}

Quaternion Quaternion::PointTo(const Vector3& sourcePoint, const Vector3& destinationPoint)
{
	Vector3 forwardVector = (destinationPoint - sourcePoint).Normalize();

	float dot = Vector3::Dot(Vector3::forward, forwardVector);

	if (abs(dot - (-1.0f)) < 0.000001f)
	{
		return Quaternion(Vector3::up.x, Vector3::up.y, Vector3::up.z, 3.1415926535897932f);
	}
	if (abs(dot - (1.0f)) < 0.000001f)
	{
		return Quaternion::identity;
	}

	float rotAngle = (float)acos(dot);
	Vector3 rotAxis = Vector3::Cross(Vector3::forward, forwardVector);
	rotAxis = rotAxis.Normalize();
	return Quaternion(rotAxis, rotAngle);
}


void Quaternion::operator=(Quaternion q)
{
	x = q.x;
	y = q.y;
	z = q.z;
	w = q.w;
}

Quaternion Quaternion::operator*(float scalar) const
{
	return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
	if (!q.Valid())
	{
		return Quaternion::identity;
	}

	Quaternion result(
		x * q.w + y * q.z - z * q.y + w * q.x,
		-x * q.z + y * q.w + z * q.x + w * q.y,
		x * q.y - y * q.x + z * q.w + w * q.z,
		-x * q.x - y * q.y - z * q.z + w * q.w
	);
	return result;
}

Quaternion Quaternion::operator+(const Quaternion& q) const
{
	return Quaternion(x + q.x,
		y + q.y,
		z + q.z,
		w + q.w);
}

Quaternion Quaternion::operator-(const Quaternion& q) const
{
	return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w);
}

Vector3 Quaternion::operator*(const Vector3& vec) const
{
	Vector3 t = Vector3::Cross(Vector3(x,y,z), vec) * 2.0f;
	return vec + t * w + Vector3::Cross(Vector3(x,y,z), t);
}

void Quaternion::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
}

void Quaternion::operator*=(const Quaternion& q)
{
	if (!q.Valid())
		return;
	*this = Quaternion(
		x * q.w + y * q.z - z * q.y + w * q.x,
		-x * q.z + y * q.w + z * q.x + w * q.y,
		x * q.y - y * q.x + z * q.w + w * q.z,
		-x * q.x - y * q.y - z * q.z + w * q.w);
}

Matrix4 Quaternion::toMatrix()
{
	return
	{
	1 - 2 * y * y - 2 * z * z,		2 * x * y - 2 * z * w,		2 * x * z + 2 * y * w, 0,
		2 * x * y + 2 * z * w,	1 - 2 * x * x - 2 * z * z,		2 * y * z - 2 * x * w, 0,
		2 * x * z - 2 * y * w,		2 * y * z + 2 * x * w,	1 - 2 * x * x - 2 * y * y, 0,
		0,							0,							0,					   1
	};
}

Vector3 Quaternion::toEuler()
{
	Vector3 result;
	// roll (x-axis rotation)
	double sinr_cosp = +2.0 * (w * x + y * z);
	double cosr_cosp = +1.0 - 2.0 * (x * x * y * y);
	result.roll = atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = +2.0 * (w* y - z * x);
	if (fabs(sinp) >= 1)
		result.pitch = copysign(MATH_PI / 2, sinp); // use 90 degrees if out of range
	else
		result.pitch = asin(sinp);

	// yaw (z-axis rotation)
	double siny_cosp = +2.0 * (w * z + x * y);
	double cosy_cosp = +1.0 - 2.0 * (y * y + z * z);
	result.yaw = atan2(siny_cosp, cosy_cosp);
	return result;
}

Quaternion Quaternion::Normalize() const
{
	if (!Valid())
		return *this;
	float magnitude = Magnitude();
	if (abs(magnitude - 1) < 0.001) //If its already Normalized
	{
		return *this;
	}

	Quaternion outQ;
	outQ.x = x / magnitude;
	outQ.y = y / magnitude;
	outQ.z = z / magnitude;
	outQ.w = w / magnitude;

	return outQ;
}

void Quaternion::Normalize(Quaternion* out)
{
	if (!out)
		return;

	if (!Valid())
		* out = Quaternion::identity;


	float magnitude = Magnitude();
	if (abs(magnitude - 1) < 0.001) //If its already Normalized
	{
		*out = *this;
	}

	out->x = x / magnitude;
	out->y = y / magnitude;
	out->z = z / magnitude;
	out->w = w / magnitude;
}

Quaternion Quaternion::Inverse()
{
	Quaternion outQ;
	outQ.x = x;
	outQ.y = y;
	outQ.z = z;
	outQ.w = -w;

	return outQ;
}

void Quaternion::Inverse(Quaternion* out)
{
	if (!out)
		return;
	out->x = x;
	out->y = y;
	out->z = z;
	out->w = -w;
}

float Quaternion::SqrMagnitude() const
{
	return x * x + y * y + z * z + w * w;

}

float Quaternion::Magnitude() const
{
	return sqrtf(x * x + y * y + z * z + w * w);

}

Quaternion Quaternion::Rotate(float angle)
{
	if (!Valid())
		return Quaternion::identity;

	Quaternion out;
	float oldHalfSin = sqrtf(1 - w * w);
	float halfSin = sinf(angle / 2);
	out.x = x / oldHalfSin * halfSin;
	out.y = y / oldHalfSin * halfSin;
	out.z = z / oldHalfSin * halfSin;
	out.w = cosf(angle / 2);
	return out;
}

Quaternion Quaternion::ScaleRotate(float scale)
{
	if (!Valid())
		return Quaternion::identity;

	float angle = acosf(w) * scale;
	Quaternion out;
	float oldHalfSin = sqrtf(1 - w * w);
	float halfSin = sinf(angle / 2);
	out.x = x / oldHalfSin * halfSin;
	out.y = y / oldHalfSin * halfSin;
	out.z = z / oldHalfSin * halfSin;
	out.w = cosf(angle / 2);
	return out;
}

float Quaternion::Dot(const Quaternion& q0, const Quaternion& q1)
{
	return q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
}

Quaternion Quaternion::Slerp(const Quaternion& a, const Quaternion& b, float t)
{
	// Only unit quaternions are valid rotations.
	// Normalize to avoid undefined behavior.
	Quaternion q0 = a.Normalize();
	Quaternion q1 = b.Normalize();

	// get the dot product between the two quaternions
	float dot = Dot(q0, q1);

	// Makes it take the shortest path
	if (dot < 0)
	{
		q1 = q1 * -1;
		dot = -dot;
	}

	const float CLOSEST_DOT = 0.9995f;
	if (dot > CLOSEST_DOT)
	{
		// If the inputs are too close, linearly interpolate
		// and Normalize the result.

		Quaternion result = q0 + (q1 - q0) * t;
		result = result.Normalize();
		return result;
	}

	// acos is safe because the cos between q0, q1 is less than closetsDot
	float angle = acosf(dot);        // theta_0 = angle between input vectors
	float partAngle = angle * t;          // theta = angle between v0 and result
	float sinPart = sinf(partAngle);     // compute this value only once sinTheta
	float sinAngle = sqrtf(1 - dot * dot); // compute this value only once sintheta0

	float s0 = cosf(partAngle) - dot * sinPart / sinAngle;  // == sin(theta_0 - theta) / sin(theta_0)
	float s1 = sinPart / sinAngle;

	return (q0 * s0) + (q1 * s1);
}

Quaternion Quaternion::Lerp(const Quaternion& a, const Quaternion& b, float t)
{
	// If the inputs are too close, linearly interpolate
	// and Normalize the result.
	t = Math::Clamp01(t);
	Quaternion result = a + (b - a) * t;
	result = result.Normalize();
	return result;
}


