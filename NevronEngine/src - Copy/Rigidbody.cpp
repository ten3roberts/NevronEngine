#include "Rigidbody.h"
#include "Transform.h"

Rigidbody::Rigidbody(float mass) : mass(mass)
{
}

Rigidbody::Rigidbody(Vector3 velocity, float mass) : velocity(velocity), mass(mass)
{
}

Rigidbody::Rigidbody(Vector3 velocity, Vector3 angularVelocity, float mass) : velocity(velocity), angularVelocity(angularVelocity), mass(mass)
{
}

Rigidbody::Rigidbody(Vector3 velocity, Quaternion angularVelocity, float mass) : velocity(velocity), mass(mass)
{
	this->angularVelocity = angularVelocity.toEuler();
}

void Rigidbody::Update(Transform* transform)
{
	transform->position += velocity * Time::deltaTime;
	transform->rotation *= Quaternion(angularVelocity * Time::deltaTime);
}

void Rigidbody::FixedUpdate(Transform* transform)
{
	transform->position += velocity * Time::fixedDeltaTime;
	transform->rotation *= Quaternion(angularVelocity * Time::fixedDeltaTime);
}


