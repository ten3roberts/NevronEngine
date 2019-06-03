#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include <algorithm>
#include <vector>

struct Vector
{
	Vector(unsigned int size);

	Vector(std::initializer_list<float> list);

	Vector(unsigned int size, float* list);

	Vector(std::vector<float> list);

	Vector(const Vector& vec);

	//Append constructor
	Vector(const Vector& a, const Vector& b);

	~Vector()
	{
		delete[] m_data;
	}

	static Vector OnSphere(unsigned int size, float radius = 1);
	static Vector InSphere(unsigned int size, float outerRadius, float innerRadius = 0);
	static Vector InSphereEven(unsigned int size, float outerRadius, float innerRadius = 0);
	static Vector InCube(unsigned int size, float width);

	//Operators:

	operator std::string()
	{
		return str();
	}

	inline float& operator[](unsigned int index) { return m_data[index]; }
	//Returns a costant reference and is therefore safe with cost&
	inline const float& get(unsigned int index) const { return m_data[index]; }
	inline unsigned int size() const { return m_size; }

	inline void operator=(const Vector& vec)
	{
		memcpy(m_data, vec.m_data, m_size * sizeof(float));
	}

	//Assumes the list is as big or bigger than the vector
	inline void operator=(float* list)
	{
		memcpy(m_data, list, m_size * sizeof(float));
	}

	Vector operator+(const Vector& vec) const;

	Vector operator-(const Vector& vec) const;

	Vector operator*(const Vector& vec) const;
	Vector operator/(const Vector& vec) const;


	Vector operator+(float scalar) const;

	Vector operator-(float scalar) const;

	Vector operator*(float scalar) const;

	Vector operator/(float scalar) const;



	//operator "?=":
	void operator+=(const Vector& vec);

	void operator-=(const Vector& vec);


	void operator*=(const Vector& vec);

	void operator/=(const Vector& vec);

	void operator+=(float scalar);

	void operator-=(float scalar);


	void operator*=(float scalar);

	void operator/=(float scalar);


	float SqrMagnitude() const;


	float Magnitude() const;

	//Multiplies the components together
	inline float Volume() const;


	//Returns the normalized version of the vector; does not modify the vector
	inline Vector Normalize() const
	{
		return *this / Magnitude();
	}

	//Normalizes the vector and assigns it to $out. Out can be the same as self
	inline void Normalize(Vector* out)
	{
		if (out)
			* out = *this / Magnitude();
	}

	Vector ClampMag(float min, float max) const;

	Vector ClampMinMag(float min) const;

	Vector ClampMaxMag(float max) const;

	Vector Clamp(float min, float max) const;

	Vector ClampMin(float min) const;

	Vector ClampMax(float max) const;

	//Returns false if the vector doesnt have a direction or length
	inline bool Valid() { return SqrMagnitude() > 0.00001; }
	inline bool Defined() { return SqrMagnitude() > 0.00001; }

	inline float* Array() { return &m_data[0]; }

	inline std::string str()
	{
		std::string result;
		for (unsigned int i = 0; i < m_size; i++)
		{
			result += std::to_string(m_data[i]);
			if (i != m_size - 1)
				result += ", ";
		}
		return result;
	}
	inline std::string str_d()
	{
		std::string result;
		for (unsigned int i = 0; i < m_size; i++)
		{
			result += std::to_string(m_data[i]);
			if (i != m_size - 1)
				result += ",";
		}
		result += "; " + std::to_string(Magnitude());
		return result;
	}
	static const Vector red;
	static const Vector green;
	static const Vector blue;

	//Calculates the dot product between two vectors
	/*static float Dot(Vector a, Vector b);

	//Returns a vector projected onto an axis
	static Vector Project(Vector vector, Vector axis);
	//Returns the length of a vector projected onto an axis
	static float ProjectFlat(Vector vector, Vector axis);

	//Linearly interpolates a and b between t
	static Vector Lerp(Vector a, Vector b, float t);*/

private:
	//The element array
	float* m_data;
	unsigned int m_size;

};

using vec = Vector;