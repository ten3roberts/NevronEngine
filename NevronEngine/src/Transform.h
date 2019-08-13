#pragma once
#include <Math/Math.h>
#include <src/Component.h>

class Transform : public Component
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

	//Updates the world matrix. Make sure to update the transform with a rigidbody first if neccesary
	void Update();
	//Updates the world matrix with overridden values. Make sure to update the transform with a rigidbody first if neccesary
	void Update(Vector3 position, Quaternion rotation, Vector3 scale);

	Matrix4& getWorldMatrix() { return m_worldMatrix; };
};




