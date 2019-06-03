#pragma once
#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

#include <string>
#include <vector>
#include <cmath>

struct Vector3;

#define MATH_PI 3.141593f
#define MATH_E  2.718282f

#define DEG_180 MATH_PI
#define DEG_90  1.570796f
#define DEG_60  0.047198f
#define DEG_45  0.785398f


//Namepsace for generating random values, arrays, and vectors. No arguments yields a result between 0 and 1
namespace Random
{
	//Will return a random scalar between 0 and 1
	inline static float Rand01()
	{
		return (float)std::rand() / RAND_MAX;
	}

	//Will return a random scalar between 0 and 1
	inline static float Rand01(unsigned int seed)
	{
		std::srand(seed);
		return (float)std::rand() / RAND_MAX;
	}

	//Will return a random scalar between -1 and 1
	inline static float RandNorm()
	{
		return ((float)std::rand() / RAND_MAX) * 2 - 1;
	}

	//Will return a random scalar between -1 and 1
	inline static float RandNorm(unsigned int seed)
	{
		std::srand(seed);
		return ((float)std::rand() / RAND_MAX) * 2 - 1;
	}

	//Will return a random scalar between $min and $max
	inline static float InRange(float min, float max)
	{
		return ((float)std::rand() / RAND_MAX) * (max - min) + min;
	}

	//Will return a random scalar between $min and $max
	inline static float InRange(float min, float max, unsigned int seed)
	{
		std::srand(seed);
		return ((float)std::rand() / RAND_MAX) * (max - min) + min;
	}
	inline std::vector<float> Array(unsigned int size, float min, float max)
	{
		std::vector<float> result(size);
		for(unsigned int i = 0; i < size; i++)
			result[i] = ((float)std::rand() / RAND_MAX) * (max - min) + min;
		return result;
	}

	inline std::vector<float> Array(unsigned int size)
	{
		std::vector<float> result(size);
		for (unsigned int i = 0; i < size; i++)
			result[i] = (float)std::rand() / RAND_MAX;
		return result;
	}	

	//Vector math

	//Will return a vector to a random point on a circle's edge
	Vector2 OnCircle(float radius = 1);

	//Will return a vector with a random direction and magnitude. (Equal chance for each magnitude; sparser further out)
	Vector2 InCircle(float outerRadius, float innerRadius = 0);
	Vector2 InCircle();

	//Will return a vector to a random point on a circle's surface. (Unequal chance for each magnitude; even spread throughout whole sphere)
	Vector2 InCircleEven(float outerRadius, float innerRadius = 0);

	//Unit circle
	Vector2 InCircleEven();

	//Will return a vector to a random point on a sphere
	Vector3 OnSphere(float radius = 1);

	//Will return a vector to a random point on a sphere
	Vector3 OnSphere(float radius, unsigned int seed);
	
	//Will return a a vector with a random direction and magnitude. (Equal chance for each magnitude; sparser further out)
	Vector3 InSphere(float outerRadius, float innerRadius = 0);
	Vector3 InSphere();

	//Will return a vector to a random point inside a sphere. (Unequal chance for each magnitude; even spread throughout whole sphere)
	Vector3 InSphereEven(float outerRadius, float innerRadius = 0);
	Vector3 InSphereEven();

	Vector3 InCube(float width);
}

namespace Math
{
	//public:
	inline static float logn(float x, float base)
	{
		return log(x) / log(base);
	}

	//Will attempt to convert a string to float or integer. If it fails it will return 0 and if $failed is passed set it to true
	template <typename N = int>
	N num(std::string in, bool* failed = nullptr)
	{
		if (failed != nullptr)
			* failed = false;
		try
		{
			if constexpr (std::is_same<N, int>())
			{
				return std::stoi(in);
			}
			else if constexpr (std::is_same<N, float>())
			{
				return std::stof(in);
			}
		}
		catch (...)
		{
			if (failed != nullptr)
				* failed = true;
			return 0;
		}
	}

	inline static float Clamp(float value, float min, float max)
	{
		return value > max ? max : value < min ? min : value;
	}

	inline static float Clamp01(float value)
	{
		return value > 1 ? 1 : value < 0 ? 0 : value;
	}

	inline static float ToRad(float& deg)
	{
		return (MATH_PI / 180.0f) * deg;
	}

	inline static float ToDeg(float& rad)
	{
		return (180.0f / MATH_PI) * rad;
	}

