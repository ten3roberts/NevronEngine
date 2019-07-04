#pragma once
#include "..\\Math\Vector.h"
#include "..\\Math\Quaternion.h"
#include "..\\Math\Matrix.h"
class Transform
{
public:
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
private:
	Matrix4 m_worldMatrix;
public:
	Transform();
	Transform(Vector3 position, Quaternion rotation, Vector3 scale);

	//Updates the world matrix
	void Update();
	//Overriding members
	void Update(Vector3 position, Quaternion rotation, Vector3 scale);

	Matrix4& getWorldMatrix() { return m_worldMatrix; };
};

