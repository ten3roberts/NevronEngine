#pragma once
#include "..\src\Systemdefs.h"
#include <string>

//x : right, red channel; y : up, green channel; z : forward, blue channel
struct Vector3
{
	Vector3();
	Vector3(float x, float y, float z);
	Vector3(float setTo);

	//inline static Vector3 Random(float min, float max);
	//Will produce a vector with a random direction and a magnitude between min max(plots to sphere)
	//static Vector3 RandomDirection(float min, float max);
	static Vector3 HSV(Vector3 hsv);
	static Vector3 HSV(float h, float s, float v);
	static Vector3 OnSphere(float radius = 1);
	static Vector3 InSphere(float outerRadius, float innerRadius = 0);
	static Vector3 InSphereEven(float outerRadius, float innerRadius = 0);
	static Vector3 InCube(float width);

	~Vector3();
	union
	{
		struct
		{
			float x, y, z;
		};
		struct
		{
			float r, g, b;
		};
		struct
		{
			float pitch, yaw, roll;
		};
		struct
		{
			float h, s, v;
		};
	};

#pragma region "Operators"
	operator std::string()
	{
		return str();
	}

	inline float* operator[](unsigned int index) { return &x + index; }
	//Returns a costant reference and is therefore safe with cost&
	const float& Get(unsigned int index) const { return *(&x + index); }

	void operator=(const Vector3& vec3);
	void operator=(float setTo) { x = setTo; y = setTo; z = setTo; }

	Vector3 operator+(const Vector3& vec3) const;
	Vector3 operator-(const Vector3& vec3) const;

	Vector3 operator*(const Vector3& vec3) const;
	Vector3 operator/(const Vector3& vec3) const;



	Vector3 operator+(float scalar) const;
	Vector3 operator-(float scalar) const;

	Vector3 operator*(float scalar) const;
	Vector3 operator/(float scalar) const;




	//operator "?=":
	void operator+=(const Vector3& vec3);
	void operator-=(const Vector3& vec3);

	void operator*=(const Vector3& vec3);
	void operator/=(const Vector3& vec3);

	void operator+=(float scalar);
	void operator-=(float scalar);

	void operator*=(float scalar);
	void operator/=(float scalar);

#pragma endregion

	float SqrMagnitude() const;
	float Magnitude() const;
	inline float Volume() const { return x * y* z; } //Multiplies the components together

	//Returns the normalized version of the vector; does not modify the vector
	Vector3 Normalize() const;
	//Normalizes the vector and assigns it to $out. Out can be the same as self
	void Normalize(Vector3* out);

	//Will clamp the vector's magnitude between a minimum and maximum value
	Vector3 ClampMag(float min, float max) const;
	//Will clamp the vector's magnitude to a minimum value
	Vector3 ClampMinMag(float min) const;
	//Will clamp the vector's magnitude to a maximum value
	Vector3 ClampMaxMag(float max) const;

	//Will clamp all the vectors components individually between a minimum and maximum value;
	Vector3 Clamp(float min, float max) const;
	//Will clamp all the vectors components individually to a minimum;
	Vector3 ClampMin(float min) const;
	//Will clamp all the vectors components individually to a maximum value;
	Vector3 ClampMax(float max) const;

	//Returns false if the vector doesnt have a direction or length
	inline bool Valid() const { return SqrMagnitude() > 0.00001; }
	inline bool Defined() const { return SqrMagnitude() > 0.00001; }


	inline float* Array() { return &x; }
	inline std::string str() { return std::string(STR(x) + ", " + STR(y) + ", " + STR(z)); }
	inline std::string str_d() { return std::string(STR(x) + ", " + STR(y) + ", " + STR(z) + "; " + STR(Magnitude())); }

	static const Vector3 red;
	static const Vector3 green;
	static const Vector3 blue;
	static const Vector3 black;

	static const Vector3 forward;
	static const Vector3 up;
	static const Vector3 right;
	static const Vector3 left;
	static const Vector3 zero;

	//Calculates the dot product between two vectors
	static float Dot(const Vector3 a, Vector3 b);

	//Returns a vector projected onto an axis
	static Vector3 Project(const Vector3& vector, const Vector3& axis);
	//Returns the length of a vector projected onto an axis
	static float ProjectFlat(const Vector3& vector, const Vector3& axis);

	//Returns a vector perpendicular to a and b
	static Vector3 Cross(const Vector3& a, const Vector3& b);

	//Returns the vector when the ray has been reflected from the normal
	static Vector3 Reflect(const Vector3& ray, const Vector3& normal);

	//Linearly interpolates a and b between t
	static Vector3 Lerp(const Vector3& a, const Vector3& b, float t);

	//Spherically interpolates a and b between t
	static Vector3 Slerp(const Vector3& a, const Vector3& b, float t);
};

#pragma region "Primitive operators"
static Vector3 operator+(float scalar, const Vector3& vec)
{
	return Vector3(scalar + vec.x, scalar + vec.y, scalar + vec.z);
}

static Vector3 operator-(float scalar, const Vector3& vec)
{
	return Vector3(scalar - vec.x, scalar - vec.y, scalar - vec.z);
}
static Vector3 operator*(float scalar, const Vector3& vec)
{
	return Vector3(scalar * vec.x, scalar * vec.y, scalar * vec.z);
}
static Vector3 operator/(float scalar, const Vector3& vec)
{
	return Vector3(scalar / vec.x, scalar / vec.y, scalar / vec.z);
}
#pragma endregion
typedef Vector3 vec3;