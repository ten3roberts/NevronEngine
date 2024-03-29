#pragma once
#include <src/pch.h>
#include <Math/Vector3.h>
#include <Math/Quaternion.h>
#include "Time.h"
#include <src/Component.h>

class Transform;

class Rigidbody : public Component
{
public:
	Vector3 velocity;
	Vector3 angularVelocity;
	float mass;
public:
	Rigidbody(float mass = 1);
	Rigidbody(Vector3 velocity, float mass = 1);
	Rigidbody(Vector3 velocity, Vector3 angularVelocity, float mass = 1.0f);
	Rigidbody(Vector3 velocity, Quaternion angularVelocity, float mass = 1.0f);
	~Rigidbody();

	//Updates the transform with deltaTime. Do this before calling Transform::Update()
	void Update(rsc_weak<Transform> transform);

	//Updates the transform with fixedDeltaTime. Do this before calling Transform::Update()
	void FixedUpdate(Transform* transform);
};




