#pragma once
#include "..\src\Systemdefs.h"

#include <string>

struct Vector3;
struct Vector4
{
	Vector4();
	Vector4(float x, float  y, float  z, float  w);
	Vector4(Vector3 vec3, float w = 1);

	Vector4(const std::string& str);

	static Vector4 Parse(const std::string& str);

	~Vector4();
	union
	{
		struct
		{
			float x, y, z, w;
		};
		struct
		{
			float r, g, b, a;
		};
	};
	//float &r = x, &g = y, &b = z, &a = w;


	//Operators:

	operator std::string()
	{
		return str();
	}

	inline float& operator[](int index) { return *(&x + index); }
	//Returns a costant reference and is therefore safe with cost&
	const float& get(unsigned int index) const { return *(&x + index); }

	void operator=(const Vector4& vec4);
	void operator=(float  setTo) { x = setTo; y = setTo; z = setTo; w = setTo; }

	Vector4 operator+(const Vector4& vec4) const;
	Vector4 operator-(const Vector4& vec4) const;

	Vector4 operator*(const Vector4& vec4) const;
	Vector4 operator/(const Vector4& vec4) const;



	Vector4 operator+(float scalar) const;
	Vector4 operator-(float scalar) const;

	Vector4 operator*(float scalar) const;
	Vector4 operator/(float scalar) const;


	//operator "?=":
	void operator+=(const Vector4& vec4);
	void operator-=(const Vector4& vec4);

	void operator*=(const Vector4& vec4);
	void operator/=(const Vector4& vec4);

	void operator+=(float scalar);
	void operator-=(float scalar);

	void operator*=(float scalar);
	void operator/=(float scalar);

	float SqrMagnitude() const;
	float Magnitude() const;
	inline float Volume() const { return x * y* z* w; }  //Multiplies the components together


	//Returns the Normalised version of the vector; does not modify the vector
	Vector4 Normalise() const;

	//Normalises the vector and assigns it to $out. Out can be the same as self
	void Normalise(Vector4* out);

	//Will strClamp the vector's magnitude between a minimum and maximum value
	Vector4 ClampMag(float min, float max) const;
	//Will strClamp the vector's magnitude to a minimum value
	Vector4 ClampMinMag(float min) const;
	//Will strClamp the vector's magnitude to a maximum value
	Vector4 ClampMaxMag(float max) const;

	//Will strClamp all the vectors components individually between a minimum and maximum value;
	Vector4 strClamp(float min, float max) const;
	//Will strClamp all the vectors components individually to a minimum;
	Vector4 ClampMin(float min) const;
	//Will strClamp all the vectors components individually to a maximum value;
	Vector4 ClampMax(float max) const;

	//Returns false if the vector doesnt have a direction or length
	inline bool Valid() const { return SqrMagnitude() > 0.00001; }
	inline bool Defined() const { return SqrMagnitude() > 0.00001; }

	inline float* Array() { return &x; }

	inline std::string str() const { return std::string(STR(x) + ", " + STR(y) + ", " + STR(z) + ", " + STR(w)); }
	inline std::string str_d() const { return std::string(STR(x) + ", " + STR(y) + ", " + STR(z) + ", " + STR(w) + "; " + STR(Magnitude())); }

	static const Vector4 one;
	static const Vector4 zero;
	static const Vector4 red;
	static const Vector4 green;
	static const Vector4 blue;
	static const Vector4 white;
	static const Vector4 black;
	

	//Calculates the dot product between two vectors
	static float Dot(const Vector4& a, const Vector4& b);

	//Returns a vector projected onto an axis
	static Vector4 Project(const Vector4& vector, const Vector4& axis);
	//Returns the length of a vector projected onto an axis
	static float ProjectFlat(const Vector4& vector, const Vector4& axis);

	//Linearly interpolates a and b between t
	static Vector4 Lerp(const Vector4& a, const Vector4& b, float t);
};

static Vector4 operator+(float& scalar, const Vector4& vec)
{
	return Vector4(scalar + vec.x, scalar + vec.y, scalar + vec.z, scalar + vec.w);
}

static Vector4 operator-(float scalar, const Vector4& vec)
{
	return Vector4(scalar - vec.x, scalar - vec.y, scalar - vec.z, scalar - vec.w);
}
static Vector4 operator*(float& scalar, const Vector4& vec)
{
	return Vector4(scalar * vec.x, scalar * vec.y, scalar * vec.z, scalar * vec.w);
}
static Vector4 operator/(float scalar, const Vector4& vec)
{
	return Vector4(scalar / vec.x, scalar / vec.y, scalar / vec.z, scalar / vec.w);
}

typedef Vector4 vec4;


