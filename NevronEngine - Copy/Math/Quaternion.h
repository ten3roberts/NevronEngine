#pragma once
#include "Vector.h"

struct Matrix4;

struct Quaternion
{
public:
	Quaternion();
	Quaternion(const Vector3& axis, float angle); //Axis angle rotations
	Quaternion(float x, float y, float z, float w);
	explicit Quaternion(const Vector3& euler);

	~Quaternion();
	static Quaternion Identity();
	//Creates a quaternion from an euler rotation
	static Quaternion Euler(const Vector3& euler);

	//Will create a new quaternion pointing along to specified direction. Direction does not need to be Normalized
	static Quaternion PointTo(const Vector3& direction);
	//Will create a new quaternion to rotate from its current position to %direction%
	static Quaternion PointTo(const Vector3& sourcePoint, const Vector3& destinationPoint);

	union
	{
		struct
		{
			float x, y, z, w;
		};
		struct
		{
			Vector3 vector;
			float w;
		};
		struct
		{
			Vector3 xyz;
			float w;
		};
	};

	operator std::string()
	{
		return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w);
	}

	void operator=(Quaternion q);

	Quaternion operator*(float scalar) const;
	Quaternion operator*(const Quaternion& q) const;

	Quaternion operator+ (const Quaternion& q) const;
	Quaternion operator- (const Quaternion& q) const;

	//Vector transformation
	Vector3 operator*(const Vector3& vec) const;

	void operator*=(float scalar);
	void operator*=(const Quaternion& q);

	Matrix4 toMatrix();
	Vector3 toEuler();

	Quaternion Normalize() const;
	void Normalize(Quaternion* out);

	Quaternion Inverse();
	void Inverse(Quaternion* out);

	float SqrMagnitude() const;
	float Magnitude() const;

	//Will rotate the quaternion angle radians
	Quaternion Rotate(float angle);
	Quaternion ScaleRotate(float scale);

	//Returns false if the vector doesnt have a direction or length
	inline bool Valid() const { return SqrMagnitude() > 0.00001; }
	inline bool Defined() const { return SqrMagnitude() > 0.00001; }

	inline std::string str() const { return std::string(STR(x) + ", " + STR(y) + ", " + STR(z) + ", " + STR(w)); }

	//Quaternion should always be a unit quaternion; only use for debugging purposes
	inline std::string str_d() const { return std::string(STR(x) + ", " + STR(y) + ", " + STR(z) + ", " + STR(w) + "; " + STR(Magnitude())); }


	static const Quaternion identity;
	static float Dot(const Quaternion& q0, const Quaternion& q1);
	static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t);
	static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t);
};

typedef Quaternion quat;