	int GetMax(float* elements, int size); //Return the index of the greatest value in elements
	int GetMin(float* elements, int size); // Returns the index of the smallest value in elements

	int GetMax(std::vector<float> elements);

	int GetMin(std::vector<float> elements);

#pragma region "Vector functions"
	//Vector2
	inline float Dot(const Vector2& a, const Vector2& b) { return Vector2::Dot(a, b); }

	//Returns a vector projected onto an axis
	inline Vector2 Project(const Vector2& vector, const Vector2& axis) { return Vector2::Project(vector, axis); }
	//Returns the length of a vector projected onto an axis
	inline float ProjectFlat(const Vector2& vector, const Vector2& axis) { return Vector2::ProjectFlat(vector, axis); }

	//Linearly interpolates a and b between t
	inline Vector2 Lerp(const Vector2& a, const Vector2& b, float t) { return Vector2::Lerp(a, b, t); }

	//Vector3
	//Calculates the dot product between two vectors
	inline float Dot(const Vector3& a, const Vector3& b) { return Vector3::Dot(a, b); }

	//Returns a vector projected onto an axis
	inline Vector3 Project(const Vector3& vector, const Vector3& axis) { return Vector3::Project(vector, axis); }
	//Returns the length of a vector projected onto an axis
	inline float ProjectFlat(const Vector3& vector, const Vector3& axis) { return Vector3::ProjectFlat(vector, axis); }

	//Returns a vector perpendicular to a and b
	inline Vector3 Cross(const Vector3& a, const Vector3& b) { return Vector3::Cross(a, b); }

	//Returns the vector when the ray has been reflected from the normal
	inline Vector3 Reflect(const Vector3& ray, const Vector3& normal) { return Vector3::Reflect(ray, normal); }

	//Linearly interpolates a and b between t
	inline Vector3 Lerp(const Vector3& a, const Vector3& b, float t) { return Vector3::Lerp(a, b, t); }

	//Spherically interpolates a and b between t
	inline Vector3 Slerp(const Vector3& a, const Vector3& b, float t) { return Vector3::Slerp(a, b, t); }

	//Vector4
	inline float Dot(const Vector4& a, const Vector4& b) { return Vector4::Dot(a, b); }

	//Returns a vector projected onto an axis
	inline Vector4 Project(const Vector4& vector, const Vector4& axis) { return Vector4::Project(vector, axis); }
	//Returns the length of a vector projected onto an axis
	inline float ProjectFlat(const Vector4& vector, const Vector4& axis) { return Vector4::ProjectFlat(vector, axis); }

	//Linearly interpolates a and b between t
	inline Vector4 Lerp(const Vector4& a, const Vector4& b, float t) { return Vector4::Lerp(a, b, t); }
#pragma endregion
};

//Merge Sorting:
template<typename T>
struct Sorter
{
	static void Sort(std::vector<T>& input, Vector3& other, bool(*comp)(T a, T b, Vector3& other))
	{
		MergeSort(input, other, 0, input.size() - 1, comp);
	}
private:
	static void MergeSort(std::vector<T>& input, Vector3& other, int low, int high, bool(*comp)(T a, T b, Vector3& other))
	{
		if (low < high)
		{
			int middle = (low / 2) + (high / 2);
			MergeSort(input, other, low, middle, comp);
			MergeSort(input, other, middle + 1, high, comp);
			Merge(input, other, low, middle, high, comp);
		}
	}

	static void Merge(std::vector<T> & input, Vector3 & other, int low, int middle, int high, bool(*comp)(T a, T b, Vector3 & other))
	{

		int left = low;
		int right = middle + 1;
		std::vector<T> tmp;
		tmp.resize((high - low) + 1);
		int tmpIndex = 0;

		while ((left <= middle) && (right <= high))
		{
			if (comp(input[left], input[right], other))
			{
				tmp[tmpIndex] = input[left];
				left = left + 1;
			}
			else
			{
				tmp[tmpIndex] = input[right];
				right = right + 1;
			}
			tmpIndex = tmpIndex + 1;
		}

		if (left <= middle)
		{
			while (left <= middle)
			{
				tmp[tmpIndex] = input[left];
				left = left + 1;
				tmpIndex = tmpIndex + 1;
			}
		}

		if (right <= high)
		{
			while (right <= high)
			{
				tmp[tmpIndex] = input[right];
				right = right + 1;
				tmpIndex = tmpIndex + 1;
			}
		}

		for (int i = 0; i < tmp.size(); i++)
		{
			input[low + i] = tmp[i];
		}

	}
};