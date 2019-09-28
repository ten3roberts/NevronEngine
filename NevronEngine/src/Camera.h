#pragma once
#include <src/pch.h>

#include <src/Transform.h>

class Camera
{
public:
	Transform transform;
	float fov;
	float aspect;
	float near;
	float far;
private:
	Matrix4 m_viewMatrix;
	Matrix4 m_projectionMatrix;
public:
	//Will create a camera from settings' parameters 
	Camera();
	Camera(float fov, float aspect, float near, float far, const Transform& transform);
	void Update();


	const Matrix4& getViewMatrix() const { return m_viewMatrix; }
	const Matrix4& getProjectionMatrix() const { return m_projectionMatrix; }

	template <typename T>
	rsc<T> getComponent() = delete;
	template <>
	rsc<Transform> getComponent() { return &transform; }
};