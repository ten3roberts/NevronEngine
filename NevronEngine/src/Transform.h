#pragma once
#include <src/pch.h>
#include <src/Component.h>



class Transform : public Component
{
public:
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
	Vector3 forward;
private:
	Matrix4 m_worldMatrix;
	//float padding[4];
public:
	Transform();
	Transform(Vector3 position, Quaternion rotation, Vector3 scale);

	//Updates the world matrix. Make sure to update the transform with a rigidbody first if neccesary
	void Update();
	//Updates the world matrix with overridden values. Make sure to update the transform with a rigidbody first if neccesary
	void Update(Vector3 position, Quaternion rotation, Vector3 scale);

	const Matrix4& getWorldMatrix() const { return m_worldMatrix; };
};




struct TransformType
{
	//TransformType() : position(0), rotation(Quaternion::identity), scale(1), camPos(0), camForward(0), mvp(Matrix4::identity) {}
	//TransformType(const Vector3& position, const Quaternion& rotation, const Vector3& scale, const Vector3& camPos, const Vector3& camForward, const Matrix4& mvp) : position(position), rotation(rotation), scale(scale), camPos(camPos), camForward(camForward), mvp(mvp) {}
	Matrix4 mvp;
	Vector4 position;
	Quaternion rotation;
	Vector4 scale;
	Vector3 camPos;
	float padding;
	//Vector3 camForward;
	//float padding[4];
};