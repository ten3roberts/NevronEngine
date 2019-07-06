#pragma once
#include "..\Math\Vector.h"
#include "..\Math\Quaternion.h"
#include "Time.h"

class Transform;

class Rigidbody
{
public:
	Vector3 velocity;
	Vector3 angularVelocity;
	float mass;
public:
	Rigidbody(float mass = 1);
	Rigidbody(Vector3 velocity, float mass = 1);
	Rigidbody(Vector3 velocity, Vector3 angularVelocity, float mass = 1);
	Rigidbody(Vector3 velocity, Quaternion angularVelocity, float mass = 1);

	//Updates the transform with deltaTime. Do this before calling Transform::Update()
	void Update(Transform* transform);

	//Updates the transform with fixedDeltaTime. Do this before calling Transform::Update()
	void FixedUpdate(Transform* transform);
};

