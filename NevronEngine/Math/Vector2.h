#pragma once
#include <string>
#include "..\src\Systemdefs.h"

struct Vector2
{
	Vector2();
	Vector2(float x, float  y);
	//Vector2(float x, float y);

	~Vector2();
	float x, y;


	//Operators:

	operator std::string()
	{
		return str();
	}

	inline float* operator[](int index) { return &x + index; }
	//Returns a costant reference and is therefore safe with cost&
	const float& Get(unsigned int index) const { return *(&x + index); }

	void operator=(const Vector2& vec2);
	void operator=(float  setTo) { x = setTo; y = setTo; }

	Vector2 operator+(const Vector2& vec2) const;
	Vector2 operator-(const Vector2& vec2) const;

	Vector2 operator*(const Vector2& vec2) const;
	Vector2 operator/(const Vector2& vec2) const;



	Vector2 operator+(float scalar) const;
	Vector2 operator-(float scalar) const;

	Vector2 operator*(float scalar) const;
	Vector2 operator/(float scalar) const;


	//operator "?=":
	void operator+=(const Vector2& vec2);
	void operator-=(const Vector2& vec2);

	void operator*=(const Vector2& vec2);
	void operator/=(const Vector2& vec2);

	void operator+=(float scalar);
	void operator-=(float scalar);

	void operator*=(float scalar);
	void operator/=(float scalar);

	float SqrMagnitude() const;
	float Magnitude() const;
	inline float Volume() const { return x * y; }  //Multiplies the components together

	//Returns the normalized version of the vector; does not modify the vector
	Vector2 Normalize() const;
	//Normalizes the vector and assigns it to $out. Out can be the same as self
	void Normalize(Vector2* out);

	//Will clamp the vector's magnitude between a minimum and maximum value
	Vector2 ClampMag(float min, float max) const;
	//Will clamp the vector's magnitude to a minimum value
	Vector2 ClampMinMag(float min) const;
	//Will clamp the vector's magnitude to a maximum value
	Vector2 ClampMaxMag(float max) const;

	//Will clamp all the vectors components individually between a minimum and maximum value;
	Vector2 Clamp(float min, float max) const;
	//Will clamp all the vectors components individually to a minimum;
	Vector2 ClampMin(float min) const;
	//Will clamp all the vectors components individually to a maximum value;
	Vector2 ClampMax(float max) const;

	//Returns false if the vector doesnt have a direction or length
	inline bool Valid() const { return SqrMagnitude() > 0.00001; }
	inline bool Defined() const { return SqrMagnitude() > 0.00001; }

	inline float* Array() { return &x; }

	inline std::string str() { return std::string(STR(x) + ", " + STR(y)); }
	inline std::string str_d() { return std::string(STR(x) + ", " + STR(y) + "; " + STR(Magnitude())); }

	//Calculates the dot product between two vectors
	static float Dot(const Vector2& a, const Vector2& b);

	//Returns a vector projected onto an axis
	static Vector2 Project(const Vector2& vector, const Vector2& axis);
	//Returns the length of a vector projected onto an axis
	static float ProjectFlat(const Vector2& vector, const Vector2& axis);

	//Linearly interpolates a and b between t
	static Vector2 Lerp(const Vector2& a, const Vector2& b, float t);
};

static Vector2 operator+(float scalar, Vector2 vec)
{
	return Vector2(scalar + vec.x, scalar + vec.y);
}

static Vector2 operator-(float scalar, Vector2 vec)
{
	return Vector2(scalar - vec.x, scalar - vec.y);
}
static Vector2 operator*(float scalar, Vector2 vec)
{
	return Vector2(scalar * vec.x, scalar * vec.y);
}
static Vector2 operator/(float scalar, Vector2 vec)
{
	return Vector2(scalar / vec.x, scalar / vec.y);
}

typedef Vector2 vec2;
