#include "Transform.h"

Transform::Transform() : position(0), rotation(0,0,0,1), scale(1,1,1)
{
}

Transform::Transform(Vector3 position, Quaternion rotation, Vector3 scale) : position(position), rotation(rotation), scale(scale)
{
}

void Transform::Update()
{
	m_worldMatrix = Matrix4::Rotate(rotation) * Matrix4::Scale(scale) * Matrix4::Translate(position);
}

void Transform::Update(Vector3 position, Quaternion rotation, Vector3 scale)
{
	m_worldMatrix = Matrix4::Rotate(rotation) * Matrix4::Scale(scale) * Matrix4::Translate(position);
}


